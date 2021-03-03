#if !defined(AFX_TIFFFILEDLG_H__B84E0B19_E2AF_4C9D_AB8E_43738939D5F8__INCLUDED_)
#define AFX_TIFFFILEDLG_H__B84E0B19_E2AF_4C9D_AB8E_43738939D5F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TiffFileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTiffFileDlg dialog

class CTiffFileDlg : public CDialog
{
// Construction
public:
	CTiffFileDlg(CWnd* pParent, int compressionLevel, int compressionType);   // standard constructor
	int GetCompressionLevel() { return m_CompressionLevel; }
	int GetCompressionType() { return m_CompressionType; }

// Dialog Data
	//{{AFX_DATA(CTiffFileDlg)
	enum { IDD = IDD_SCG_FILETIFF_OPTIONS };
	int		m_CompressionLevel;
	int		m_CompressionType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTiffFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTiffFileDlg)
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnUpdateTiffEditCompressionLevel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
   CWinApp *m_App;	// Pointer to application object
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIFFFILEDLG_H__B84E0B19_E2AF_4C9D_AB8E_43738939D5F8__INCLUDED_)
