#pragma once
#include "xcommand.h"
#include "xacquisition.h"
#include "xoff_correct.h"
#include "xdisplay.h"
#include "xframe_transfer.h"
// CXOperateDlg dialog

class CXOperateDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CXOperateDlg)

public:
	CXOperateDlg();
	virtual ~CXOperateDlg();

// Dialog Data
	enum { IDD = IDD_OPERATE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	XCommand*			_xcommand_;
	XAcquisition*		_xacquisition_;
	XOffCorrect*		_xoff_correct_;
	XDisplay*			_xdisplay_;
	XFrameTransfer*		_xtransfer_;

	float				_gama;
	UINT				_gain_target;
	CString				_file_name;
	UINT				_flash_index;
	
	BOOL				_en_gf_correct;
	BOOL				_en_baseline_correct;
	UINT				_baseline_value;
	BOOL				_en_line_trigger;
	int					_line_trigger_mode;
	BYTE				_line_fine_delay;
	BYTE				_line_raw_delay;
	BOOL				_en_frame_trigger;
	int					_frame_trigger_mode;
	BYTE				_frame_delay;
	int					_average_filter;
	int					_sum_filter;

	afx_msg void OnBnClickedGrab();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedSnap();
	afx_msg void OnBnClickedGamaPlus();
	afx_msg void OnBnClickedGamaSub();
	afx_msg void OnBnClickedSaveImg();
	
	afx_msg void OnBnClickedCalcOffset();
	afx_msg void OnBnClickedCalcGain();
	
	afx_msg void OnBnClickedSaveGainOffset();
	afx_msg void OnBnClickedLoadGainOffset();	
	afx_msg void OnBnClickedCheckEnGainOffset();
	afx_msg void OnBnClickedCheckEnBaseline();
	
	afx_msg void OnBnClickedCheckEnExLineTrigger();
	afx_msg void OnCbnSelchangeComboLineTrMode();

	afx_msg void OnBnClickedLineFineDelay();
	afx_msg void OnBnClickedLineRawDelay();
	
	afx_msg void OnBnClickedCheckEnExFrameTrigger();
	afx_msg void OnCbnSelchangeComboFrameTrMode();
	afx_msg void OnBnClickedFrameDelay();

	afx_msg void OnCbnSelchangeComboAverage();
	afx_msg void OnCbnSelchangeComboSum();

	void Initial();
};
