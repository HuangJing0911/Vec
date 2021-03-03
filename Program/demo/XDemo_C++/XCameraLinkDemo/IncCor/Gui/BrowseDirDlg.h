#ifndef _BROWSEDIRDLG_H_
#define _BROWSEDIRDLG_H_

// BrowseDirDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBrowseDirDlg dialog

class CBrowseDirDlg
{
public:
	CBrowseDirDlg(const char *path);
	CString GetDir() { return m_Dir; }

	// Operations
	int DoModal();

protected:
   // Only use UINT_PTR starting with Visual Studio .NET 2003, since its presence depends on the
   // Platform SDK version with Visual Studio 6, and it is only needed for 64-bit compatibility anyway.
#if defined(_MSC_VER) && _MSC_VER >= 1300
	static UINT_PTR APIENTRY BrowseHookProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
#else
	static UINT APIENTRY BrowseHookProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif

protected:
	CString m_Dir;
};

#endif	// _BROWSEDIRDLG_H_
