# Microsoft Developer Studio Project File - Name="PEDIT_TEST" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=PEDIT_TEST - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PEDIT_TEST.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PEDIT_TEST.mak" CFG="PEDIT_TEST - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PEDIT_TEST - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "PEDIT_TEST - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PEDIT_TEST - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\wxWidgets\contrib\src\stc\scintilla\include\\" /I "..\wxWidgets\contrib\src\stc\scintilla\src\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  comctl32.lib Imm32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "PEDIT_TEST - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\wxWidgets\contrib\src\stc\scintilla\include\\" /I "..\wxWidgets\contrib\src\stc\scintilla\src\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  comctl32.lib Imm32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "PEDIT_TEST - Win32 Release"
# Name "PEDIT_TEST - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "scintilla"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\AutoComplete.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\CallTip.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\CellBuffer.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\CharClassify.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\ContractionState.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\Document.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\DocumentAccessor.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\Editor.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\ExternalLexer.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\Indicator.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\KeyMap.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\KeyWords.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexAda.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexAPDL.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexAsm.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexAsn1.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexAU3.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexAVE.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexBaan.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexBash.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexBasic.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexBullant.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexCaml.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexCLW.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexConf.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexCPP.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexCrontab.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexCsound.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexCSS.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexEiffel.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexErlang.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexEScript.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexFlagship.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexForth.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexFortran.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexGui4Cli.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexHaskell.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexHTML.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexInno.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexKix.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexLisp.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexLout.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexLua.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexMatlab.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexMetapost.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexMMIXAL.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexMPT.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexMSSQL.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexNsis.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexOpal.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexOthers.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexPascal.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexPB.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexPerl.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexPOV.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexPS.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexPython.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexRebol.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexRuby.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexScriptol.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexSmalltalk.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexSpecman.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexSpice.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexSQL.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexTADS3.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexTCL.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexTeX.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexVB.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexVerilog.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexVHDL.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LexYAML.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\LineMarker.cxx
# End Source File
# Begin Source File

SOURCE=.\include\PlatWin.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\PropSet.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\RESearch.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\ScintillaBase.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\Style.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\StyleContext.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\UniConversion.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\ViewStyle.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\WindowAccessor.cxx
# End Source File
# Begin Source File

SOURCE=..\wxWidgets\contrib\src\stc\scintilla\src\XPM.cxx
# End Source File
# End Group
# Begin Source File

SOURCE=.\anchor_system.c
# End Source File
# Begin Source File

SOURCE=.\debug_print.c
# End Source File
# Begin Source File

SOURCE=.\ini_file.c
# End Source File
# Begin Source File

SOURCE=.\pedit_test.c
# End Source File
# Begin Source File

SOURCE=.\include\ScintillaWin.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\pedit.rc
# End Source File
# End Group
# End Target
# End Project
