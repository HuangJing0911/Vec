#pragma once
#include "xcommand.h"
#include "afxcmn.h"
#include "afxwin.h"

// CXControlDlg dialog

class CXControlDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CXControlDlg)

public:
	CXControlDlg();
	virtual ~CXControlDlg();

// Dialog Data
	enum { IDD = IDD_CONTROL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	DECLARE_MESSAGE_MAP()
public:
	UINT _int_time;
	UINT _non_int_time;

	XCommand* _xcommand_;
	CString _cmd_str;
	CComboBox _dm_index;
	int _cur_dm;
	UCHAR _high_gain;
	UCHAR _low_gain;

	XThread		_download_thread;
	uint8_t		_firmware_type;
	XThread		_script_thread;
	void  Initial();
	afx_msg void OnBnClickedSetIntTime();
	afx_msg void OnBnClickedSetNonIntTime();
	
	afx_msg void OnBnClickedSendCmd();
	afx_msg void OnCbnSelchangeComboDmIndex();
	
	afx_msg void OnBnClickedResetDmGain();
	afx_msg void OnBnClickedResetAllDm();
	afx_msg void OnBnClickedDownGcu();
	afx_msg void OnBnClickedDownDm();
	static XTHREAD_CALL DownloadThread(void* arg);
	uint32_t DownloadThreadMember();
	afx_msg void OnBnClickedDownScript();
	static XTHREAD_CALL ScriptThread(void* arg);
	uint32_t ScriptThreadMember();
	BOOL _en_led;
	afx_msg void OnBnClickedCheckEnLed();
	afx_msg void OnCbnSelchangeComboGcuTestPat();
	int _gcu_test_pattern;
	afx_msg void OnCbnSelchangeComboDmTestPat();
	int _dm_test_pattern;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedGcuHealth();
};
