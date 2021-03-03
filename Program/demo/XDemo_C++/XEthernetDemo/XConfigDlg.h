#pragma once

#include "xsystem.h"
#include "xcommand.h"
#include "xgig_factory.h"
#include "xacquisition.h"
#include "xframe_transfer.h"
#include "xdisplay.h"
#include "xoff_correct.h"
#include "xpiecewise_correct.h"
#include "afxwin.h"
#include "afxcmn.h"

// CXConfigDlg dialog

class CXEthernetDemoDlg;

class CXConfigDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CXConfigDlg)

public:
	CXConfigDlg();
	virtual ~CXConfigDlg();

// Dialog Data
	enum { IDD = IDD_CONFIGURE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();

public:

	XSystem*			_xsystem_;
	XCommand*			_xcommand_;
	XAcquisition*		_xacquisition_;
	XFrameTransfer*		_xtransfer_;
	XDisplay*			_xdisplay_;
	XOffCorrect*		_xoff_correct_;
	XGigFactory*		_xfactory_;

	XPiecewiseCorrect*	_xpiecewise_correct_;
	
	CComboBox			_local_ip_combo;
	CString				_mac_str;
	CIPAddressCtrl		_ip_control;
	UINT				_cmd_port;
	UINT				_img_port;
	UINT				_line_number;
	int					_color_mode;
	int					_xgcu_type;
	int					_card_type;
	int					_op_mode;
	int					_pixel_depth;
	UCHAR				_card_num1;
	UCHAR				_card_num2;
	UCHAR				_card_num3;
	UCHAR				_card_num4;
	UCHAR				_card_num5;
	int					_binning_mode;
	int					_dm_pixel_num;

	afx_msg void OnCbnSelchangeComboLocalAdapter();
	afx_msg void OnBnClickedRecover();
	afx_msg void OnBnClickedReset();
	
	
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedClose();
	afx_msg void OnClose();
	
	afx_msg void OnBnClickedResetConfig();
	afx_msg void OnBnClickedRestore();

	int _energy_mode;
};
