// XAdvanceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "XEthernetDemo.h"
#include "XAdvanceDlg.h"
#include "afxdialogex.h"

#include "XEthernetDemoDlg.h"
extern CXEthernetDemoDlg* demo_dlg_;

// XAdvanceDlg 对话框

IMPLEMENT_DYNAMIC(CXAdvanceDlg, CPropertyPage)

CXAdvanceDlg::CXAdvanceDlg()
	: CPropertyPage(CXAdvanceDlg::IDD)
	, _target1(10000)
	, _target2(20000)
	, _target3(50000)
	, _en_piecewise_correct(FALSE)
{

}

CXAdvanceDlg::~CXAdvanceDlg()
{
}

void CXAdvanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TARGET1, _target1);
	DDX_Text(pDX, IDC_EDIT_TARGET2, _target2);
	DDX_Text(pDX, IDC_EDIT_TARGET3, _target3);
	DDX_Check(pDX, IDC_CHECK_EN_P_CORRECT, _en_piecewise_correct);
}


BEGIN_MESSAGE_MAP(CXAdvanceDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_CALC_P_OFFSET, &CXAdvanceDlg::OnBnClickedCalcPOffset)
	ON_BN_CLICKED(IDC_CALC_P_GAIN1, &CXAdvanceDlg::OnBnClickedCalcPGain1)
	ON_BN_CLICKED(IDC_CALC_P_GAIN2, &CXAdvanceDlg::OnBnClickedCalcPGain2)
	ON_BN_CLICKED(IDC_CALC_P_GAIN3, &CXAdvanceDlg::OnBnClickedCalcPGain3)
	ON_BN_CLICKED(IDC_SAVE_PARA, &CXAdvanceDlg::OnBnClickedSavePara)
	ON_BN_CLICKED(IDC_LOAD_PARA, &CXAdvanceDlg::OnBnClickedLoadPara)
	ON_BN_CLICKED(IDC_CHECK_EN_P_CORRECT, &CXAdvanceDlg::OnBnClickedCheckEnPCorrect)
END_MESSAGE_MAP()


void CXAdvanceDlg::OnBnClickedCalcPOffset()
{
	AfxMessageBox("Please turn off X-Ray");
	//Calc offset
	_xpiecewise_correct_->CalculateOffset1(_xacquisition_, _xtransfer_);
}


void CXAdvanceDlg::OnBnClickedCalcPGain1()
{
	AfxMessageBox("Please turn to low X-Ray");
	UpdateData();
	_xpiecewise_correct_->CalculateGain1(_xacquisition_, _xtransfer_, _target1);
}


void CXAdvanceDlg::OnBnClickedCalcPGain2()
{
	AfxMessageBox("Please turn to middle X-Ray");
	UpdateData();
	_xpiecewise_correct_->CalculatePara2(_xacquisition_, _xtransfer_, _target2);
}


void CXAdvanceDlg::OnBnClickedCalcPGain3()
{
	AfxMessageBox("Please turn to high X-Ray");
	UpdateData();
	_xpiecewise_correct_->CalculatePara3(_xacquisition_, _xtransfer_, _target3);
}


void CXAdvanceDlg::OnBnClickedSavePara()
{
	_xpiecewise_correct_->SaveFile("piecewise_correct.txt");
}


void CXAdvanceDlg::OnBnClickedLoadPara()
{
	_xpiecewise_correct_->LoadFile("piecewise_correct.txt");
}


void CXAdvanceDlg::OnBnClickedCheckEnPCorrect()
{
	UpdateData();
	demo_dlg_->_en_piecewise_correct = _en_piecewise_correct;
}
