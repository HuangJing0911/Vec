#if !defined(AFX_PIXPROWAVGDLG_H__E6DB3E64_8D70_11D4_BCB7_00A0C9081566__INCLUDED_)
#define AFX_PIXPROWAVGDLG_H__E6DB3E64_8D70_11D4_BCB7_00A0C9081566__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PixProWAvgDlg.h : header file
//

#include "design.h"

/////////////////////////////////////////////////////////////////////////////
// CPixProWAvgDlg dialog

class CPixProWAvgDlg : public CDialog
{
// Construction
public:
	CPixProWAvgDlg(CWnd* pParent, PCORPPL_FCT_PROP properties, SapPixProParams *pParams);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPixProWAvgDlg)
	enum { IDD = IDD_SCG_PIXPRO_WEIGHTED_AVG };
	CComboBox	m_cbFactor;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPixProWAvgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CORPPL_FCT_PROP m_Properties;
	SapPixProParams *m_pParams;
	int m_BankId;
	BOOL m_bRefImage;
	int m_Factor;

	// Generated message map functions
	//{{AFX_MSG(CPixProWAvgDlg)
	afx_msg void OnPixproSdramBank0();
	afx_msg void OnPixproSdramBank1();
	afx_msg void OnPixproWeightedAvg();
	afx_msg void OnPixproRefImage();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeFactor();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PIXPROWAVGDLG_H__E6DB3E64_8D70_11D4_BCB7_00A0C9081566__INCLUDED_)
