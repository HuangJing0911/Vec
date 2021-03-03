#if !defined(AFX_AVIFILEDLG_H__98730DF3_7CE0_11D4_BCA1_00A0C9081566__INCLUDED_)
#define AFX_AVIFILEDLG_H__98730DF3_7CE0_11D4_BCA1_00A0C9081566__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AviFileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAviFileDlg dialog

class CAviFileDlg : public CDialog
{
// Construction
public:
	CAviFileDlg(CWnd* pParent, BOOL bLoad, int firstFrame, int compressionLevel, int compressionType);   // standard constructor
	int GetFirstFrame() { return m_FirstFrame; }
	int GetCompressionLevel() { return m_CompressionLevel; }
	int GetCompressionType() { return m_CompressionType; }

// Dialog Data
	//{{AFX_DATA(CAviFileDlg)
	enum { IDD = IDD_SCG_FILEAVI_OPTIONS };
	int		m_FirstFrame;
	int		m_CompressionLevel;
	int		m_CompressionType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAviFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAviFileDlg)
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnUpdateAviEditFirstFrame();
	afx_msg void OnUpdateAviEditCompressionLevel();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
   CWinApp *m_App;	// Pointer to application object
	BOOL m_bLoad;		// TRUE to enable load options
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AVIFILEDLG_H__98730DF3_7CE0_11D4_BCA1_00A0C9081566__INCLUDED_)
