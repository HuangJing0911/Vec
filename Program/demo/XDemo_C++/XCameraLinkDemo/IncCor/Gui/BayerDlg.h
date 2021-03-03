// BayerDlg.h : header file
//

#if !defined(AFX_BAYERDLG_H__32005EA1_2AF6_46F7_8F9E_57815D5457E7__INCLUDED_)
#define AFX_BAYERDLG_H__32005EA1_2AF6_46F7_8F9E_57815D5457E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CBayerOptionsDlg dialog

class SAPCLASSGUI_CLASS CBayerOptionsDlg : public CDialog, public SapManager
{
public:
   // Number of possible values for Bayer parameters
   enum LogTypes
   {
      NumAlignValues  = 4,
      NumMethodValues = 6,
   };

// Construction
public:
	CBayerOptionsDlg(CWnd* pParent, SapBayer *pBayer, SapTransfer *pXfer= NULL, CImageWnd *pImageWnd= NULL, SapProcessing* m_pPro= NULL);
   virtual ~CBayerOptionsDlg();

   void UpdateInterface();

// Dialog Data
	//{{AFX_DATA(CBayerOptionsDlg)
	enum { IDD = IDD_SCG_BAYER_OPTIONS };
	float	m_BlueGain;
	float	m_Gamma;
	float	m_GreenGain;
	float	m_RedGain;
	BOOL	m_GammaEnabled;
	//}}AFX_DATA

	SapBayer::Align  m_Align;
	SapBayer::Method m_Method;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBayerOptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBayerOptionsDlg)
	afx_msg void OnChange();
	afx_msg void OnAutoWhiteBalance();
	virtual BOOL OnInitDialog();
	afx_msg void OnApply();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
	afx_msg void OnGammaEnable();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void OnChangeRange(UINT id);
	void OnChangeConversion(UINT id);
	void UpdateView();

private:
	SapBayer*		m_pBayer;
	SapTransfer*	m_pXfer;
	SapProcessing*	m_pPro;
	CImageWnd*		m_pImageWnd;

	static SapBayer::Align  m_AlignValues[NumAlignValues];
	static SapBayer::Method m_MethodValues[NumMethodValues];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BAYERDLG_H__32005EA1_2AF6_46F7_8F9E_57815D5457E7__INCLUDED_)
