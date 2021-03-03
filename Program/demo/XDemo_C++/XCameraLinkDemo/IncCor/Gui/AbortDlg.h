#if !defined(AFX_ABORTDLG_H__06AEB441_8670_11D2_8819_00608CCC5308__INCLUDED_)
#define AFX_ABORTDLG_H__06AEB441_8670_11D2_8819_00608CCC5308__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// AbortDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAbortDlg dialog

class SAPCLASSGUI_CLASS CAbortDlg : public CDialog
{
// Construction
public:
	CAbortDlg(CWnd* pParent, SapTransfer *pXfer, int timeout=1000);  

protected:
	SapTransfer *m_pXfer;
	DWORD m_time0;
	DWORD m_timeout;
	CRect m_rect;
	BOOL m_bShow;

// Dialog Data
	//{{AFX_DATA(CAbortDlg)
	enum { IDD = IDD_SCG_ABORT_GRAB };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAbortDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAbortDlg)
	afx_msg void OnAbort();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABORTDLG_H__06AEB441_8670_11D2_8819_00608CCC5308__INCLUDED_)
