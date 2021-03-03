#if !defined(AFX_PIXPROREFIMAGEDLG_H__6956FFF3_88B1_11D4_BCB1_00A0C9081566__INCLUDED_)
#define AFX_PIXPROREFIMAGEDLG_H__6956FFF3_88B1_11D4_BCB1_00A0C9081566__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PixProRefImageDlg.h : header file
//

#include "design.h"

/////////////////////////////////////////////////////////////////////////////
// CPixProRefImageDlg dialog

class CPixProRefImageDlg : public CDialog
{
// Construction
public:
	CPixProRefImageDlg(CWnd* pParent, PCORPPL_FCT_PROP properties, SapPixProParams *pParams);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPixProRefImageDlg)
	enum { IDD = IDD_SCG_PIXPRO_REF_IMAGE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPixProRefImageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CORPPL_FCT_PROP m_Properties;
	SapPixProParams *m_pParams;
	int m_BankId;

	// Generated message map functions
	//{{AFX_MSG(CPixProRefImageDlg)
	afx_msg void OnPixproSdramBank0();
	afx_msg void OnPixproSdramBank1();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PIXPROREFIMAGEDLG_H__6956FFF3_88B1_11D4_BCB1_00A0C9081566__INCLUDED_)
