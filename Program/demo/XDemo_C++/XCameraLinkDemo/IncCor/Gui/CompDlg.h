#if !defined(AFX_COMPDLG_H__175F28B1_1CB3_11D2_9630_00A0C9081566__INCLUDED_)
#define AFX_COMPDLG_H__175F28B1_1CB3_11D2_9630_00A0C9081566__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CompDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCompDlg dialog

class SAPCLASSGUI_CLASS CCompDlg : public CDialog, public SapManager
{
// Construction
public:
	CCompDlg(CWnd* pParent, SapAcquisition *pAcq, SapTransfer *pXfer = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCompDlg)
	enum { IDD = IDD_SCG_COMPOSITE };
	CSliderCtrl	m_slider_sharpness;
	CSliderCtrl	m_slider_saturation;
	CSliderCtrl	m_slider_hue;
	CSliderCtrl	m_slider_contrast;
	CSliderCtrl	m_slider_brightness;
	long	m_brightness;
	long	m_hue;
	int	m_contrast;
	int	m_saturation;
	long	m_sharpness;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCompDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

protected:
	SapAcquisition *m_pAcq;
	SapTransfer *m_pXfer;
	int	m_curPlane;
	int	m_brightnessPrm[ 4];
	int	m_contrastPrm[ 4];
	int	m_huePrm;
	int	m_saturationPrm;
   int   m_sharpnessPrm;

	// Capabilities
	int m_brightnessMinCap[ 4];
	int m_brightnessMaxCap[ 4];
	int m_contrastMinCap[ 4];
	int m_contrastMaxCap[ 4];
	int m_hueMinCap;
	int m_hueMaxCap;
	int m_saturationMinCap;
	int m_saturationMaxCap;
	int m_sharpnessMinCap;
	int m_sharpnessMaxCap;

	int	m_contrastCap;
	int	m_contrastColorCap;
	int	m_brightnessCap;
	int	m_brightnessColorCap;
	int	m_hueCap;
	int	m_saturationCap;
	int	m_sharpnessCap;

	int m_brightnessValues[ 4];
	int m_brightnessInit[ 4];
	int m_contrastValues[ 4];
	int m_contrastInit[ 4];
	int m_hueInit;
	int m_saturationInit;
	int m_sharpnessInit;

	void	SetBrightness( void);
	void	SetContrast( void);
	void	SetHue( void);
	void	SetSaturation( void);
	void	SetSharpness( void);
	void	Snap( void);

	void	UpdateWindow( void);

	// Generated message map functions
	//{{AFX_MSG(CCompDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRed();
	afx_msg void OnGreen();
	afx_msg void OnBlue();
	virtual void OnCancel();
	afx_msg void OnKillfocusEditContrast();
	afx_msg void OnKillfocusEditBrightness();
	afx_msg void OnKillfocusEditSaturation();
	afx_msg void OnKillfocusEditHue();
	afx_msg void OnDefault();
	afx_msg void OnKillfocusEditSharpness();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMPDLG_H__175F28B1_1CB3_11D2_9630_00A0C9081566__INCLUDED_)
