#if !defined(AFX_ACQCONFIGDLG_H__1050BCF7_DF38_42E7_9A4E_DFDCABD52116__INCLUDED_)
#define AFX_ACQCONFIGDLG_H__1050BCF7_DF38_42E7_9A4E_DFDCABD52116__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AcqConfigDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAcqConfigDlg dialog

class SAPCLASSGUI_CLASS CAcqConfigDlg : public CDialog, public SapManager
{
// Enumerations
public:
   enum ServerCategory
   {
      ServerAll,
      ServerAcq,
      ServerAcqDevice
   };

// Construction
public:
	CAcqConfigDlg(CWnd* pParent, ServerCategory serverCategory = ServerAcq);
	CAcqConfigDlg(CWnd* pParent, SapLocation loc, const char* configFile, ServerCategory serverCategory = ServerAcq);

   SapLocation GetLocation();
   char* GetConfigFile() { return m_ConfigFileEnable ? m_ConfigFile : NULL; }
   ServerCategory GetServerCategory() { return m_ServerCategory; }

   // Obsolete functions
	CAcqConfigDlg::CAcqConfigDlg(CWnd* pParent, SapAcquisition *pAcq, const char *productDir= NULL); // Replaced by CAcqConfigDlg(CWnd*, SapLocation, const char*)
	SapAcquisition &GetAcquisition() { return m_Acq; }    // Replaced by GetLocation and GetConfigFile

// Dialog Data
	//{{AFX_DATA(CAcqConfigDlg)
	enum { IDD = IDD_SCG_ACQ_CONFIG };
		// NOTE: the ClassWizard will add data members here
	CComboBox	m_cbResource;
	CComboBox	m_cbServer;
	CComboBox	m_cbConfig;
	CListBox	m_configListFileName;
	CString	m_ServerName;
	int		m_ResourceIndex;
	BOOL		m_ConfigFileEnable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAcqConfigDlg)
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
	void UpdateCurrentDir(char *newCurrentDir);
	void UpdateNames();
	void UpdateBoxAvailability();

	// Generated message map functions
	//{{AFX_MSG(CAcqConfigDlg)
	afx_msg void OnConfigBrowse();
	afx_msg void OnKillfocusEditConfigPath();
	afx_msg void OnSelchangeComboConfig();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeComboServer();
	afx_msg void OnSelchangeComboResource();
   afx_msg void OnBnClickedConfigFileCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	SapAcquisition m_Acq;		// Internal SapAcquisition object
	CWinApp *m_App;				// Pointer to application object

   CString m_ProductDir;      // Initial directory for finding CAM and VIC files

	char m_ConfigFile[MAX_PATH];					// Complete config file path
   ServerCategory m_ServerCategory;          // Category of servers (All, Acq or acqDevice)

	char m_currentConfigDir[MAX_PATH];			// Directory where to find the config files
	char m_currentConfigFileName[MAX_PATH];	// File name (without path) of the current config file selected
	int  m_currentConfigFileIndex;				// Index of the file name in the list current selected

	BOOL m_configFileAvailable;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACQCONFIGDLG_H__1050BCF7_DF38_42E7_9A4E_DFDCABD52116__INCLUDED_)
