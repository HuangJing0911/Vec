#if !defined(AFX_VIEWDLG_H__56749EA3_C4ED_11D2_B2A1_00A0C91AC0FB__INCLUDED_)
#define AFX_VIEWDLG_H__56749EA3_C4ED_11D2_B2A1_00A0C91AC0FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewDlg dialog

class SAPCLASSGUI_CLASS CViewDlg : public CDialog
{
// Construction
public:
	CViewDlg( CWnd *pParent= NULL, SapView *pView = NULL);

// Dialog Data
	//{{AFX_DATA(CViewDlg)
	enum { IDD = IDD_SCG_VIEW };
	CSpinButtonCtrl	m_scaleWidthFactorSpinCtrl;
	CSpinButtonCtrl	m_scaleWidthSpinCtrl;
	CSpinButtonCtrl	m_scaleHeightSpinCtrl;
	CSliderCtrl	m_slider_range;
	int		m_Range;
	BOOL	m_bLockAspectRatio;
	DWORD	m_scaleHeight;
	DWORD	m_scaleWidth;
	float	m_scaleHeightFactor;
	float	m_scaleWidthFactor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetRange( void);
	void LoadSettings();
	void SaveSettings();

	// Generated message map functions
	//{{AFX_MSG(CViewDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdateEditViewRange();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnUpdateScaleHeight();
	afx_msg void OnUpdateScaleWidth();
	afx_msg void OnUpdateScaleWidthFactor();
	afx_msg void OnUpdateScaleHeightFactor();
	afx_msg void OnDeltaposSpinScaleHeight(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinScaleHeightFactor(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinScaleWidth(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinScaleWidthFactor(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLockAspectRatio();
	afx_msg void OnScalingNone();
	afx_msg void OnScalingFitToWindow();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CWinApp *m_App;				// Pointer to application object

	int m_RangeMin;
	int m_RangeMax;
	int m_RangeInit;
	SapView *m_pView;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWDLG_H__56749EA3_C4ED_11D2_B2A1_00A0C91AC0FB__INCLUDED_)
