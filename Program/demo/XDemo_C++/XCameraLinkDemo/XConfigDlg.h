#pragma once

//#include "xsystem.h"
#include "xcommand.h"
#include "ixfactory.h"
#include "xacquisition.h"
#include "xframe_transfer.h"
#include "xdisplay.h"
#include "xoff_correct.h"
#include "afxwin.h"
#include "afxcmn.h"

// CXConfigDlg dialog

class CXCameraLinkDemoDlg;

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

	XDevice*			_xdev_;
	XCommand*			_xcommand_;
	XAcquisition*		_xacquisition_;
	XFrameTransfer*		_xtransfer_;
	XDisplay*			_xdisplay_;
	XOffCorrect*		_xoff_correct_;
	IXFactory*			_xfactory_;
	
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
	int					_com_port;

	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedClose();
	afx_msg void OnClose();
	
	afx_msg void OnBnClickedResetConfig();
	afx_msg void OnBnClickedRestore();

	int _dm_pixel_num;
	int _energy_mode;
	int _baud_rate;
};
