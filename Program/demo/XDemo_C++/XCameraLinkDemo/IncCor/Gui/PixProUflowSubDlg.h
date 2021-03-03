#if !defined(AFX_PIXPROUFLOWSUBDLG_H__E6DB3E62_8D70_11D4_BCB7_00A0C9081566__INCLUDED_)
#define AFX_PIXPROUFLOWSUBDLG_H__E6DB3E62_8D70_11D4_BCB7_00A0C9081566__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PixProUflowSubDlg.h : header file
//

#include "design.h"

/////////////////////////////////////////////////////////////////////////////
// CPixProUflowSubDlg dialog

class CPixProUflowSubDlg : public CDialog
{
// Construction
public:
	CPixProUflowSubDlg(CWnd* pParent, PCORPPL_FCT_PROP properties, SapPixProParams *pParams);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPixProUflowSubDlg)
	enum { IDD = IDD_SCG_PIXPRO_UFLOW_SUB };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPixProUflowSubDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CORPPL_FCT_PROP m_Properties;
	SapPixProParams *m_pParams;
	int m_BankId;
	BOOL m_bRefImage;

	// Generated message map functions
	//{{AFX_MSG(CPixProUflowSubDlg)
	afx_msg void OnPixproSdramBank0();
	afx_msg void OnPixproSdramBank1();
	afx_msg void OnPixproUflowSub();
	afx_msg void OnPixproRefImage();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PIXPROUFLOWSUBDLG_H__E6DB3E62_8D70_11D4_BCB7_00A0C9081566__INCLUDED_)
