#if !defined(AFX_ACQDLG_H__766CBD31_F0D0_11D1_AF76_00A0C91AC0FB__INCLUDED_)
#define AFX_ACQDLG_H__766CBD31_F0D0_11D1_AF76_00A0C91AC0FB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// AcqDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAcqDlg dialog

class SAPCLASSGUI_CLASS CAcqDlg : public CDialog, public SapManager
{
// Construction
public:
	CAcqDlg(CWnd* pParent, SapAcquisition *pAcq);

// Dialog Data
	//{{AFX_DATA(CAcqDlg)
	enum { IDD = IDD_SCG_ACQUISITION };
	int m_camSel;
	BOOL	m_extTrigEnable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAcqDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

protected:
	SapAcquisition		*m_pAcq;				// Acquisition object

	// Capabilities
	int		m_extTrigCap;				// External trigger 
	int		m_extTrigDetectionCap;	// External trigger detection
	int		m_bitOrderingCap;			// Bit Ordering
	int		m_camSelMax;				// Maximum number of camera selectors

	void   UpdateWindow( void);

	// Generated message map functions
	
	//{{AFX_MSG(CAcqDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnExternalTriggerEnable();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACQDLG_H__766CBD31_F0D0_11D1_AF76_00A0C91AC0FB__INCLUDED_)

