#if !defined(AFX_AREASCANDLG_H__81CCAD71_6A94_11D2_8789_00608CCC5308__INCLUDED_)
#define AFX_AREASCANDLG_H__81CCAD71_6A94_11D2_8789_00608CCC5308__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// AScanDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAScanDlg dialog

class SAPCLASSGUI_CLASS CAScanDlg : public CDialog, public SapManager
{
// Construction
public:
	CAScanDlg(CWnd* pParent, SapAcquisition *pAcq);

// Dialog Data
	//{{AFX_DATA(CAScanDlg)
	enum { IDD = IDD_SCG_ACQ_AREASCAN };
	CScrollBar	m_ScrollBar_Time;
	CScrollBar	m_ScrollBar_Frame;
	int	m_frame;
	int	m_time;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAScanDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

protected:
	SapAcquisition	*m_pAcq;

	// Capabilities
	int m_camTriggerCap;
	int m_camResetCap;
	int m_timeIntegrateCap;
	int m_timeIntegrateDurationMinCap;
	int m_timeIntegrateDurationMaxCap;
	int m_camTimeIntegrateDurationMin;
	int m_camTimeIntegrateDurationMax;
	int m_frameIntegrateCap;
	int m_frameIntegrateCountMaxCap;
	int m_masterModeCap;
	int m_IntFrameTriggerCap;
	int m_IntFrameTriggerPolarityCap;
	int m_Polarity;

	void VerifyTimeIntegrateDuration( BOOL bNotify);
	void VerifyFrameIntegrateCount( void);
	void UpdateWindow( void);

	// Generated message map functions
	//{{AFX_MSG(CAScanDlg)
	afx_msg void OnKillfocusEditFrame();
	afx_msg void OnKillfocusEditTime();
	afx_msg void OnFrameIntegrate();
	afx_msg void OnTimeIntegrate();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnCamTriggerEnable();
	afx_msg void OnCamResetEnable();
	afx_msg void OnIntFrameTriggerEnable();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AREASCANDLG_H__81CCAD71_6A94_11D2_8789_00608CCC5308__INCLUDED_)
