#ifndef _SAPCLASSGUI_H_
#define _SAPCLASSGUI_H_

//	SapClassGui.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Resource definitions

#include "SapClassGuiRes.h"


/////////////////////////////////////////////////////////////////////////////
// Basic classes

#include "SapClassBasic.h"


/////////////////////////////////////////////////////////////////////////////
// General definitions

#if defined SAPCLASSGUI
	#define SAPCLASSGUI_VAR		__declspec(dllexport)
	#define SAPCLASSGUI_CLASS	__declspec(dllexport)
	#define SAPCLASSGUI_FUNC	__declspec(dllexport)
#else
	#define SAPCLASSGUI_VAR		__declspec(dllimport)
	#define SAPCLASSGUI_CLASS	__declspec(dllimport)
	#define SAPCLASSGUI_FUNC	__declspec(dllimport)
#endif

#ifndef _AFXDLL
   #error The project option "General | Microsoft Foundation Classes" must be "Use MFC in a shared DLL"
#endif

/////////////////////////////////////////////////////////////////////////////
// General functions

void SAPCLASSGUI_FUNC SapClassGuiInitModule();


/////////////////////////////////////////////////////////////////////////////
// Windows and dialogs header files

#include "ImageWnd.h"
#include "AbortDlg.h"
#include "AcqDlg.h"
#include "AScanDlg.h"
#include "BufDlg.h"
#include "GioInputDlg.h"
#include "GioOutputDlg.h"
#include "GioBidirectionalDlg.h"
#include "AcqConfigDlg.h"
#include "CompDlg.h"
#include "ViewDlg.h"
#include "LoadSaveDlg.h"
#include "LScanDlg.h"
#include "FlatFieldDlg.h"
#include "BayerDlg.h"

#if !COR_WIN64
#include "CabDlg.h"
#include "CamVicDlg.h"
#include "PixProDlg.h"
#endif

#endif	// _SAPCLASSGUI_H_
