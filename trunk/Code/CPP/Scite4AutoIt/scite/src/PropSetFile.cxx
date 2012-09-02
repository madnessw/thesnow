﻿// SciTE - Scintilla based Text Editor
/** @file PropSetFile.cxx
 ** Property set implementation.
 ** 属性文件设置执行
 **/
// Copyright 1998-2007 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <locale.h>
#include <assert.h>

#include <string>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <algorithm>

#include <Windows.h> //added
#if defined(GTK)

#include <unistd.h>

#endif

#include "Scintilla.h"

#include "GUI.h"

#include "SString.h"
#include "FilePath.h"
#include "PropSetFile.h"

// The comparison and case changing functions here assume ASCII
// or extended ASCII such as the normal Windows code page.

static inline char MakeUpperCase(char ch) {
	if (ch < 'a' || ch > 'z')
		return ch;
	else
		return static_cast<char>(ch - 'a' + 'A');
}

inline bool IsASpace(unsigned int ch) {
    return (ch == ' ') || ((ch >= 0x09) && (ch <= 0x0d));
}

static std::set<std::string> FilterFromString(std::string values) {
	std::set<std::string> fs;
	std::istringstream isValues(values);
	while (!isValues.eof()) {
		std::string sValue;
		isValues >> sValue;
		if (!sValue.empty())
			fs.insert(sValue);
	}
	return fs;
}

void ImportFilter::SetFilter(std::string sExcludes, std::string sIncludes) {
	excludes = FilterFromString(sExcludes);
	includes = FilterFromString(sIncludes);
}

bool ImportFilter::IsValid(std::string name) const {
	if (!includes.empty()) {
		return includes.count(name) > 0;
	} else {
		return excludes.count(name) == 0;
	}
}

bool PropSetFile::caseSensitiveFilenames = false;

PropSetFile::PropSetFile(bool lowerKeys_) : lowerKeys(lowerKeys_), superPS(0) {
}

PropSetFile::PropSetFile(const PropSetFile &copy) : lowerKeys(copy.lowerKeys), props(copy.props), superPS(copy.superPS) {
}

PropSetFile::~PropSetFile() {
	superPS = 0;
	Clear();
}

PropSetFile &PropSetFile::operator=(const PropSetFile &assign) {
	if (this != &assign) {
		lowerKeys = assign.lowerKeys;
		superPS = assign.superPS;
		props = assign.props;
	}
	return *this;
}

void PropSetFile::Set(const char *key, const char *val, ptrdiff_t lenKey, ptrdiff_t lenVal) {
	if (!*key)	// Empty keys are not supported
		return;
	if (lenKey == -1)
		lenKey = static_cast<int>(strlen(key));
	if (lenVal == -1)
		lenVal = static_cast<int>(strlen(val));
	props[std::string(key, lenKey)] = std::string(val, lenVal);
}

void PropSetFile::Set(const char *keyVal) {
	while (IsASpace(*keyVal))
		keyVal++;
	const char *endVal = keyVal;
	while (*endVal && (*endVal != '\n'))
		endVal++;
	const char *eqAt = strchr(keyVal, '=');
	if (eqAt) {
		Set(keyVal, eqAt + 1, eqAt-keyVal, endVal - eqAt - 1);
	} else if (*keyVal) {	// No '=' so assume '=1'
		Set(keyVal, "1", endVal-keyVal, 1);
	}
}

void PropSetFile::Unset(const char *key, int lenKey) {
	if (!*key)	// Empty keys are not supported
		return;
	if (lenKey == -1)
		lenKey = static_cast<int>(strlen(key));
	mapss::iterator keyPos = props.find(std::string(key, lenKey));
	if (keyPos != props.end())
		props.erase(keyPos);
}

void PropSetFile::SetMultiple(const char *s) {
	const char *eol = strchr(s, '\n');
	while (eol) {
		Set(s);
		s = eol + 1;
		eol = strchr(s, '\n');
	}
	Set(s);
}

bool PropSetFile::Exists(const char *key) const {
	mapss::const_iterator keyPos = props.find(std::string(key));
	if (keyPos != props.end()) {
		return true;
	} else {
		if (superPS) {
			// Failed here, so try in base property set
			return superPS->Exists(key);
		} else {
			return false;
		}
	}
}

SString PropSetFile::Get(const char *key) const {
	const std::string sKey(key);
	const PropSetFile *psf = this;
	while (psf) {
		mapss::const_iterator keyPos = psf->props.find(sKey);
		if (keyPos != psf->props.end()) {
			return SString(keyPos->second.c_str());
		}
		// Failed here, so try in base property set
		psf = psf->superPS;
	}
	return "";
}

static SString ShellEscape(const char *toEscape) {
	SString str(toEscape);
	for (int i = static_cast<int>(str.length()-1); i >= 0; --i) {
		switch (str[i]) {
		case ' ':
		case '|':
		case '&':
		case ',':
		case '`':
		case '"':
		case ';':
		case ':':
		case '!':
		case '^':
		case '$':
		case '{':
		case '}':
		case '(':
		case ')':
		case '[':
		case ']':
		case '=':
		case '<':
		case '>':
		case '\\':
		case '\'':
			str.insert(i, "\\");
			break;
		default:
			break;
		}
	}
	return str.c_str();
}

SString PropSetFile::Evaluate(const char *key) const {
	if (strchr(key, ' ')) {
		if (isprefix(key, "escape ")) {
			SString val = Get(key+7);
			return ShellEscape(val.c_str());
		} else if (isprefix(key, "star ")) {
			const std::string sKeybase(key + 5);
			// Create set of variables with values
			mapss values;
			// For this property set and all base sets
			for (const PropSetFile *psf = this; psf; psf = psf->superPS) {
				mapss::const_iterator it = psf->props.lower_bound(sKeybase);
				while ((it != psf->props.end()) && (it->first.find(sKeybase) == 0)) {
					mapss::iterator itDestination = values.find(it->first);
					if (itDestination == values.end()) {
						// Not present so add
						values[it->first] = it->second;
					}
					++it;
				}
			}
			// Concatenate all variables
			std::string combination;
			for (mapss::const_iterator itV = values.begin(); itV != values.end(); ++itV) {
				combination += itV->second;
			}
			return SString(combination.c_str());
		}
	} else {
		return Get(key);
	}
	return "";
}

// There is some inconsistency between GetExpanded("foo") and Expand("$(foo)").
// A solution is to keep a stack of variables that have been expanded, so that
// recursive expansions can be skipped.  For now I'll just use the C++ stack
// for that, through a recursive function and a simple chain of pointers.

struct VarChain {
	VarChain(const char*var_=NULL, const VarChain *link_=NULL): var(var_), link(link_) {}

	bool contains(const char *testVar) const {
		return (var && (0 == strcmp(var, testVar)))
			|| (link && link->contains(testVar));
	}

	const char *var;
	const VarChain *link;
};

static int ExpandAllInPlace(const PropSetFile &props, SString &withVars, int maxExpands, const VarChain &blankVars = VarChain()) {
	int varStart = withVars.search("$(");
	while ((varStart >= 0) && (maxExpands > 0)) {
		int varEnd = withVars.search(")", varStart+2);
		if (varEnd < 0) {
			break;
		}

		// For consistency, when we see '$(ab$(cde))', expand the inner variable first,
		// regardless whether there is actually a degenerate variable named 'ab$(cde'.
		int innerVarStart = withVars.search("$(", varStart+2);
		while ((innerVarStart > varStart) && (innerVarStart < varEnd)) {
			varStart = innerVarStart;
			innerVarStart = withVars.search("$(", varStart+2);
		}

		SString var(withVars.c_str(), varStart + 2, varEnd);
		SString val = props.Evaluate(var.c_str());

		if (blankVars.contains(var.c_str())) {
			val.clear(); // treat blankVar as an empty string (e.g. to block self-reference)
		}

		if (--maxExpands >= 0) {
			maxExpands = ExpandAllInPlace(props, val, maxExpands, VarChain(var.c_str(), &blankVars));
		}

		withVars.remove(varStart, varEnd-varStart+1);
		withVars.insert(varStart, val.c_str(), val.length());

		varStart = withVars.search("$(");
	}

	return maxExpands;
}

SString PropSetFile::GetExpanded(const char *key) const {
	SString val = Get(key);
	ExpandAllInPlace(*this, val, 100, VarChain(key));
	return val;
}

SString PropSetFile::Expand(const char *withVars, int maxExpands) const {
	SString val = withVars;
	ExpandAllInPlace(*this, val, maxExpands);
	return val;
}

int PropSetFile::GetInt(const char *key, int defaultValue) const {
	SString val = GetExpanded(key);
	if (val.length())
		return val.value();
	return defaultValue;
}

static bool isPrefix(const char *target, const char *prefix) {
	while (*target && *prefix) {
		if (*target != *prefix)
			return false;
		target++;
		prefix++;
	}
	if (*prefix)
		return false;
	else
		return true;
}

void PropSetFile::Clear() {
	props.clear();
}

char *PropSetFile::ToString() const {
	std::string sval;
	for (mapss::const_iterator it=props.begin(); it != props.end(); ++it) {
		sval += it->first;
		sval += "=";
		sval += it->second;
		sval += "\n";
	}
	char *ret = new char [sval.size() + 1];
	strcpy(ret, sval.c_str());
	return ret;
}

/**
 * Get a line of input. If end of line escaped with '\\' then continue reading.
 */
static bool GetFullLine(const char *&fpc, size_t &lenData, char *s, size_t len) {
	bool continuation = true;
	s[0] = '\0';
	while ((len > 1) && lenData > 0) {
		char ch = *fpc;
		fpc++;
		lenData--;
		if ((ch == '\r') || (ch == '\n')) {
			if (!continuation) {
				if ((lenData > 0) && (ch == '\r') && ((*fpc) == '\n')) {
					// munch the second half of a crlf
					fpc++;
					lenData--;
				}
				*s = '\0';
				return true;
			}
		} else if ((ch == '\\') && (lenData > 0) && ((*fpc == '\r') || (*fpc == '\n'))) {
			continuation = true;
			if ((lenData > 1) && (((*fpc == '\r') && (*(fpc+1) == '\r')) || ((*fpc == '\n') && (*(fpc+1) == '\n'))))
				continuation = false;
			else if ((lenData > 2) && ((*fpc == '\r') && (*(fpc+1) == '\n') && (*(fpc+2) == '\n' || *(fpc+2) == '\r')))
				continuation = false;
		} else {
			continuation = false;
			*s++ = ch;
			*s = '\0';
			len--;
		}
	}
	return false;
}

static bool IsSpaceOrTab(char ch) {
	return (ch == ' ') || (ch == '\t');
}

static bool IsCommentLine(const char *line) {
	while (IsSpaceOrTab(*line)) ++line;
	return (*line == '#');
}

bool IsPropertiesFile(const FilePath &filename) {
	FilePath ext = filename.Extension();
	if (EqualCaseInsensitive(ext.AsUTF8().c_str(), PROPERTIES_EXTENSION + 1))
		return true;
	return false;
}

static bool GenericPropertiesFile(const FilePath &filename) {
//**FIXED IT IN CHINESE VERSION,DO NOT CHANGE IT***
	GUI::gui_string name = filename.BaseName().AsInternal();
	if (name == L"abbrev" || name == L"Embedded"||name ==L"全局缩写" || name==L"全局设置")
		return true;
	return name.find(L"SciTE") != std::wstring::npos;
//**FIXED IT IN CHINESE VERSION,DO NOT CHANGE IT***
}

void PropSetFile::Import(FilePath filename, FilePath directoryForImports, const ImportFilter &filter, std::vector<FilePath> *imports) {
	if (Read(filename, directoryForImports, filter, imports)) {
		if (imports && (std::find(imports->begin(),imports->end(), filename) == imports->end())) {
			imports->push_back(filename);
		}
	}
}

bool PropSetFile::ReadLine(const char *lineBuffer, bool ifIsTrue, FilePath directoryForImports,
                           const ImportFilter &filter, std::vector<FilePath> *imports) {
	//UnSlash(lineBuffer);
	if (!IsSpaceOrTab(lineBuffer[0]))    // If clause ends with first non-indented line
		ifIsTrue = true;
	if (isPrefix(lineBuffer, "if ")) {
		const char *expr = lineBuffer + strlen("if") + 1;
		ifIsTrue = GetInt(expr) != 0;
	} else if (isPrefix(lineBuffer, "import ") && directoryForImports.IsSet()) {
		SString importName(lineBuffer + strlen("import") + 1);
		if (importName == "*") {
			// Import all .properties files in this directory except for system properties
			FilePathSet directories;
			FilePathSet files;
			directoryForImports.List(directories, files);
			for (size_t i = 0; i < files.size(); i ++) {
				FilePath fpFile = files[i];
				if (IsPropertiesFile(fpFile) &&
					!GenericPropertiesFile(fpFile) &&
					filter.IsValid(fpFile.BaseName().AsUTF8())) {
					FilePath importPath(directoryForImports, fpFile);
					Import(importPath, directoryForImports, filter, imports);
				}
			}
		} else if (filter.IsValid(importName.c_str())) {
			importName += ".properties";
			FilePath importPath(directoryForImports, FilePath(GUI::StringFromUTF8(importName.c_str())));
			Import(importPath, directoryForImports, filter, imports);
		}
	} else if (ifIsTrue && !IsCommentLine(lineBuffer)) {
		Set(lineBuffer);
	}
	return ifIsTrue;
}

void PropSetFile::ReadFromMemory(const char *data, size_t len, FilePath directoryForImports,
                                 const ImportFilter &filter, std::vector<FilePath> *imports) {
	const char *pd = data;
	std::vector<char> lineBuffer(len+1);	// +1 for NUL
	bool ifIsTrue = true;
	while (len > 0) {
		GetFullLine(pd, len, &lineBuffer[0], lineBuffer.size());
		if (lowerKeys) {
			for (int i=0; lineBuffer[i] && (lineBuffer[i] != '='); i++) {
				if ((lineBuffer[i] >= 'A') && (lineBuffer[i] <= 'Z')) {
					lineBuffer[i] = static_cast<char>(lineBuffer[i] - 'A' + 'a');
				}
			}
		}
		ifIsTrue = ReadLine(&lineBuffer[0], ifIsTrue, directoryForImports, filter, imports);
	}
}

bool PropSetFile::Read(FilePath filename, FilePath directoryForImports,
                       const ImportFilter &filter, std::vector<FilePath> *imports) {
	FILE *rcfile = filename.Open(fileRead);
	if (rcfile) {
		fseek(rcfile, 0, SEEK_END);
		long sizeFile = ftell(rcfile);
		if (sizeFile == 0) {
			fclose(rcfile);
			return false;
		}
		fseek(rcfile, 0, SEEK_SET);
		std::vector<char> propsData(sizeFile);
		int lenFile = static_cast<int>(fread(&propsData[0], 1, propsData.size(), rcfile));
		fclose(rcfile);
		const char *data = propsData.data();
		if (memcmp(data, "\xef\xbb\xbf", 3) == 0) {
			data += 3;
			lenFile -= 3;
		}
		ReadFromMemory(data, lenFile, directoryForImports, filter, imports);
		return true;
	}
	return false;
}

void PropSetFile::SetInteger(const char *key, int i) {
	char tmp[32];
	sprintf(tmp, "%d", static_cast<int>(i));
	Set(key, tmp);
}

static bool StringEqual(const char *a, const char *b, size_t len, bool caseSensitive) {
	if (caseSensitive) {
		for (size_t i = 0; i < len; i++) {
			if (a[i] != b[i])
				return false;
		}
	} else {
		for (size_t i = 0; i < len; i++) {
			if (MakeUpperCase(a[i]) != MakeUpperCase(b[i]))
				return false;
		}
	}
	return true;
}

// Match file names to patterns allowing for a '*' suffix or prefix.
static bool MatchWild(const char *pattern, size_t lenPattern, const char *fileName, bool caseSensitive) {
	size_t lenFileName = strlen(fileName);
	if (lenFileName == lenPattern) {
		if (StringEqual(pattern, fileName, lenFileName, caseSensitive)) {
			return true;
		}
	}
	if (lenFileName >= lenPattern-1) {
		if (pattern[0] == '*') {
			// Matching suffixes
			return StringEqual(pattern+1, fileName + lenFileName - (lenPattern-1), lenPattern-1, caseSensitive);
		} else if (pattern[lenPattern-1] == '*') {
			// Matching prefixes
			return StringEqual(pattern, fileName, lenPattern-1, caseSensitive);
		}
	}
	return false;
}

static bool startswith(const std::string &s, const char *keybase) {
	return isPrefix(s.c_str(), keybase);
}

SString PropSetFile::GetWildUsingStart(const PropSetFile &psStart, const char *keybase, const char *filename) {
	const std::string sKeybase(keybase);
	const size_t lenKeybase = strlen(keybase);
	const PropSetFile *psf = this;
	while (psf) {
		mapss::const_iterator it = psf->props.lower_bound(sKeybase);
		while ((it != psf->props.end()) && startswith(it->first, keybase)) {
			const char *orgkeyfile = it->first.c_str() + lenKeybase;
			char *keyptr = NULL;

			if (strncmp(orgkeyfile, "$(", 2) == 0) {
				const char *cpendvar = strchr(orgkeyfile, ')');
				if (cpendvar) {
					SString var(orgkeyfile, 2, cpendvar-orgkeyfile);
					SString s = psStart.GetExpanded(var.c_str());
					keyptr = StringDup(s.c_str());
				}
			}
			const char *keyfile = keyptr;

			if (keyfile == NULL)
				keyfile = orgkeyfile;

			for (;;) {
				const char *del = strchr(keyfile, ';');
				if (del == NULL)
					del = keyfile + strlen(keyfile);
				if (MatchWild(keyfile, del - keyfile, filename, caseSensitiveFilenames)) {
					delete []keyptr;
					return SString(it->second.c_str());
				}
				if (*del == '\0')
					break;
				keyfile = del + 1;
			}
			delete []keyptr;

			if (0 == strcmp(it->first.c_str(), keybase)) {
				return SString(it->second.c_str());
			}
			++it;
		}
		// Failed here, so try in base property set
		psf = psf->superPS;
	}
	return "";
}

SString PropSetFile::GetWild(const char *keybase, const char *filename) {
	return GetWildUsingStart(*this, keybase, filename);
}

// GetNewExpand does not use Expand as it has to use GetWild with the filename for each
// variable reference found.
SString PropSetFile::GetNewExpand(const char *keybase, const char *filename) {
	char *base = StringDup(GetWild(keybase, filename).c_str());
	assert(base);
	char *cpvar = strstr(base, "$(");
	int maxExpands = 1000;	// Avoid infinite expansion of recursive definitions
	while (cpvar && (maxExpands > 0)) {
		const char *cpendvar = strchr(cpvar, ')');
		if (cpendvar) {
			ptrdiff_t lenvar = cpendvar - cpvar - 2;  	// Subtract the $()
			char *var = StringDup(cpvar + 2, lenvar);
			assert(var);
			SString val = GetWild(var, filename);
			if (0 == strcmp(var, keybase))
				val.clear(); // Self-references evaluate to empty string
			size_t newlenbase = strlen(base) + val.length() - lenvar;
			char *newbase = new char[newlenbase];
			strncpy(newbase, base, cpvar - base);
			strcpy(newbase + (cpvar - base), val.c_str());
			strcpy(newbase + (cpvar - base) + val.length(), cpendvar + 1);
			delete []var;
			delete []base;
			base = newbase;
		}
		cpvar = strstr(base, "$(");
		maxExpands--;
	}
	SString sret = base;
	delete []base;
	return sret;
}

/**
 * Initiate enumeration.
 */
bool PropSetFile::GetFirst(const char *&key, const char *&val) {
	mapss::iterator it = props.begin();
	if (it != props.end()) {
		key = it->first.c_str();
		val = it->second.c_str();
		return true;
	} else {
		return false;
	}
}

/**
 * Continue enumeration.
 */
bool PropSetFile::GetNext(const char *&key, const char *&val) {
	mapss::iterator it = props.find(key);
	if (it != props.end()) {
		++it;
		if (it != props.end()) {
			key = it->first.c_str();
			val = it->second.c_str();
			return true;
		}
	}
	return false;
}

static inline bool IsLetter(char ch) {
	return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));
}
//added
const wchar_t * SString::w_str(){
	DWORD dwNum = ::MultiByteToWideChar (CP_ACP, 0, s, -1, NULL, 0);
	wchar_t *pwText;
	pwText = new wchar_t[dwNum];
	if(!pwText)
	{
		delete []pwText;
		return L"";
	}
	::MultiByteToWideChar (CP_ACP, 0, s, -1, pwText, dwNum);
	//delete []pwText;
	return pwText;
};
//added
int CompareNoCase(const char *a, const char *b) {
	while (*a && *b) {
		if (*a != *b) {
			char upperA = MakeUpperCase(*a);
			char upperB = MakeUpperCase(*b);
			if (upperA != upperB)
				return upperA - upperB;
		}
		a++;
		b++;
	}
	// Either *a or *b is nul
	return *a - *b;
}

bool EqualCaseInsensitive(const char *a, const char *b) {
	return 0 == CompareNoCase(a, b);
}

// Since the CaseInsensitive functions declared in SString
// are implemented here, I will for now put the non-inline
// implementations of the SString members here as well, so
// that I can quickly see what effect this has.

SString::SString(int i) : sizeGrowth(sizeGrowthDefault) {
	char number[32];
	sprintf(number, "%0d", i);
	s = StringAllocate(number);
	sSize = sLen = (s) ? strlen(s) : 0;
}

SString::SString(size_t i) : sizeGrowth(sizeGrowthDefault) {
	std::ostringstream strstrm;
	strstrm << i;
	s = StringAllocate(strstrm.str().c_str());
	sSize = sLen = (s) ? strlen(s) : 0;
}

SString::SString(double d, int precision) : sizeGrowth(sizeGrowthDefault) {
	char number[32];
	sprintf(number, "%.*f", precision, d);
	s = StringAllocate(number);
	sSize = sLen = (s) ? strlen(s) : 0;
}

bool SString::grow(lenpos_t lenNew) {
	while (sizeGrowth * 6 < lenNew) {
		sizeGrowth *= 2;
	}
	char *sNew = new char[lenNew + sizeGrowth + 1];
	if (sNew) {
		if (s) {
			memcpy(sNew, s, sLen);
			delete []s;
		}
		s = sNew;
		s[sLen] = '\0';
		sSize = lenNew + sizeGrowth;
	}
	return sNew != 0;
}

SString &SString::assign(const char *sOther, lenpos_t sSize_) {
	if (!sOther) {
		sSize_ = 0;
	} else if (sSize_ == measure_length) {
		sSize_ = strlen(sOther);
	}
	if (sSize > 0 && sSize_ <= sSize) {	// Does not allocate new buffer if the current is big enough
		if (s) {
			if (sSize_) {
				memcpy(s, sOther, sSize_);
			}
			s[sSize_] = '\0';
		}
		sLen = sSize_;
	} else {
		delete []s;
		s = StringAllocate(sOther, sSize_);
		if (s) {
			sSize = sSize_;	// Allow buffer bigger than real string, thus providing space to grow
			sLen = sSize_;
		} else {
			sSize = sLen = 0;
		}
	}
	return *this;
}

bool SString::operator==(const SString &sOther) const {
	if ((s == 0) && (sOther.s == 0))
		return true;
	if ((s == 0) || (sOther.s == 0))
		return false;
	return strcmp(s, sOther.s) == 0;
}

bool SString::operator==(const char *sOther) const {
	if ((s == 0) && (sOther == 0))
		return true;
	if ((s == 0) || (sOther == 0))
		return false;
	return strcmp(s, sOther) == 0;
}

SString SString::substr(lenpos_t subPos, lenpos_t subLen) const {
	if (subPos >= sLen) {
		return SString();					// return a null string if start index is out of bounds
	}
	if ((subLen == measure_length) || (subPos + subLen > sLen)) {
		subLen = sLen - subPos;		// can't substr past end of source string
	}
	return SString(s, subPos, subPos + subLen);
}

SString &SString::lowercase(lenpos_t subPos, lenpos_t subLen) {
	if ((subLen == measure_length) || (subPos + subLen > sLen)) {
		subLen = sLen - subPos;		// don't apply past end of string
	}
	for (lenpos_t i = subPos; i < subPos + subLen; i++) {
		if (s[i] < 'A' || s[i] > 'Z')
			continue;
		else
			s[i] = static_cast<char>(s[i] - 'A' + 'a');
	}
	return *this;
}

SString &SString::uppercase(lenpos_t subPos, lenpos_t subLen) {
	if ((subLen == measure_length) || (subPos + subLen > sLen)) {
		subLen = sLen - subPos;		// don't apply past end of string
	}
	for (lenpos_t i = subPos; i < subPos + subLen; i++) {
		if (s[i] < 'a' || s[i] > 'z')
			continue;
		else
			s[i] = static_cast<char>(s[i] - 'a' + 'A');
	}
	return *this;
}

SString &SString::append(const char *sOther, lenpos_t sLenOther, char sep) {
	if (!sOther) {
		return *this;
	}
	if (sLenOther == measure_length) {
		sLenOther = strlen(sOther);
	}
	int lenSep = 0;
	if (sLen && sep) {	// Only add a separator if not empty
		lenSep = 1;
	}
	lenpos_t lenNew = sLen + sLenOther + lenSep;
	// Conservative about growing the buffer: don't do it, unless really needed
	if ((lenNew < sSize) || (grow(lenNew))) {
		if (lenSep) {
			s[sLen] = sep;
			sLen++;
		}
		memcpy(&s[sLen], sOther, sLenOther);
		sLen += sLenOther;
		s[sLen] = '\0';
	}
	return *this;
}

SString &SString::insert(lenpos_t pos, const char *sOther, lenpos_t sLenOther) {
	if (!sOther || pos > sLen) {
		return *this;
	}
	if (sLenOther == measure_length) {
		sLenOther = strlen(sOther);
	}
	lenpos_t lenNew = sLen + sLenOther;
	// Conservative about growing the buffer: don't do it, unless really needed
	if ((lenNew < sSize) || grow(lenNew)) {
		lenpos_t moveChars = sLen - pos + 1;
		for (lenpos_t i = moveChars; i > 0; i--) {
			s[pos + sLenOther + i - 1] = s[pos + i - 1];
		}
		memcpy(s + pos, sOther, sLenOther);
		sLen = lenNew;
	}
	return *this;
}

/**
 * Remove @a len characters from the @a pos position, included.
 * Characters at pos + len and beyond replace characters at pos.
 * If @a len is 0, or greater than the length of the string
 * starting at @a pos, the string is just truncated at @a pos.
 */
void SString::remove(lenpos_t pos, lenpos_t len) {
	if (pos >= sLen) {
		return;
	}
	if (len < 1 || pos + len >= sLen) {
		s[pos] = '\0';
		sLen = pos;
	} else {
		for (lenpos_t i = pos; i < sLen - len + 1; i++) {
			s[i] = s[i+len];
		}
		sLen -= len;
	}
}

bool SString::startswith(const char *prefix) {
	lenpos_t lenPrefix = strlen(prefix);
	if (lenPrefix > sLen) {
		return false;
	}
	return strncmp(s, prefix, lenPrefix) == 0;
}

bool SString::endswith(const char *suffix) {
	lenpos_t lenSuffix = strlen(suffix);
	if (lenSuffix > sLen) {
		return false;
	}
	return strncmp(s + sLen - lenSuffix, suffix, lenSuffix) == 0;
}

int SString::search(const char *sFind, lenpos_t start) const {
	if (start < sLen) {
		const char *sFound = strstr(s + start, sFind);
		if (sFound) {
			return static_cast<int>(sFound - s);
		}
	}
	return -1;
}

int SString::substitute(char chFind, char chReplace) {
	int c = 0;
	char *t = s;
	while (t) {
		t = strchr(t, chFind);
		if (t) {
			*t = chReplace;
			t++;
			c++;
		}
	}
	return c;
}

int SString::substitute(const char *sFind, const char *sReplace) {
	int c = 0;
	lenpos_t lenFind = strlen(sFind);
	lenpos_t lenReplace = strlen(sReplace);
	int posFound = search(sFind);
	while (posFound >= 0) {
		remove(posFound, lenFind);
		insert(posFound, sReplace, lenReplace);
		posFound = search(sFind, posFound + lenReplace);
		c++;
	}
	return c;
}

char *SContainer::StringAllocate(lenpos_t len) {
	if (len != measure_length) {
		return new char[len + 1];
	} else {
		return 0;
	}
}

char *SContainer::StringAllocate(const char *sValue, lenpos_t len) {
	if (sValue == 0) {
		return 0;
	}
	if (len == measure_length) {
		len = strlen(sValue);
	}
	char *sNew = new char[len + 1];
	if (sNew) {
		memcpy(sNew, sValue, len);
		sNew[len] = '\0';
	}
	return sNew;
}

// End SString functions

bool isprefix(const char *target, const char *prefix) {
	while (*target && *prefix) {
		if (*target != *prefix)
			return false;
		target++;
		prefix++;
	}
	if (*prefix)
		return false;
	else
		return true;
}
