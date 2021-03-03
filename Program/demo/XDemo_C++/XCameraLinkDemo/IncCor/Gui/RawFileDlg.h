#if !defined(AFX_RAWFILEDLG_H__DFF20671_F0EA_11D2_AE29_00A0C932BC01__INCLUDED_)
#define AFX_RAWFILEDLG_H__DFF20671_F0EA_11D2_AE29_00A0C932BC01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RawFileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRawFileDlg dialog

class CRawFileDlg : public CDialog
{
// Construction
public:
	CRawFileDlg(CWnd* pParent, int width, int height, int offset, SapFormat format = 0);
	int GetRawFileWidth()		{ return m_FileRawWidth; }
	int GetRawFileHeight()		{ return m_FileRawHeight; }
	int GetRawFileOffset()		{ return m_FileRawOffset; }
   SapFormat GetRawFileFormat() {return m_FileRawFormat; }

// Dialog Data
	//{{AFX_DATA(CRawFileDlg)
	enum { IDD = IDD_SCG_FILERAW_SIZE };
	UINT	m_FileRawHeight;
	UINT	m_FileRawWidth;
	UINT	m_FileRawOffset;
   SapFormat m_FileRawFormat;
   CComboBox	m_cbFormat;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRawFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

	// Generated message map functions
	//{{AFX_MSG(CRawFileDlg)
   virtual BOOL OnInitDialog();
   afx_msg void OnSelchangeComboFormat();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
   CWinApp *m_App;				// Pointer to application object
   static SapFormatInfo m_FormatInfo[];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RAWFILEDLG_H__DFF20671_F0EA_11D2_AE29_00A0C932BC01__INCLUDED_)
