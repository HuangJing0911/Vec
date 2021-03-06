#pragma once
//#include "xcommand.h"
#include "xacquisition.h"
#include "xframe_transfer.h"
#include "xpiecewise_correct.h"

class CXAdvanceDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CXAdvanceDlg)

public:
	CXAdvanceDlg();
	virtual ~CXAdvanceDlg();


	enum { IDD = IDD_ADV_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

public:
	
	XAcquisition*		_xacquisition_;
	XPiecewiseCorrect*	_xpiecewise_correct_;
	XFrameTransfer*		_xtransfer_;
	
	UINT _target1;
	UINT _target2;
	UINT _target3;
	
	afx_msg void OnBnClickedCalcPOffset();
	afx_msg void OnBnClickedCalcPGain1();
	afx_msg void OnBnClickedCalcPGain2();
	afx_msg void OnBnClickedCalcPGain3();

	afx_msg void OnBnClickedSavePara();
	afx_msg void OnBnClickedLoadPara();
	BOOL _en_piecewise_correct;
	afx_msg void OnBnClickedCheckEnPCorrect();
};
