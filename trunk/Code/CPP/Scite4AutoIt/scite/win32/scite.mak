# Make file for SciTE on Windows Visual C++ and Borland C++ version
# Copyright 1998-2001 by Neil Hodgson <neilh@scintilla.org>
# The License.txt file describes the conditions under which this software may be distributed.
# This makefile is for using Visual C++ with nmake or Borland C++ with make depending on
# the setting of the VENDOR macro. If no VENDOR is defined n the command line then
# the tool used is automatically detected.
# Usage for Microsoft:
#     nmake -f scite.mak
# Usage for Borland:
#     make -f scite.mak
# For debug versions define DEBUG on the command line.
# For a build without Lua, define NO_LUA on the command line.
# The main makefile uses mingw32 gcc and may be more current than this file.

.SUFFIXES: .cxx

DIR_BIN=..\bin
PROP_DIR=..\bin\properties

PROG=$(DIR_BIN)\SciTE.exe
PROGSTATIC=$(DIR_BIN)\Sc1.exe
DLLS=$(DIR_BIN)\Scintilla.dll $(DIR_BIN)\SciLexer.dll

!IFNDEF VENDOR
!IFDEF _NMAKE_VER
#Microsoft nmake so make default VENDOR MICROSOFT
VENDOR=MICROSOFT
!ELSE
VENDOR=BORLAND
!ENDIF
!ENDIF

!IF "$(VENDOR)"=="MICROSOFT"

CC=cl
RC=rc
LD=link

CXXFLAGS=-Zi -TP -W4 -EHsc -Zc:forScope -Zc:wchar_t -D_CRT_SECURE_NO_DEPRECATE=1 -D_CRT_NONSTDC_NO_DEPRECATE
CCFLAGS=-TC -W3 -wd4244 -D_CRT_SECURE_NO_DEPRECATE=1

# For something scary:-Wp64
CXXDEBUG=-Od -MTd -DDEBUG
# Don't use "-MD", even with "-D_STATIC_CPPLIB" because it links to MSVCR71.DLL
CXXNDEBUG=-O1 -Oi -MT -DNDEBUG -GL
NAME=-Fo
LDFLAGS=-OPT:REF -LTCG -DEBUG
LDDEBUG=
LIBS=KERNEL32.lib USER32.lib GDI32.lib COMDLG32.lib COMCTL32.lib ADVAPI32.lib IMM32.lib SHELL32.LIB OLE32.LIB
NOLOGO=-nologo

!ELSE
# BORLAND

CC=bcc32
RC=brcc32 -r
LD=ilink32

CXXFLAGS=-P -tWM -w -w-prc -w-inl -RT-
CCFLAGS=-tWM -w -RT- -x- -v- -w-aus -w-sig

# Above turns off warnings for clarfying parentheses and inlines with for not expanded
CXXDEBUG=-Od -v -DDEBUG
CXXNDEBUG=-O1 -DNDEBUG
NAME=-o
LDFLAGS=-Gn -x -c
LDDEBUG=-v
LIBS=import32 cw32mt
NOLOGO=-q

!ENDIF

!IFDEF QUIET
CC=@$(CC)
CXXFLAGS=$(CXXFLAGS) $(NOLOGO)
CCFLAGS=$(CCFLAGS) $(NOLOGO)
LDFLAGS=$(LDFLAGS) $(NOLOGO)
!ENDIF

!IFDEF DEBUG
CXXFLAGS=$(CXXFLAGS) $(CXXDEBUG)
CCFLAGS=$(CCFLAGS) $(CXXDEBUG)
LDFLAGS=$(LDDEBUG) $(LDFLAGS)
!ELSE
CXXFLAGS=$(CXXFLAGS) $(CXXNDEBUG)
CCFLAGS=$(CCFLAGS) $(CXXNDEBUG)
!ENDIF

INCLUDEDIRS=-I../../scintilla/include -I../../scintilla/win32 -I../src

OBJS=\
	SciTEBase.obj \
	FilePath.obj \
	SciTEBuffers.obj \
	SciTEIO.obj \
	Exporters.obj \
	PropSetFile.obj \
	StringList.obj \
	SciTEProps.obj \
	Utf8_16.obj \
	SciTEWin.obj \
	SciTEWinBar.obj \
	SciTEWinDlg.obj \
	IFaceTable.obj \
	DirectorExtension.obj \
	MultiplexExtension.obj \
	UniqueInstance.obj \
	WinMutex.obj \
	..\..\scintilla\win32\PlatWin.obj \
	..\..\scintilla\win32\UniConversion.obj \
	..\..\scintilla\win32\WindowAccessor.obj \
	..\..\scintilla\win32\XPM.obj

#++Autogenerated -- run src/LexGen.py to regenerate
#**LEXOBJS=\\\n\(\t..\\..\\scintilla\\win32\\\*.obj \\\n\)
LEXOBJS=\
	..\..\scintilla\win32\LexAbaqus.obj \
	..\..\scintilla\win32\LexAda.obj \
	..\..\scintilla\win32\LexAPDL.obj \
	..\..\scintilla\win32\LexAsm.obj \
	..\..\scintilla\win32\LexAsn1.obj \
	..\..\scintilla\win32\LexASY.obj \
	..\..\scintilla\win32\LexAU3.obj \
	..\..\scintilla\win32\LexAVE.obj \
	..\..\scintilla\win32\LexBaan.obj \
	..\..\scintilla\win32\LexBash.obj \
	..\..\scintilla\win32\LexBasic.obj \
	..\..\scintilla\win32\LexBullant.obj \
	..\..\scintilla\win32\LexCaml.obj \
	..\..\scintilla\win32\LexCLW.obj \
	..\..\scintilla\win32\LexCmake.obj \
	..\..\scintilla\win32\LexCOBOL.obj \
	..\..\scintilla\win32\LexConf.obj \
	..\..\scintilla\win32\LexCPP.obj \
	..\..\scintilla\win32\LexCrontab.obj \
	..\..\scintilla\win32\LexCsound.obj \
	..\..\scintilla\win32\LexCSS.obj \
	..\..\scintilla\win32\LexD.obj \
	..\..\scintilla\win32\LexEiffel.obj \
	..\..\scintilla\win32\LexErlang.obj \
	..\..\scintilla\win32\LexEScript.obj \
	..\..\scintilla\win32\LexFlagship.obj \
	..\..\scintilla\win32\LexForth.obj \
	..\..\scintilla\win32\LexFortran.obj \
	..\..\scintilla\win32\LexGAP.obj \
	..\..\scintilla\win32\LexGui4Cli.obj \
	..\..\scintilla\win32\LexHaskell.obj \
	..\..\scintilla\win32\LexHTML.obj \
	..\..\scintilla\win32\LexInno.obj \
	..\..\scintilla\win32\LexKix.obj \
	..\..\scintilla\win32\LexLisp.obj \
	..\..\scintilla\win32\LexLout.obj \
	..\..\scintilla\win32\LexLua.obj \
	..\..\scintilla\win32\LexMagik.obj \
	..\..\scintilla\win32\LexMarkdown.obj \
	..\..\scintilla\win32\LexMatlab.obj \
	..\..\scintilla\win32\LexMetapost.obj \
	..\..\scintilla\win32\LexMMIXAL.obj \
	..\..\scintilla\win32\LexMPT.obj \
	..\..\scintilla\win32\LexMSSQL.obj \
	..\..\scintilla\win32\LexMySQL.obj \
	..\..\scintilla\win32\LexNimrod.obj \
	..\..\scintilla\win32\LexNsis.obj \
	..\..\scintilla\win32\LexOpal.obj \
	..\..\scintilla\win32\LexOthers.obj \
	..\..\scintilla\win32\LexPascal.obj \
	..\..\scintilla\win32\LexPB.obj \
	..\..\scintilla\win32\LexPerl.obj \
	..\..\scintilla\win32\LexPLM.obj \
	..\..\scintilla\win32\LexPOV.obj \
	..\..\scintilla\win32\LexPowerPro.obj \
	..\..\scintilla\win32\LexPowerShell.obj \
	..\..\scintilla\win32\LexProgress.obj \
	..\..\scintilla\win32\LexPS.obj \
	..\..\scintilla\win32\LexPython.obj \
	..\..\scintilla\win32\LexR.obj \
	..\..\scintilla\win32\LexRebol.obj \
	..\..\scintilla\win32\LexRuby.obj \
	..\..\scintilla\win32\LexScriptol.obj \
	..\..\scintilla\win32\LexSmalltalk.obj \
	..\..\scintilla\win32\LexSML.obj \
	..\..\scintilla\win32\LexSorcus.obj \
	..\..\scintilla\win32\LexSpecman.obj \
	..\..\scintilla\win32\LexSpice.obj \
	..\..\scintilla\win32\LexSQL.obj \
	..\..\scintilla\win32\LexTACL.obj \
	..\..\scintilla\win32\LexTADS3.obj \
	..\..\scintilla\win32\LexTAL.obj \
	..\..\scintilla\win32\LexTCL.obj \
	..\..\scintilla\win32\LexTeX.obj \
	..\..\scintilla\win32\LexVB.obj \
	..\..\scintilla\win32\LexVerilog.obj \
	..\..\scintilla\win32\LexVHDL.obj \
	..\..\scintilla\win32\LexYAML.obj \

#--Autogenerated -- end of automatically generated section

OBJSSTATIC=\
	SciTEBase.obj \
	FilePath.obj \
	SciTEBuffers.obj \
	SciTEIO.obj \
	Exporters.obj \
	PropSetFile.obj \
	StringList.obj \
	SciTEProps.obj \
	Utf8_16.obj \
	Sc1.obj \
	SciTEWinBar.obj \
	SciTEWinDlg.obj \
	IFaceTable.obj \
	DirectorExtension.obj \
	MultiplexExtension.obj \
	UniqueInstance.obj \
	WinMutex.obj \
	..\..\scintilla\win32\AutoComplete.obj \
	..\..\scintilla\win32\CallTip.obj \
	..\..\scintilla\win32\CellBuffer.obj \
	..\..\scintilla\win32\ContractionState.obj \
	..\..\scintilla\win32\CharClassify.obj \
	..\..\scintilla\win32\Decoration.obj \
	..\..\scintilla\win32\Document.obj \
	..\..\scintilla\win32\DocumentAccessor.obj \
	..\..\scintilla\win32\Editor.obj \
	..\..\scintilla\win32\ExternalLexer.obj \
	..\..\scintilla\win32\Indicator.obj \
	..\..\scintilla\win32\KeyMap.obj \
	..\..\scintilla\win32\KeyWords.obj \
	..\..\scintilla\win32\LineMarker.obj \
	..\..\scintilla\win32\PerLine.obj \
	..\..\scintilla\win32\PlatWin.obj \
	..\..\scintilla\win32\PositionCache.obj \
	..\..\scintilla\win32\PropSet.obj \
	..\..\scintilla\win32\RESearch.obj \
	..\..\scintilla\win32\RunStyles.obj \
	..\..\scintilla\win32\ScintillaBaseL.obj \
	..\..\scintilla\win32\ScintillaWinL.obj \
	..\..\scintilla\win32\Selection.obj \
	..\..\scintilla\win32\Style.obj \
	..\..\scintilla\win32\StyleContext.obj \
	..\..\scintilla\win32\UniConversion.obj \
	..\..\scintilla\win32\ViewStyle.obj \
	..\..\scintilla\win32\WindowAccessor.obj \
	..\..\scintilla\win32\XPM.obj

#++Autogenerated -- run scintilla/src/LexGen.py to regenerate
#**1:LEXPROPS=\\\n\($(DIR_BIN)\\\* \)
LEXPROPS=\
$(PROP_DIR)\abaqus.properties $(PROP_DIR)\ada.properties \
$(PROP_DIR)\asm.properties $(PROP_DIR)\asn1.properties $(PROP_DIR)\au3.properties \
$(PROP_DIR)\ave.properties $(PROP_DIR)\baan.properties \
$(PROP_DIR)\blitzbasic.properties $(PROP_DIR)\bullant.properties \
$(PROP_DIR)\caml.properties $(PROP_DIR)\cmake.properties \
$(PROP_DIR)\cobol.properties $(PROP_DIR)\conf.properties \
$(PROP_DIR)\cpp.properties $(PROP_DIR)\csound.properties \
$(PROP_DIR)\css.properties $(PROP_DIR)\d.properties $(PROP_DIR)\eiffel.properties \
$(PROP_DIR)\erlang.properties $(PROP_DIR)\escript.properties \
$(PROP_DIR)\flagship.properties $(PROP_DIR)\forth.properties \
$(PROP_DIR)\fortran.properties $(PROP_DIR)\freebasic.properties \
$(PROP_DIR)\gap.properties $(PROP_DIR)\html.properties \
$(PROP_DIR)\inno.properties $(PROP_DIR)\kix.properties \
$(PROP_DIR)\latex.properties $(PROP_DIR)\lisp.properties \
$(PROP_DIR)\lot.properties $(PROP_DIR)\lout.properties $(PROP_DIR)\lua.properties \
$(PROP_DIR)\matlab.properties $(PROP_DIR)\metapost.properties \
$(PROP_DIR)\mmixal.properties $(PROP_DIR)\nimrod.properties \
$(PROP_DIR)\nncrontab.properties $(PROP_DIR)\nsis.properties \
$(PROP_DIR)\opal.properties $(PROP_DIR)\others.properties \
$(PROP_DIR)\pascal.properties $(PROP_DIR)\perl.properties \
$(PROP_DIR)\pov.properties $(PROP_DIR)\powerpro.properties \
$(PROP_DIR)\powershell.properties $(PROP_DIR)\ps.properties \
$(PROP_DIR)\purebasic.properties $(PROP_DIR)\python.properties \
$(PROP_DIR)\r.properties $(PROP_DIR)\rebol.properties $(PROP_DIR)\ruby.properties \
$(PROP_DIR)\scriptol.properties $(PROP_DIR)\smalltalk.properties \
$(PROP_DIR)\sorcins.properties $(PROP_DIR)\specman.properties \
$(PROP_DIR)\spice.properties $(PROP_DIR)\sql.properties \
$(PROP_DIR)\tacl.properties $(PROP_DIR)\tal.properties $(PROP_DIR)\tcl.properties \
$(PROP_DIR)\tex.properties $(PROP_DIR)\vb.properties \
$(PROP_DIR)\verilog.properties $(PROP_DIR)\vhdl.properties \
$(PROP_DIR)\yaml.properties
#--Autogenerated -- end of automatically generated section

PROPS=$(DIR_BIN)\全局设置.properties $(DIR_BIN)\全局缩写.properties $(LEXPROPS)

!IFNDEF NO_LUA
LUA_CORE_OBJS = lapi.obj lcode.obj ldebug.obj ldo.obj ldump.obj lfunc.obj lgc.obj llex.obj \
                lmem.obj lobject.obj lopcodes.obj lparser.obj lstate.obj lstring.obj \
                ltable.obj ltm.obj lundump.obj lvm.obj lzio.obj

LUA_LIB_OBJS =	lauxlib.obj lbaselib.obj ldblib.obj liolib.obj lmathlib.obj ltablib.obj \
                lstrlib.obj loadlib.obj loslib.obj linit.obj

LUA_OBJS = LuaExtension.obj SingleThreadExtension.obj $(LUA_CORE_OBJS) $(LUA_LIB_OBJS)

OBJS = $(OBJS) $(LUA_OBJS)
OBJSSTATIC = $(OBJSSTATIC) $(LUA_OBJS)
INCLUDEDIRS = $(INCLUDEDIRS) -I../lua/include
!ELSE
CXXFLAGS=$(CXXFLAGS) -DNO_LUA
!ENDIF

CXXFLAGS=$(CXXFLAGS) $(INCLUDEDIRS)
CCFLAGS=$(CCFLAGS) $(INCLUDEDIRS)


ALL: $(PROG) $(PROGSTATIC) $(DLLS) $(PROPS)

clean:
	del /q $(DIR_BIN)\*.exe *.o *.obj $(DIR_BIN)\*.dll *.res *.map $(DIR_BIN)\*.exp $(DIR_BIN)\*.lib $(DIR_BIN)\*.pdb

$(DIR_BIN)\Scintilla.dll: ..\..\scintilla\bin\Scintilla.dll
	copy ..\..\scintilla\bin\Scintilla.dll $@

$(DIR_BIN)\SciLexer.dll: ..\..\scintilla\bin\SciLexer.dll
	copy ..\..\scintilla\bin\SciLexer.dll $@

$(DIR_BIN)\SciTEGlobal.properties: ..\src\SciTEGlobal.properties
	copy ..\src\SciTEGlobal.properties $@

$(DIR_BIN)\abbrev.properties: ..\src\abbrev.properties
	copy ..\src\abbrev.properties $@
	
$(DIR_BIN)\全局设置.properties: ..\src\全局设置.properties
	copy ..\src\SciTEGlobal.properties $@
	
$(DIR_BIN)\全局缩写.properties: ..\src\全局缩写.properties
	copy ..\src\abbrev.properties $@
		
#++Autogenerated -- run scintilla/src/LexGen.py to regenerate
#**1:\($(DIR_BIN)\\\*:\t..\\src\\\*\n\tcopy ..\\src\\\* $@\n\)
$(PROP_DIR)\abaqus.properties:	..\src\abaqus.properties
	copy ..\src\abaqus.properties $@
$(PROP_DIR)\ada.properties:	..\src\ada.properties
	copy ..\src\ada.properties $@
$(PROP_DIR)\asm.properties:	..\src\asm.properties
	copy ..\src\asm.properties $@
$(PROP_DIR)\asn1.properties:	..\src\asn1.properties
	copy ..\src\asn1.properties $@
$(PROP_DIR)\au3.properties:	..\src\au3.properties
	copy ..\src\au3.properties $@
$(PROP_DIR)\ave.properties:	..\src\ave.properties
	copy ..\src\ave.properties $@
$(PROP_DIR)\baan.properties:	..\src\baan.properties
	copy ..\src\baan.properties $@
$(PROP_DIR)\blitzbasic.properties:	..\src\blitzbasic.properties
	copy ..\src\blitzbasic.properties $@
$(PROP_DIR)\bullant.properties:	..\src\bullant.properties
	copy ..\src\bullant.properties $@
$(PROP_DIR)\caml.properties:	..\src\caml.properties
	copy ..\src\caml.properties $@
$(PROP_DIR)\cmake.properties:	..\src\cmake.properties
	copy ..\src\cmake.properties $@
$(PROP_DIR)\cobol.properties:	..\src\cobol.properties
	copy ..\src\cobol.properties $@
$(PROP_DIR)\conf.properties:	..\src\conf.properties
	copy ..\src\conf.properties $@
$(PROP_DIR)\cpp.properties:	..\src\cpp.properties
	copy ..\src\cpp.properties $@
$(PROP_DIR)\csound.properties:	..\src\csound.properties
	copy ..\src\csound.properties $@
$(PROP_DIR)\css.properties:	..\src\css.properties
	copy ..\src\css.properties $@
$(PROP_DIR)\d.properties:	..\src\d.properties
	copy ..\src\d.properties $@
$(PROP_DIR)\eiffel.properties:	..\src\eiffel.properties
	copy ..\src\eiffel.properties $@
$(PROP_DIR)\erlang.properties:	..\src\erlang.properties
	copy ..\src\erlang.properties $@
$(PROP_DIR)\escript.properties:	..\src\escript.properties
	copy ..\src\escript.properties $@
$(PROP_DIR)\flagship.properties:	..\src\flagship.properties
	copy ..\src\flagship.properties $@
$(PROP_DIR)\forth.properties:	..\src\forth.properties
	copy ..\src\forth.properties $@
$(PROP_DIR)\fortran.properties:	..\src\fortran.properties
	copy ..\src\fortran.properties $@
$(PROP_DIR)\freebasic.properties:	..\src\freebasic.properties
	copy ..\src\freebasic.properties $@
$(PROP_DIR)\gap.properties:	..\src\gap.properties
	copy ..\src\gap.properties $@
$(PROP_DIR)\html.properties:	..\src\html.properties
	copy ..\src\html.properties $@
$(PROP_DIR)\inno.properties:	..\src\inno.properties
	copy ..\src\inno.properties $@
$(PROP_DIR)\kix.properties:	..\src\kix.properties
	copy ..\src\kix.properties $@
$(PROP_DIR)\latex.properties:	..\src\latex.properties
	copy ..\src\latex.properties $@
$(PROP_DIR)\lisp.properties:	..\src\lisp.properties
	copy ..\src\lisp.properties $@
$(PROP_DIR)\lot.properties:	..\src\lot.properties
	copy ..\src\lot.properties $@
$(PROP_DIR)\lout.properties:	..\src\lout.properties
	copy ..\src\lout.properties $@
$(PROP_DIR)\lua.properties:	..\src\lua.properties
	copy ..\src\lua.properties $@
$(PROP_DIR)\matlab.properties:	..\src\matlab.properties
	copy ..\src\matlab.properties $@
$(PROP_DIR)\metapost.properties:	..\src\metapost.properties
	copy ..\src\metapost.properties $@
$(PROP_DIR)\mmixal.properties:	..\src\mmixal.properties
	copy ..\src\mmixal.properties $@
$(PROP_DIR)\nimrod.properties:	..\src\nimrod.properties
	copy ..\src\nimrod.properties $@
$(PROP_DIR)\nncrontab.properties:	..\src\nncrontab.properties
	copy ..\src\nncrontab.properties $@
$(PROP_DIR)\nsis.properties:	..\src\nsis.properties
	copy ..\src\nsis.properties $@
$(PROP_DIR)\opal.properties:	..\src\opal.properties
	copy ..\src\opal.properties $@
$(PROP_DIR)\others.properties:	..\src\others.properties
	copy ..\src\others.properties $@
$(PROP_DIR)\pascal.properties:	..\src\pascal.properties
	copy ..\src\pascal.properties $@
$(PROP_DIR)\perl.properties:	..\src\perl.properties
	copy ..\src\perl.properties $@
$(PROP_DIR)\pov.properties:	..\src\pov.properties
	copy ..\src\pov.properties $@
$(PROP_DIR)\powerpro.properties:	..\src\powerpro.properties
	copy ..\src\powerpro.properties $@
$(PROP_DIR)\powershell.properties:	..\src\powershell.properties
	copy ..\src\powershell.properties $@
$(PROP_DIR)\ps.properties:	..\src\ps.properties
	copy ..\src\ps.properties $@
$(PROP_DIR)\purebasic.properties:	..\src\purebasic.properties
	copy ..\src\purebasic.properties $@
$(PROP_DIR)\python.properties:	..\src\python.properties
	copy ..\src\python.properties $@
$(PROP_DIR)\r.properties:	..\src\r.properties
	copy ..\src\r.properties $@
$(PROP_DIR)\rebol.properties:	..\src\rebol.properties
	copy ..\src\rebol.properties $@
$(PROP_DIR)\ruby.properties:	..\src\ruby.properties
	copy ..\src\ruby.properties $@
$(PROP_DIR)\scriptol.properties:	..\src\scriptol.properties
	copy ..\src\scriptol.properties $@
$(PROP_DIR)\smalltalk.properties:	..\src\smalltalk.properties
	copy ..\src\smalltalk.properties $@
$(PROP_DIR)\sorcins.properties:	..\src\sorcins.properties
	copy ..\src\sorcins.properties $@
$(PROP_DIR)\specman.properties:	..\src\specman.properties
	copy ..\src\specman.properties $@
$(PROP_DIR)\spice.properties:	..\src\spice.properties
	copy ..\src\spice.properties $@
$(PROP_DIR)\sql.properties:	..\src\sql.properties
	copy ..\src\sql.properties $@
$(PROP_DIR)\tacl.properties:	..\src\tacl.properties
	copy ..\src\tacl.properties $@
$(PROP_DIR)\tal.properties:	..\src\tal.properties
	copy ..\src\tal.properties $@
$(PROP_DIR)\tcl.properties:	..\src\tcl.properties
	copy ..\src\tcl.properties $@
$(PROP_DIR)\tex.properties:	..\src\tex.properties
	copy ..\src\tex.properties $@
$(PROP_DIR)\vb.properties:	..\src\vb.properties
	copy ..\src\vb.properties $@
$(PROP_DIR)\verilog.properties:	..\src\verilog.properties
	copy ..\src\verilog.properties $@
$(PROP_DIR)\vhdl.properties:	..\src\vhdl.properties
	copy ..\src\vhdl.properties $@
$(PROP_DIR)\yaml.properties:	..\src\yaml.properties
	copy ..\src\yaml.properties $@

#--Autogenerated -- end of automatically generated section

# Normally distributed rather than built as may not have grep on all machines
# Copy all non-comment lines from all the properties files into one combined file
..\src\Embedded.properties: $(PROPS)
	grep -v -h "^[#]" $(PROPS) >..\src\Embedded.properties

# A custom rule for .obj files built by scintilla:
..\..\scintilla\win32\PlatWin.obj: 	..\..\scintilla\win32\PlatWin.cxx
	@echo You must run the Scintilla makefile to build $*.obj
	@exit 255

SciTERes.res: SciTERes.rc ..\src\SciTE.h ..\..\scintilla\win32\PlatformRes.h
	$(RC) $(INCLUDEDIRS) -fo$@ SciTERes.rc

Sc1Res.res: SciTERes.rc ..\src\SciTE.h ..\..\scintilla\win32\PlatformRes.h
	$(RC) $(INCLUDEDIRS) -dSTATIC_BUILD -fo$@ SciTERes.rc

!IF "$(VENDOR)"=="MICROSOFT"

$(PROG): $(OBJS) SciTERes.res
	$(LD) $(LDFLAGS) -OUT:$@ $** $(LIBS)

$(PROGSTATIC): $(OBJSSTATIC) $(LEXOBJS) Sc1Res.res
	$(LD) $(LDFLAGS) -OUT:$@ $** $(LIBS)

!ELSE

$(PROG): $(OBJS) SciTERes.res
	$(LD) $(LDFLAGS) -Tpe -aa @&&|
	c0w32 $(OBJS)
	$@

	$(LIBS)

	SciTERes.res
|

$(PROGSTATIC): $(OBJSSTATIC) $(LEXOBJS) Sc1Res.res
	$(LD) $(LDFLAGS) -Tpe -aa @&&|
	c0w32 $(OBJSSTATIC) $(LEXOBJS)
	$@

	$(LIBS)

	Sc1Res.res
|

!ENDIF

# Define how to build all the objects and what they depend on
# Some source files are compiled into more than one object because of different conditional compilation

{..\src}.cxx.obj:
	$(CC) $(CXXFLAGS) -c $<
{.}.cxx.obj:
	$(CC) $(CXXFLAGS) -c $<

{..\lua\src}.c.obj:
	$(CC) $(CCFLAGS) -c $<
{..\lua\src\lib}.c.obj:
	$(CC) $(CCFLAGS) -c $<

Sc1.obj: SciTEWin.cxx
	$(CC) $(CXXFLAGS) -DSTATIC_BUILD -c $(NAME)$@ SciTEWin.cxx

# Dependencies
DirectorExtension.obj: \
	DirectorExtension.cxx \
	../../scintilla/include/Platform.h \
	../../scintilla/include/PropSet.h \
	../src/SString.h \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/Accessor.h \
	../src/Extender.h \
	DirectorExtension.h \
	../src/SciTE.h \
	../src/FilePath.h \
	../src/PropSetFile.h \
	../src/Mutex.h \
	../src/JobQueue.h \
	../src/SciTEBase.h
MultiplexExtension.obj: \
	../src/MultiplexExtension.cxx \
	../src/MultiplexExtension.h \
	../src/Extender.h \
	../../scintilla/include/Scintilla.h
SingleThreadExtension.obj: \
	SingleThreadExtension.cxx \
	SingleThreadExtension.h \
	../src/Extender.h \
	../../scintilla/include/Scintilla.h
UniqueInstance.obj: \
	UniqueInstance.cxx \
	UniqueInstance.h \
	SciTEWin.h \
	../src/SciTE.h \
	../src/Extender.h \
	../src/FilePath.h \
	../src/PropSetFile.h \
	../src/SciTEBase.h \
	../src/SciTEKeys.h \
	../../scintilla/include/PropSet.h \
	../src/SString.h \
	../../scintilla/include/Accessor.h \
	../../scintilla/include/KeyWords.h \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/Platform.h
WinMutex.obj: \
	WinMutex.cxx \
	../src/Mutex.h
SciTEWin.obj: \
	SciTEWin.cxx \
	SciTEWin.h \
	../../scintilla/include/Platform.h \
	../src/SciTE.h \
	../../scintilla/include/PropSet.h \
	../src/SString.h \
	../../scintilla/include/Accessor.h \
	../../scintilla/include/KeyWords.h \
	../../scintilla/include/Scintilla.h \
	../src/Extender.h \
	../src/FilePath.h \
	../src/SciTEBase.h \
	DirectorExtension.h \
	../src/MultiplexExtension.h \
	../src/LuaExtension.h \
	../src/PropSetFile.h \
	../src/Mutex.h \
	../src/JobQueue.h \
	../src/SciTEKeys.h \
	UniqueInstance.h \
	SingleThreadExtension.h
Sc1.obj: \
	SciTEWin.cxx \
	SciTEWin.h \
	../../scintilla/include/Platform.h \
	../src/SciTE.h \
	../../scintilla/include/PropSet.h \
	../src/SString.h \
	../../scintilla/include/Accessor.h \
	../../scintilla/include/KeyWords.h \
	../../scintilla/include/Scintilla.h \
	../src/Extender.h \
	../src/FilePath.h \
	../src/SciTEBase.h \
	DirectorExtension.h \
	../src/MultiplexExtension.h \
	../src/LuaExtension.h \
	../src/PropSetFile.h \
	../src/Mutex.h \
	../src/JobQueue.h \
	../src/SciTEKeys.h \
	SingleThreadExtension.h
SciTEWinBar.obj: \
	SciTEWinBar.cxx \
	SciTEWin.h \
	../../scintilla/include/Platform.h \
	../src/SciTE.h \
	../../scintilla/include/PropSet.h \
	../src/SString.h \
	../../scintilla/include/Accessor.h \
	../../scintilla/include/KeyWords.h \
	../../scintilla/include/Scintilla.h \
	../src/Extender.h \
	../src/FilePath.h \
	../src/PropSetFile.h \
	../src/Mutex.h \
	../src/JobQueue.h \
	../src/SciTEBase.h \
	../src/SciTEKeys.h \
	UniqueInstance.h
SciTEWinDlg.obj: \
	SciTEWinDlg.cxx \
	SciTEWin.h \
	../../scintilla/include/Platform.h \
	../src/SciTE.h \
	../../scintilla/include/PropSet.h \
	../src/SString.h \
	../../scintilla/include/Accessor.h \
	../../scintilla/include/KeyWords.h \
	../../scintilla/include/Scintilla.h \
	../src/Extender.h \
	../src/FilePath.h \
	../src/PropSetFile.h \
	../src/Mutex.h \
	../src/JobQueue.h \
	../src/SciTEBase.h \
	../src/SciTEKeys.h \
	UniqueInstance.h
Exporters.obj: \
	../src/Exporters.cxx \
	../../scintilla/include/Platform.h \
	../src/SciTE.h \
	../../scintilla/include/PropSet.h \
	../src/SString.h \
	../../scintilla/include/Accessor.h \
	../../scintilla/include/WindowAccessor.h \
	../../scintilla/include/Scintilla.h \
	../src/Extender.h \
	../src/FilePath.h \
	../src/PropSetFile.h \
	../src/Mutex.h \
	../src/JobQueue.h \
	../src/SciTEBase.h
PropSetFile.obj: \
	../src/PropSetFile.cxx \
	../src/PropSetFile.h \
	../src/SString.h
SciTEBase.obj: \
	../src/SciTEBase.cxx \
	../../scintilla/include/Platform.h \
	../src/SciTE.h \
	../src/PropSetFile.h \
	../../scintilla/include/PropSet.h \
	../src/SString.h \
	../../scintilla/include/Accessor.h \
	../../scintilla/include/WindowAccessor.h \
	../../scintilla/include/KeyWords.h \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/ScintillaWidget.h \
	../../scintilla/include/SciLexer.h \
	../src/Extender.h \
	../src/FilePath.h \
	../src/Mutex.h \
	../src/JobQueue.h \
	../src/SciTEBase.h
FilePath.obj: \
	../src/FilePath.cxx \
	../../scintilla/include/Platform.h \
	../../scintilla/include/PropSet.h \
	../src/SString.h \
	../src/FilePath.h
SciTEBuffers.obj: \
	../src/SciTEBuffers.cxx \
	../../scintilla/include/Platform.h \
	../src/SciTE.h \
	../../scintilla/include/PropSet.h \
	../src/SString.h \
	../../scintilla/include/Accessor.h \
	../../scintilla/include/WindowAccessor.h \
	../../scintilla/include/KeyWords.h \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/SciLexer.h \
	../src/Extender.h \
	../src/FilePath.h \
	../src/PropSetFile.h \
	../src/Mutex.h \
	../src/JobQueue.h \
	../src/SciTEBase.h
SciTEIO.obj: \
	../src/SciTEIO.cxx \
	../../scintilla/include/Platform.h \
	../src/SciTE.h \
	../../scintilla/include/PropSet.h \
	../src/SString.h \
	../../scintilla/include/Accessor.h \
	../../scintilla/include/WindowAccessor.h \
	../../scintilla/include/Scintilla.h \
	../src/Extender.h \
	../src/Utf8_16.h \
	../src/FilePath.h \
	../src/PropSetFile.h \
	../src/Mutex.h \
	../src/JobQueue.h \
	../src/SciTEBase.h
StringList.obj: \
	../src/StringList.cxx \
	../src/StringList.h \
	../src/SString.h
SciTEProps.obj: \
	../src/SciTEProps.cxx \
	../../scintilla/include/Platform.h \
	../src/SciTE.h \
	../../scintilla/include/PropSet.h \
	../src/SString.h \
	../../scintilla/include/Accessor.h \
	../../scintilla/include/Scintilla.h \
	../../scintilla/include/SciLexer.h \
	../src/Extender.h \
	../src/FilePath.h \
	../src/PropSetFile.h \
	../src/Mutex.h \
	../src/JobQueue.h \
	../src/SciTEBase.h
Utf8_16.obj: \
	../src/Utf8_16.cxx \
	../src/Utf8_16.h

!IFNDEF NO_LUA
LuaExtension.obj: \
	../src/LuaExtension.cxx \
	../src/LuaExtension.h \
	../src/Extender.h \
	../src/SciTEKeys.h \
	../src/IFaceTable.h \
	../../scintilla/include/Platform.h \
	../src/SString.h \
	../../scintilla/include/PropSet.h \
	../../scintilla/include/Accessor.h \
	../../scintilla/include/Scintilla.h

IFaceTable.obj: \
	../src/IFaceTable.cxx \
	../src/IFaceTable.h

# Lua core dependencies are omitted; if the Lua source code
# is modified, a make clean may be necessary.
!ENDIF
