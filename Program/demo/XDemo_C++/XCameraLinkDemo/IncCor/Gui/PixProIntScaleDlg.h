#if !defined(AFX_PIXPROINTSCALEDLG_H__E6DB3E65_8D70_11D4_BCB7_00A0C9081566__INCLUDED_)
#define AFX_PIXPROINTSCALEDLG_H__E6DB3E65_8D70_11D4_BCB7_00A0C9081566__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PixProIntScaleDlg.h : header file
//

#include "design.h"

/////////////////////////////////////////////////////////////////////////////
// CPixProIntScaleDlg dialog

class CPixProIntScaleDlg : public CDialog
{
// Construction
public:
	CPixProIntScaleDlg(CWnd* pParent, PCORPPL_FCT_PROP properties, SapPixProParams *pParams);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPixProIntScaleDlg)
	enum { IDD = IDD_SCG_PIXPRO_INTEGRATE_SCALE };
	CComboBox	m_cbCount;
	int			m_Factor;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPixProIntScaleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CORPPL_FCT_PROP m_Properties;
	SapPixProParams *m_pParams;
	int m_BankId;
	int m_Count;

	// Generated message map functions
	//{{AFX_MSG(CPixProIntScaleDlg)
	afx_msg void OnPixproSdramBank0();
	afx_msg void OnPixproSdramBank1();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCount();
	afx_msg void OnSelchangeFactor();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PIXPROINTSCALEDLG_H__E6DB3E65_8D70_11D4_BCB7_00A0C9081566__INCLUDED_)
