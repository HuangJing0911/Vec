#if !defined(AFX_CABDLG_H__042A4E91_57F3_11D4_BC47_00A0C9081566__INCLUDED_)
#define AFX_CABDLG_H__042A4E91_57F3_11D4_BC47_00A0C9081566__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CabDlg.h : header file
//

const int MAX_NUM_PORTS = 8;
const int MAX_NUM_CHANNELS = 16;

/////////////////////////////////////////////////////////////////////////////
// Transfer mode structure

typedef struct
{
	int value;
	int capMask;
	char *name;
} TRANSFERMODEINFO;

/////////////////////////////////////////////////////////////////////////////
// CCabDlg dialog

class SAPCLASSGUI_CLASS CCabDlg : public CDialog, public SapManager
{
// Construction
public:
   CCabDlg(CWnd* pParent, SapCab *pCab, SapXferNode *pSrcNode = NULL, int cabIndex = 0);
	~CCabDlg();

	SapCab &GetCab() { return *m_pCab; }
	void EnableControls(void);

// Dialog Data
	//{{AFX_DATA(CCabDlg)
	enum { IDD = IDD_SCG_CAB };
	CComboBox	m_cbTransferMode;
	int	m_blockSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCabDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	SapCab* m_pCab;
	BOOL    m_isTransmitter;
	int     m_currentPort;
	BOOL    m_bCreated;
	BOOL    m_bAllocated;

	// Cab capabilities common to all ports
	int m_portCountCap;
	int m_channelCountCap;
	int m_clkMinCap;
	int m_clkMaxCap;
	int m_blockSizeMinCap;
	int m_blockSizeMaxCap;

	// Port specific capabilities and parameters
	int m_portCap[MAX_NUM_PORTS];
	int m_allPortCap;
	int m_channel[MAX_NUM_PORTS];

	// Generated message map functions
	//{{AFX_MSG(CCabDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCabReceiver();
	afx_msg void OnCabTransmitter();
	afx_msg void OnSelchangeCabTransferMode();
	afx_msg void OnSelchangeCabChannelTable(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCabChannel0();
	afx_msg void OnCabChannel1();
	afx_msg void OnCabChannel2();
	afx_msg void OnCabChannel3();
	afx_msg void OnCabChannel4();
	afx_msg void OnCabChannel5();
	afx_msg void OnCabChannel6();
	afx_msg void OnCabChannel7();
	afx_msg void OnCabChannel8();
	afx_msg void OnCabChannel9();
	afx_msg void OnCabChannel10();
	afx_msg void OnCabChannel11();
	afx_msg void OnCabChannel12();
	afx_msg void OnCabChannel13();
	afx_msg void OnCabChannel14();
	afx_msg void OnCabChannel15();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Transfer mode table
	static TRANSFERMODEINFO m_transferModeInfo[];

	// Channel control IDs
	static int m_chControlId[];

	void ToggleCabChannel(int channel);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CABDLG_H__042A4E91_57F3_11D4_BC47_00A0C9081566__INCLUDED_)
