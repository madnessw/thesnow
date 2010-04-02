// SciTE - Scintilla based Text Editor
/** @file Extra.h 
 ** 
 **/
// Copyright 2010 by thesnoW <thegfw@gmail.com>
// The License.txt file describes the conditions under which this software may be distributed.
//#include "SciTEBase.h"
#include <windows.h>
class ToolTip {
public:
	HWND hwndTip;
	HWND Create(HWND ControlHWND, LPTSTR lpszText);
	void Delete(HWND ControlHWND);
};

class LanguageIni{
public:
	wchar_t lang[255];
	char langA[255];
	LPWSTR GetValue(LPWSTR lpSection,LPWSTR lpKey,LPWSTR lpDefault);
	LPCSTR GetValueA(LPCSTR lpSection,LPCSTR lpKey,LPCSTR lpDefault);
};