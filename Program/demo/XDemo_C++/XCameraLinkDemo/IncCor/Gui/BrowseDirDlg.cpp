// BrowseDirDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SapClassBasic.h"
#include "SapClassGui.h"
#include "BrowseDirDlg.h"
#include "dlgs.h"

/////////////////////////////////////////////////////////////////////////////
// CBrowseDirDlg

CBrowseDirDlg::CBrowseDirDlg(const char *dir)
{
	m_Dir = dir;
}

int CBrowseDirDlg::DoModal()
{
	char ftitle[MAX_PATH] = "";
	char title[MAX_PATH] = "Browse for Directory";
	char fname[MAX_PATH] = "*.*";
	char dir[MAX_PATH];

   CorStrncpy(dir, m_Dir, sizeof(dir));

	// Fill-in structure
	OPENFILENAME of;
	of.lStructSize = sizeof( OPENFILENAME);
	of.hwndOwner = GetFocus();
	of.lpstrFilter = NULL;
	of.lpstrCustomFilter = NULL;
	of.nMaxCustFilter = 0;
	of.nFilterIndex = 0;
	of.lpstrFile = fname;
	of.nMaxFile = MAX_PATH;
	of.lpstrFileTitle = ftitle;
	of.lpstrFileTitle = NULL;
	of.nMaxFileTitle = 0;
	of.lpstrInitialDir = dir;
	of.lpstrTitle = title;
	of.Flags = OFN_NONETWORKBUTTON|OFN_ENABLEHOOK|OFN_NOCHANGEDIR|OFN_ENABLETEMPLATE|OFN_PATHMUSTEXIST|OFN_LONGNAMES;
	of.nFileOffset = 0;
	of.nFileExtension = 0;
	of.lpstrDefExt = NULL;
	of.lCustData = 0;
	of.lpfnHook = BrowseHookProc;
	of.lpTemplateName = MAKEINTRESOURCE(IDD_SCG_BROWSE_DIRECTORY);
	of.hInstance = AfxFindResourceHandle(of.lpTemplateName, RT_DIALOG);

	// Execute dialog
	if (GetSaveFileName(&of))
	{
		char szDrive[_MAX_DRIVE];
		char szDir[_MAX_DIR];

      CorSplitpath(fname, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, NULL, 0, NULL, 0);

		m_Dir = CString(szDrive) + CString(szDir) + '\0';
		return IDOK;
	}

	return IDCANCEL;
}

//
//
//Routine Description:
//
//    Hook procedure for directory browse common file dialog.  This hook
//    procedure is required to put the window in the foreground and set 
//		the edit so that the common file dialog dll thinks the user entered 
//	 a value.
//
// Arguments:
//
//    hwnd       - window handle to the dialog box
//    message    - message number
//    wParam     - first message parameter
//    lParam     - second message parameter
//
// Return Value:
//
//    TRUE       - did not process the message
//    FALSE      - did process the message
//

// Only use UINT_PTR starting with Visual Studio .NET 2003, since its presence depends on the
// Platform SDK version with Visual Studio 6, and it is only needed for 64-bit compatibility anyway.
#if defined(_MSC_VER) && _MSC_VER >= 1300
UINT_PTR APIENTRY CBrowseDirDlg::BrowseHookProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) 
#else
UINT APIENTRY CBrowseDirDlg::BrowseHookProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) 
#endif
{
	if( message == WM_INITDIALOG)
	{
		::SetForegroundWindow( hwnd);
	}
	else if( message == WM_PAINT)
	{
		::SetDlgItemText( hwnd, edt1, "filename.txt");
	}

	return 0;
}

