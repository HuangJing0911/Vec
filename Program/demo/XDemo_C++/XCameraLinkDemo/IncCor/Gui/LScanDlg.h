#if !defined(AFX_LINESCANDLG_H__81CCAD72_6A94_11D2_8789_00608CCC5308__INCLUDED_)
#define AFX_LINESCANDLG_H__81CCAD72_6A94_11D2_8789_00608CCC5308__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// LScanDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLScanDlg dialog

class SAPCLASSGUI_CLASS CLScanDlg : public CDialog, public SapManager
{
// Construction
public:
	CLScanDlg(CWnd* pParent, SapAcquisition *pAcq);


// Dialog Data
	//{{AFX_DATA(CLScanDlg)
	enum { IDD = IDD_SCG_ACQ_LINESCAN };
	CScrollBar	m_intLineTrigger_hz;
	CScrollBar	m_ScrollBar_Pixels;
	int	m_pixels;
	BOOL	m_extFrameTriggerEnable;
	BOOL	m_extLineTriggerEnable;
	BOOL	m_lineTriggerEnable;
	BOOL	m_lineIntegrateEnable;
	int	m_IntLineTrigger;
	BOOL	m_intLineTriggerEnable;
	int	m_linescanDirection;
	BOOL	m_shaftEncoderEnable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLScanDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

protected:
	SapAcquisition		*m_pAcq;

	// Capabilities
	int	m_lineTriggerCap;
	int	m_lineIntegrateCap;
	int	m_lineIntegrateDurationMinCap;
	int	m_lineIntegrateDurationMaxCap;
	int	m_extLineTriggerCap;
	int	m_extLineTriggerDetectionCap;
	int	m_extLineTriggerLevelCap;
	int	m_extFrameTriggerCap;
	int	m_extFrameTriggerDetectionCap;
	int	m_extFrameTriggerLevelCap;
	int	m_intlineTriggerCap;
	int   m_linescanDirectionCap;
	int	m_shaftEncoderCap;

	void   VerifyLineIntegrateDuration( void);
	void   UpdateWindow( void);


	// Generated message map functions
	//{{AFX_MSG(CLScanDlg)
	afx_msg void OnKillfocusEditLineIntegration();
	afx_msg void OnExternalFrameTriggerEnable();
	afx_msg void OnExternalLineTriggerEnable();
	afx_msg void OnLineIntegrate();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnInternalLineTriggerEnable();
	afx_msg void OnLineIntegrateEnable();
	afx_msg void OnShaftEncoderEnable();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINESCANDLG_H__81CCAD72_6A94_11D2_8789_00608CCC5308__INCLUDED_)
