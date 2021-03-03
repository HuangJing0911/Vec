// XConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XCameraLinkDemo.h"
#include "XConfigDlg.h"
#include "XDevice.h"
#include "XCameraLinkDemoDlg.h"
// CXConfigDlg dialog

extern CXCameraLinkDemoDlg* demo_dlg_;
IMPLEMENT_DYNAMIC(CXConfigDlg, CPropertyPage)

CXConfigDlg::CXConfigDlg()
	: CPropertyPage(CXConfigDlg::IDD)
	, _line_number(512)
	, _color_mode(0)
	, _xgcu_type(0)
	, _card_type(0)
	, _op_mode(0)
	, _pixel_depth(0)
	, _card_num1(0)
	, _card_num2(0)
	, _card_num3(0)
	, _card_num4(0)
	, _card_num5(0)
	, _binning_mode(0)
	, _com_port(0)
	, _dm_pixel_num(0)
	, _energy_mode(0)
	, _baud_rate(0)
{

}

CXConfigDlg::~CXConfigDlg()
{
}

void CXConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_LINE_NUMBER, _line_number);
	DDX_CBIndex(pDX, IDC_COMBO_PSEUDO_COLOR, _color_mode);
	DDX_CBIndex(pDX, IDC_COMBO_GCU_TYPE, _xgcu_type);
	DDX_CBIndex(pDX, IDC_COMBO_CARD_TYPE, _card_type);
	DDX_CBIndex(pDX, IDC_COMBO_OP_MODE, _op_mode);
	DDX_CBIndex(pDX, IDC_COMBO_PIXEL_DEPTH, _pixel_depth);
	DDX_Text(pDX, IDC_EDIT_CARD_NUM1, _card_num1);
	DDV_MinMaxUInt(pDX, _card_num1, 0, 16);
	DDX_Text(pDX, IDC_EDIT_CARD_NUM2, _card_num2);
	DDV_MinMaxUInt(pDX, _card_num2, 0, 16);
	DDX_Text(pDX, IDC_EDIT_CARD_NUM3, _card_num3);
	DDX_Text(pDX, IDC_EDIT_CARD_NUM4, _card_num4);
	DDV_MinMaxUInt(pDX, _card_num4, 0, 16);
	DDX_Text(pDX, IDC_EDIT_CARD_NUM5, _card_num5);
	DDV_MinMaxUInt(pDX, _card_num5, 0, 16);
	DDV_MinMaxUInt(pDX, _card_num3, 0, 16);
	DDX_CBIndex(pDX, IDC_COMBO_BINNING_MODE, _binning_mode);
	DDX_CBIndex(pDX, IDC_COMBO_COM_PORT, _com_port);
	DDX_CBIndex(pDX, IDC_COMBO_DM_PIXEL, _dm_pixel_num);
	DDX_CBIndex(pDX, IDC_ENERGY_MODE, _energy_mode);
	DDX_CBIndex(pDX, IDC_COMBO_BAUD_RATE, _baud_rate);
}


BEGIN_MESSAGE_MAP(CXConfigDlg, CPropertyPage)

	ON_BN_CLICKED(IDC_OPEN, &CXConfigDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_CLOSE, &CXConfigDlg::OnBnClickedClose)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_RESET_CONFIG, &CXConfigDlg::OnBnClickedResetConfig)
	ON_BN_CLICKED(IDC_RESTORE, &CXConfigDlg::OnBnClickedRestore)
END_MESSAGE_MAP()


// CXConfigDlg message handlers
BOOL CXConfigDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	//Trace the network adapters
	return TRUE;

}


void CXConfigDlg::OnBnClickedOpen()
{
	UpdateData();
	XDevice* dev_ = _xdev_; 
	if(NULL == dev_)
		return;
	dev_->SetSerialPort(_com_port+1);
	dev_->SetCmdPort(_baud_rate);
	_xcommand_->SetFactory(_xfactory_);
	_xacquisition_->SetFactory(_xfactory_);
	_xtransfer_->SetLineNum(_line_number);
	_xacquisition_->RegisterFrameTransfer(_xtransfer_);
	//Add info header to image line
	//_xacquisition_->EnableLineInfo(1);

	if(_xcommand_->Open(dev_))
	{
		if(_xacquisition_->Open(dev_, _xcommand_))
		{
			_xdisplay_->Open(dev_, _line_number, demo_dlg_->GetDispWnd()->m_hWnd, _color_mode);
			_xoff_correct_->Open(dev_);
		}

		//Get detector infomation
		UpdateData(1);
		demo_dlg_->GetSN();

		uint64_t para_data;
		_xcommand_->GetPara(XPARA_GCU_TYPE, para_data);
		_xgcu_type = static_cast<int>(para_data);
		
		_xcommand_->GetPara(XPARA_CARD_TYPE, para_data);
		_card_type = static_cast<int>(para_data);
		
		_xcommand_->GetPara(XPARA_OPE_MODE, para_data);
		_op_mode = static_cast<int>(para_data);

		_xcommand_->GetPara(XPARA_PIXEL_DEPTH, para_data);
		switch(para_data)
		{
		case 16:
			_pixel_depth = 0;
			break;
		case 18:
			_pixel_depth = 1;
			break;
		case 20:
			_pixel_depth = 2;
			break;
		}
		_xcommand_->GetPara(XPARA_BINNING_MODE, para_data);
		_binning_mode = static_cast<int>(para_data);
		
		_xcommand_->GetPara(XPARA_ENERGY_MODE,para_data);
		_energy_mode = static_cast<int>(para_data);
		_xcommand_->GetPara(XPARA_DM_PIXEL_NUM, para_data);
		_dm_pixel_num = static_cast<int>(para_data)-5;

		_xcommand_->GetPara(XPARA_CH_NUM, para_data);
		_card_num5 = static_cast<UCHAR>(para_data);
		_card_num4 = static_cast<UCHAR>(para_data>>8);
		_card_num3 = static_cast<UCHAR>(para_data>>16);
		_card_num2 = static_cast<UCHAR>(para_data>>24);
		_card_num1 = static_cast<UCHAR>(para_data>>32);
		UpdateData(0);
	}
}

void CXConfigDlg::OnBnClickedClose()
{
	_xoff_correct_->Close();
	_xdisplay_->Close();
	_xacquisition_->Close();
	_xcommand_->Close();

}

void CXConfigDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	OnBnClickedClose();
	CPropertyPage::OnClose();
}

void CXConfigDlg::OnBnClickedResetConfig()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
	uint64_t total_card = 0;
	total_card = _card_num1;
	total_card = (total_card<<8) + _card_num2;
	total_card = (total_card<<8) + _card_num3;
	total_card = (total_card<<8) + _card_num4;
	total_card = (total_card<<8) + _card_num5;
	_xcommand_->SetPara(XPARA_CH_NUM, total_card);

	_xcommand_->SetPara(XPARA_OPE_MODE, _op_mode);
	_xcommand_->SetPara(XPARA_BINNING_MODE, _binning_mode);
	//Save all setting to flash
	_xcommand_->ExecutePara(XPARA_SAVE_PARA);	
	AfxMessageBox("Please click \"Close\" and \"Open\"");
}

void CXConfigDlg::OnBnClickedRestore()
{
	//Restore the default settings
	_xcommand_->ExecutePara(XPARA_INIT1_PARA);
	AfxMessageBox("Please click \"Close\" and \"Open\"");

}
