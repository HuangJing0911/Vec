#if !defined(AFX_GIOBIDIRECTIONALDLG_H__E0E42E13_F7AB_457A_A396_AF8B3222F7AE__INCLUDED_)
#define AFX_GIOBIDIRECTIONALDLG_H__E0E42E13_F7AB_457A_A396_AF8B3222F7AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GioBidirectionalDlg.h : header file
//
#define MAX_INPUT_GIO_IO		12
#define INPUT_DIALOG_BACKGOUND_COLOR RGB(0xB0, 0xB0, 0xB0)
#define INPUT_DIALOG_TEXT_COLOR RGB(0, 0, 0x80)

/////////////////////////////////////////////////////////////////////////////
// CGioBidirectionalDlg dialog

class CGioBidirectionalDlg;

struct GIOContextBidirectional
{
	CGioBidirectionalDlg *pDlg;
	UINT32 io;
};

class SAPCLASSGUI_CLASS CGioBidirectionalDlg : public CDialog
{

// Construction
public:
	CGioBidirectionalDlg(CWnd* pParent,UINT iDevice, SapGio *pGio);// standard constructor
	CWnd* m_pParent;
	UINT m_iDevice;

// Dialog Data
	//{{AFX_DATA(CGioBidirectionalDlg)
	enum { IDD = IDD_SCG_GIO_BIDIR };
	CButton		m_Static_Label;
	CComboBox	m_Combo_Signal_Event;
	CComboBox	m_Combo_Input_Level;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGioBidirectionalDlg)
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
	GIOContextBidirectional m_gioContext[MAX_INPUT_GIO_IO];
	UINT m_GioEventCount[MAX_INPUT_GIO_IO];
   

protected:
	CBrush m_bkBrush; 
	SapGio *m_pGio;
	BOOL ResetInterruptCount();

	// Generated message map functions
	//{{AFX_MSG(CGioBidirectionalDlg)
	virtual BOOL OnInitDialog();
   void ChangePinDirection(UINT32 pinNumber, int direction);
   void ChangePinState(int pinNumber);
	afx_msg void OnEnableCallback();
	afx_msg void OnSelchangeComboInputLevel();
	afx_msg void OnSelchangeComboSignalEvent();
   afx_msg void OnSelchangeComboDir1();
   afx_msg void OnSelchangeComboDir2();
   afx_msg void OnSelchangeComboDir3();
   afx_msg void OnSelchangeComboDir4();
   afx_msg void OnSelchangeComboDir5();
   afx_msg void OnSelchangeComboDir6();
   afx_msg void OnSelchangeComboDir7();
   afx_msg void OnSelchangeComboDir8();
   afx_msg void OnSelchangeComboDir9();
   afx_msg void OnSelchangeComboDir10();
   afx_msg void OnSelchangeComboDir11();
   afx_msg void OnSelchangeComboDir12();
	afx_msg void OnPin1();
	afx_msg void OnPin2();
	afx_msg void OnPin3();
	afx_msg void OnPin4();
	afx_msg void OnPin5();
	afx_msg void OnPin6();
	afx_msg void OnPin7();
	afx_msg void OnPin8();
	afx_msg void OnPin9();
	afx_msg void OnPin10();
	afx_msg void OnPin11();
	afx_msg void OnPin12();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GIOBIDIRECTIONALDLG_H__E0E42E13_F7AB_457A_A396_AF8B3222F7AE__INCLUDED_)
