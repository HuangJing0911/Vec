#if !defined(AFX_PIXPRODLG_H__48AC52E2_A415_11D2_88C5_00608CCC5308__INCLUDED_)
#define AFX_PIXPRODLG_H__48AC52E2_A415_11D2_88C5_00608CCC5308__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PixProDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPixProDlg dialog

class SAPCLASSGUI_CLASS CPixProDlg : public CDialog
{
// Construction
public:
	CPixProDlg(CWnd* pParent, SapPixPro *pPixPro, SapXferNode *pSrcNode = NULL);	// standard constructor
	SapPixPro &GetPixPro() { return m_PixPro; }

// Dialog Data
	//{{AFX_DATA(CPixProDlg)
	enum { IDD = IDD_SCG_PIXPRO };
	CListBox	m_listFilename;
	CComboBox	m_cbDesign;
	CComboBox	m_cbFunction;
	CComboBox	m_cbConfig;
	int m_Function;
	int m_Config;
	//}}AFX_DATA

protected:
	void FillFunctionCombo();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPixProDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void LoadSettings();
	void SaveSettings();
	void SetDirectories();
	void UpdateCurrentDir(char *newCurrentDir);
	void UpdateNames();
	void UpdateBoxAvailability();

protected:
	CWinApp *m_App;							// Pointer to application object
	SapPixPro m_PixPro;

	char m_currentDir[MAX_PATH];			// Directory where to find the CPI files
	char m_currentFileName[MAX_PATH];	// File name (without path) of the current file selected
	int m_currentFileIndex;					// Index of the file name in the list current selected

	char m_oldCurrentDir[MAX_PATH];			// Old Directory where to find the CPI files
	char m_oldCurrentFileName[MAX_PATH];	// Old File name (without path) of the current file selected
	int m_oldCurrentFileIndex;					// Old Index of the file name in the list current selected

	BOOL m_fileAvailable;				// TRUE if a file was specified

	// Generated message map functions
	//{{AFX_MSG(CPixProDlg)
	afx_msg void OnBrowse();
	virtual void OnOK();
	afx_msg void OnKillfocusEditPath();
	afx_msg void OnSelchangeCombo();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeFunction();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PIXPRODLG_H__48AC52E2_A415_11D2_88C5_00608CCC5308__INCLUDED_)
