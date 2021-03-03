#if !defined(AFX_CAMVICDLG_H__29FF6C14_7BF0_11D3_BD88_00608CCC5668__INCLUDED_)
#define AFX_CAMVICDLG_H__29FF6C14_7BF0_11D3_BD88_00608CCC5668__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CamVicDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCamVicDlg dialog

class SAPCLASSGUI_CLASS CCamVicDlg : public CDialog, public SapManager
{
// Construction
public:
	CCamVicDlg(CWnd* pParent, SapAcquisition *pAcq = NULL, const char *productDir = "SAPERADIR");   // standard constructor
	SapAcquisition &GetAcquisition() { return m_Acq; }

// Dialog Data
	//{{AFX_DATA(CCamVicDlg)
	enum { IDD = IDD_SCG_CAM_VIC };
	CComboBox	m_cbResource;
	CComboBox	m_cbServer;
	CComboBox	m_cbVic;
	CComboBox	m_cbCamera;
	CListBox	m_vicListFileName;
	CListBox	m_camListFileName;
	CString	m_ServerName;
	int		m_ResourceIndex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCamVicDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL UpdateAcquisition();
	void LoadSettings();
	void SaveSettings();
	void InitServerCombo();
	void InitResourceCombo();
	void SetDirectories();
	void UpdateCurrentDir(char *newCurrentDir, BOOL bCamFile);
	void UpdateNames(BOOL bCamFile);
	void UpdateBoxAvailability();
	BOOL MakeVicAvailable();

	// Generated message map functions
	//{{AFX_MSG(CCamVicDlg)
	afx_msg void OnCamBrowse();
	afx_msg void OnVicBrowse();
	afx_msg void OnKillfocusEditCamPath();
	afx_msg void OnKillfocusEditVicPath();
	afx_msg void OnSelchangeComboCamera();
	afx_msg void OnSelchangeComboVic();
	virtual BOOL OnInitDialog();
	afx_msg void OnGenerateVic();
	virtual void OnOK();
	afx_msg void OnSelchangeComboServer();
	afx_msg void OnSelchangeComboResource();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	SapAcquisition m_Acq;		// Internal SapAcquisition object
	CWinApp *m_App;				// Pointer to application object

   CString m_ProductDir;      // Initial directory for finding CAM and VIC files

	char m_CamFile[MAX_PATH];						// Complete CAM path
	char m_VicFile[MAX_PATH];						// Complete VIC path

	char m_currentCamDir[MAX_PATH];			// Directory where to find the CCA files
	char m_currentCamFileName[MAX_PATH];	// File name (without path) of the current file selected
	int m_currentCamFileIndex;					// Index of the file name in the list current selected

	char m_currentVicDir[MAX_PATH];			// Directory where to find the CVI files
	char m_currentVicFileName[MAX_PATH];	// File name (without path) of the current file selected
	int m_currentVicFileIndex;					// Index of the file name in the list current selected

	BOOL m_camFileAvailable;
	BOOL m_vicFileAvailable;

	char m_makeVicFile[MAX_PATH];				// MakeVic utility path
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMVICDLG_H__29FF6C14_7BF0_11D3_BD88_00608CCC5668__INCLUDED_)
