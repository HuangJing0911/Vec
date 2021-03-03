#if !defined(AFX_GIOOUTPUTDLG_H__1EACC042_7762_4B79_8219_82D0B8339795__INCLUDED_)
#define AFX_GIOOUTPUTDLG_H__1EACC042_7762_4B79_8219_82D0B8339795__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GioOutputDlg.h : header file
//

#define MAX_OUTPUT_GIO_IO		12
#define OUTPUT_DIALOG_BACKGROUND_COLOR RGB(0xA0, 0xA0, 0xA0)
#define OUTPUT_DIALOG_TEXT_COLOR RGB(0x80, 0, 0)

/////////////////////////////////////////////////////////////////////////////
// CGioOutputDlg dialog
class CGioOutputDlg;

struct GIOContextOutput
{
	CGioOutputDlg *pDlg;
	UINT32 io;
};


class SAPCLASSGUI_CLASS CGioOutputDlg : public CDialog
{
// Construction
public:
	CGioOutputDlg(CWnd* pParent,UINT iDevice, SapGio *pGio);// standard constructor
	CWnd* m_pParent;
	UINT m_iDevice;

// Dialog Data
	//{{AFX_DATA(CGioOutputDlg)
	enum { IDD = IDD_SCG_GIO_OUTPUT };
	CButton		m_Static_Label;
	CStatic		m_Picture;
	CStatic		m_Picture2;
	CComboBox	m_Combo_Output_State;
	CComboBox	m_Combo_Out_Signal_Event;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGioOutputDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL Create();
	BOOL Update();
	UINT MoveWindow(UINT offset);
	BOOL InitOutputDlg();
	GIOContextOutput m_gioContext[MAX_OUTPUT_GIO_IO];
	UINT m_GioEventCount[MAX_OUTPUT_GIO_IO];
	void ChangePinState(CButton* pinButton);
	void InitComboOutputState();

protected:
	CBrush m_bkBrush; 
	SapGio *m_pGio;
	UINT32 m_CapFaultDetect;

	// Generated message map functions
	//{{AFX_MSG(CGioOutputDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckOutputState();
	afx_msg void OnSelchangeComboOutputState();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void UpdateIO();
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GIOOUTPUTDLG_H__1EACC042_7762_4B79_8219_82D0B8339795__INCLUDED_)
