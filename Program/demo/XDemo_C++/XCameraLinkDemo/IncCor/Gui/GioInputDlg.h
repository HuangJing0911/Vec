#if !defined(AFX_GIOINPUTDLG_H__E0E42E13_F7AB_457A_A396_AF8B3222F7AE__INCLUDED_)
#define AFX_GIOINPUTDLG_H__E0E42E13_F7AB_457A_A396_AF8B3222F7AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GioInputDlg.h : header file
//
#define MAX_INPUT_GIO_IO		12
#define INPUT_DIALOG_BACKGOUND_COLOR RGB(0xB0, 0xB0, 0xB0)
#define INPUT_DIALOG_TEXT_COLOR RGB(0, 0, 0x80)

/////////////////////////////////////////////////////////////////////////////
// CGioInputDlg dialog

class CGioInputDlg;

struct GIOContextInput
{
	CGioInputDlg *pDlg;
	UINT32 io;
};

class SAPCLASSGUI_CLASS CGioInputDlg : public CDialog
{

// Construction
public:
	CGioInputDlg(CWnd* pParent,UINT iDevice, SapGio *pGio);// standard constructor
	CWnd* m_pParent;
	UINT m_iDevice;

// Dialog Data
	//{{AFX_DATA(CGioInputDlg)
	enum { IDD = IDD_SCG_GIO_INPUT };
	CButton		m_Static_Label;
	CComboBox	m_Combo_Signal_Event;
	CComboBox	m_Combo_Input_Level;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGioInputDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	static void GioCallbackInfo(SapGioCallbackInfo *pInfo);
	BOOL Create();
	UINT MoveWindow(UINT offset);
	BOOL Update();
	GIOContextInput m_gioContext[MAX_INPUT_GIO_IO];
	UINT m_GioEventCount[MAX_INPUT_GIO_IO];

protected:
	CBrush m_bkBrush; 
	SapGio *m_pGio;
	BOOL ResetInterruptCount();

	// Generated message map functions
	//{{AFX_MSG(CGioInputDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnEnableCallback();
	afx_msg void OnSelchangeComboInputLevel();
	afx_msg void OnSelchangeComboSignalEvent();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GIOINPUTDLG_H__E0E42E13_F7AB_457A_A396_AF8B3222F7AE__INCLUDED_)
