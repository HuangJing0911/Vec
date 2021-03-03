#if !defined(AFX_JPEGFILEDLG_H__68CEC0E3_4C34_11D4_BDD9_00A0C932BC01__INCLUDED_)
#define AFX_JPEGFILEDLG_H__68CEC0E3_4C34_11D4_BDD9_00A0C932BC01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JpegFileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJpegFileDlg dialog

class CJpegFileDlg : public CDialog
{
// Construction
public:
	CJpegFileDlg(CWnd* pParent, int quality);   // standard constructor
   int GetJpegFileQuality() { return m_FileJpegQuality; }

// Dialog Data
	//{{AFX_DATA(CJpegFileDlg)
	enum { IDD = IDD_SCG_FILEJPG_OPTIONS };
	UINT	m_FileJpegQuality;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJpegFileDlg)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

	// Generated message map functions
	//{{AFX_MSG(CJpegFileDlg)
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnOK();
	afx_msg void OnUpdateJpgEditQuality();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CJpegFileDlg)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

protected:
   CWinApp *m_App;				// Pointer to application object
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JPEGFILEDLG_H__68CEC0E3_4C34_11D4_BDD9_00A0C932BC01__INCLUDED_)
