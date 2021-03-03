# Microsoft Developer Studio Project File - Name="SapClassGui" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SapClassGui - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SapClassGui.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SapClassGui.mak" CFG="SapClassGui - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SapClassGui - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SapClassGui - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SapClassGui - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\Basic" /I "..\..\Include" /I "..\..\externals\CorLib" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "SAPCLASSGUI" /YX"SapClassGui.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ..\..\Lib\Win32\Corapi.lib ..\..\Lib\Win32\SapClassBasic.lib /nologo /base:"0x67d80000" /subsystem:windows /dll /machine:I386 /out:"Release/SapClassGui74.dll" /implib:"Release/SapClassGui.lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
TargetPath=.\Release\SapClassGui74.dll
SOURCE="$(InputPath)"
PostBuild_Desc=Updating System32 directory
PostBuild_Cmds=copy "$(TargetPath)" %SystemRoot%\system32	copy .\Release\SapClassGui.lib ..\..\Lib	copy .\Release\SapClassGui.lib ..\..\Lib\Win32\VS6
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SapClassGui - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SapClassGui___Win32_Debug"
# PROP BASE Intermediate_Dir "SapClassGui___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\Basic" /I "..\..\Include" /I "..\..\externals\CorLib" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "SAPCLASSGUI" /YX"SapClassGui.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\..\Lib\Win32\Corapi.lib ..\..\Lib\Win32\SapClassBasic.lib /nologo /base:"0x67d80000" /subsystem:windows /dll /debug /machine:I386 /out:"Debug/SapClassGui74D.dll" /implib:"Debug/SapClassGuiD.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
TargetPath=.\Debug\SapClassGui74D.dll
SOURCE="$(InputPath)"
PostBuild_Desc=Updating System32 directory
PostBuild_Cmds=copy "$(TargetPath)" %SystemRoot%\system32	copy .\Debug\SapClassGuiD.lib ..\..\Lib	copy .\Debug\SapClassGuiD.lib ..\..\Lib\Win32\VS6
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "SapClassGui - Win32 Release"
# Name "SapClassGui - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AbortDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AcqConfigDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AcqDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AScanDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AviFileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BayerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BrowseDirDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BufDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CabDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CamVicDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CompDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FlatFieldDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GioInputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GioOutputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\JpegFileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadSaveDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LScanDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PixProAbsSubDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PixProDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PixProIntScaleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PixProRefImageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PixProUflowSubDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PixProWAvgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RawFileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SapClassGui.cpp
# End Source File
# Begin Source File

SOURCE=.\SapClassGui.rc
# End Source File
# Begin Source File

SOURCE=.\TiffFileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AbortDlg.h
# End Source File
# Begin Source File

SOURCE=.\AcqConfigDlg.h
# End Source File
# Begin Source File

SOURCE=.\AcqDlg.h
# End Source File
# Begin Source File

SOURCE=.\AScanDlg.h
# End Source File
# Begin Source File

SOURCE=.\AviFileDlg.h
# End Source File
# Begin Source File

SOURCE=.\BayerDlg.h
# End Source File
# Begin Source File

SOURCE=.\BrowseDirDlg.h
# End Source File
# Begin Source File

SOURCE=.\BufDlg.h
# End Source File
# Begin Source File

SOURCE=.\CabDlg.h
# End Source File
# Begin Source File

SOURCE=.\CamVicDlg.h
# End Source File
# Begin Source File

SOURCE=.\CompDlg.h
# End Source File
# Begin Source File

SOURCE=.\corenv.h
# End Source File
# Begin Source File

SOURCE=.\FlatFieldDlg.h
# End Source File
# Begin Source File

SOURCE=.\GioInputDlg.h
# End Source File
# Begin Source File

SOURCE=.\GioOutputDlg.h
# End Source File
# Begin Source File

SOURCE=.\ImageWnd.h
# End Source File
# Begin Source File

SOURCE=.\JpegFileDlg.h
# End Source File
# Begin Source File

SOURCE=.\LoadSaveDlg.h
# End Source File
# Begin Source File

SOURCE=.\LScanDlg.h
# End Source File
# Begin Source File

SOURCE=.\PixProAbsSubDlg.h
# End Source File
# Begin Source File

SOURCE=.\PixProDlg.h
# End Source File
# Begin Source File

SOURCE=.\PixProIntScaleDlg.h
# End Source File
# Begin Source File

SOURCE=.\PixProRefImageDlg.h
# End Source File
# Begin Source File

SOURCE=.\PixProUflowSubDlg.h
# End Source File
# Begin Source File

SOURCE=.\PixProWAvgDlg.h
# End Source File
# Begin Source File

SOURCE=.\RawFileDlg.h
# End Source File
# Begin Source File

SOURCE=.\SapClassGui.h
# End Source File
# Begin Source File

SOURCE=.\SapClassGuiRes.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TiffFileDlg.h
# End Source File
# Begin Source File

SOURCE=.\ViewDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Align_BG_GR.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Align_GB_RG.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Align_GR_BG.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Align_RG_GB.bmp
# End Source File
# Begin Source File

SOURCE=.\res\green_flag.bmp
# End Source File
# Begin Source File

SOURCE=.\res\red_flag.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SapClassGui.rc2
# End Source File
# End Group
# Begin Group "Header Files - SapClassBasic"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Basic\corenv.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapAcqDevice.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapAcqToBufThruCab.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapAcquisition.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapBayer.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapBuffer.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapBufferRemote.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapBufferRoi.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapBufferWithTrash.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapBufferWithTrashAndRoi.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapCab.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapClassBasic.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapClassBasicDef.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapClassBasicObsolete.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapClassBasicRes.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapCommBoard.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapCommHost.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapCommunication.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapCounter.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapData.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapDisplay.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapFeature.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapFlatField.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapGio.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapGraphic.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapLut.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapManager.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapMultiAcqToBufThruCab.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapPerformance.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapPixPro.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapProcessing.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapRTPro.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapRTProDesign.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapRTProDesignOnBrdBuffers.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapTransfer.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapTransferEx.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapView.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapXferMultiNode.h
# End Source File
# Begin Source File

SOURCE=..\Basic\SapXferNode.h
# End Source File
# End Group
# End Target
# End Project
