// XOperateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XCameraLinkDemo.h"
#include "XOperateDlg.h"
#include "XCameraLinkDemoDlg.h"
extern CXCameraLinkDemoDlg* demo_dlg_;
// CXOperateDlg dialog

IMPLEMENT_DYNAMIC(CXOperateDlg, CPropertyPage)

CXOperateDlg::CXOperateDlg()
	: CPropertyPage(CXOperateDlg::IDD)
	, _gama(1.0f)
	, _file_name(_T("image.tif"))
	, _gain_target(50000)
	, _flash_index(0)
	, _en_gf_correct(FALSE)
	, _baseline_value(0)
	, _en_baseline_correct(FALSE)
	, _en_line_trigger(FALSE)
	, _line_trigger_mode(0)
	, _line_fine_delay(0)
	, _line_raw_delay(0)
	, _en_frame_trigger(FALSE)
	, _frame_trigger_mode(0)
	, _frame_delay(0)
	, _average_filter(0)
	, _sum_filter(0)
{

}

CXOperateDlg::~CXOperateDlg()
{

}

void CXOperateDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FILE_NAME, _file_name);
	DDX_Text(pDX, IDC_EDIT_GAIN_TARGET, _gain_target);
	DDX_Text(pDX, IDC_EDIT_FLASH_INDEX, _flash_index);
	DDV_MinMaxUInt(pDX, _flash_index, 0, 3);
	DDX_Check(pDX, IDC_CHECK_EN_GAIN_OFFSET, _en_gf_correct);
	DDX_Text(pDX, IDC_EDIT_BASELINE_VALUE, _baseline_value);
	DDV_MinMaxUInt(pDX, _baseline_value, 0, 1000);
	DDX_Check(pDX, IDC_CHECK_EN_BASELINE, _en_baseline_correct);
	DDX_Check(pDX, IDC_CHECK_EN_EX_LINE_TRIGGER, _en_line_trigger);
	DDX_CBIndex(pDX, IDC_COMBO_LINE_TR_MODE, _line_trigger_mode);
	DDX_Text(pDX, IDC_EDIT_LINE_FINE_DELAY, _line_fine_delay);
	DDX_Text(pDX, IDC_EDIT_LINE_RAW_DELAY, _line_raw_delay);
	DDX_Check(pDX, IDC_CHECK_EN_EX_FRAME_TRIGGER, _en_frame_trigger);
	DDX_CBIndex(pDX, IDC_COMBO_FRAME_TR_MODE, _frame_trigger_mode);
	DDX_Text(pDX, IDC_EDIT_FRAME_DELAY, _frame_delay);
	DDX_CBIndex(pDX, IDC_COMBO_AVERAGE, _average_filter);
	DDX_CBIndex(pDX, IDC_COMBO_SUM, _sum_filter);
}


BEGIN_MESSAGE_MAP(CXOperateDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_GRAB, &CXOperateDlg::OnBnClickedGrab)
	ON_BN_CLICKED(IDC_STOP, &CXOperateDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_SNAP, &CXOperateDlg::OnBnClickedSnap)
	ON_BN_CLICKED(IDC_GAMA_PLUS, &CXOperateDlg::OnBnClickedGamaPlus)
	ON_BN_CLICKED(IDC_GAMA_SUB, &CXOperateDlg::OnBnClickedGamaSub)
	ON_BN_CLICKED(IDC_SAVE_IMG, &CXOperateDlg::OnBnClickedSaveImg)
	ON_BN_CLICKED(IDC_CALC_OFFSET, &CXOperateDlg::OnBnClickedCalcOffset)
	ON_BN_CLICKED(IDC_CALC_GAIN, &CXOperateDlg::OnBnClickedCalcGain)
	ON_BN_CLICKED(IDC_SAVE_GAIN_OFFSET, &CXOperateDlg::OnBnClickedSaveGainOffset)
	ON_BN_CLICKED(IDC_LOAD_GAIN_OFFSET, &CXOperateDlg::OnBnClickedLoadGainOffset)
	ON_BN_CLICKED(IDC_CHECK_EN_GAIN_OFFSET, &CXOperateDlg::OnBnClickedCheckEnGainOffset)
	ON_BN_CLICKED(IDC_CHECK_EN_BASELINE, &CXOperateDlg::OnBnClickedCheckEnBaseline)
	ON_BN_CLICKED(IDC_CHECK_EN_EX_LINE_TRIGGER, &CXOperateDlg::OnBnClickedCheckEnExLineTrigger)
	ON_CBN_SELCHANGE(IDC_COMBO_LINE_TR_MODE, &CXOperateDlg::OnCbnSelchangeComboLineTrMode)
	ON_BN_CLICKED(IDC_LINE_FINE_DELAY, &CXOperateDlg::OnBnClickedLineFineDelay)
	ON_BN_CLICKED(IDC_LINE_RAW_DELAY, &CXOperateDlg::OnBnClickedLineRawDelay)
	ON_BN_CLICKED(IDC_CHECK_EN_EX_FRAME_TRIGGER, &CXOperateDlg::OnBnClickedCheckEnExFrameTrigger)
	ON_CBN_SELCHANGE(IDC_COMBO_FRAME_TR_MODE, &CXOperateDlg::OnCbnSelchangeComboFrameTrMode)
	ON_BN_CLICKED(IDC_FRAME_DELAY, &CXOperateDlg::OnBnClickedFrameDelay)
	ON_CBN_SELCHANGE(IDC_COMBO_AVERAGE, &CXOperateDlg::OnCbnSelchangeComboAverage)
	ON_CBN_SELCHANGE(IDC_COMBO_SUM, &CXOperateDlg::OnCbnSelchangeComboSum)
END_MESSAGE_MAP()


// CXOperateDlg message handlers

void CXOperateDlg::OnBnClickedGrab()
{
	demo_dlg_->_frame_counter = 0;
	demo_dlg_->_line_lost_num = 0;
	demo_dlg_->OnEventMsg(53, 0);
	_xacquisition_->Grab(0);
}

void CXOperateDlg::OnBnClickedStop()
{

	_xacquisition_->Stop();
}

void CXOperateDlg::OnBnClickedSnap()
{
	demo_dlg_->_line_lost_num = 0;
	demo_dlg_->OnEventMsg(53, 0);
	_xacquisition_->Snap();
}

void CXOperateDlg::OnBnClickedGamaPlus()
{
	_gama += 0.2f;
	_xdisplay_->SetGama(_gama);
}

void CXOperateDlg::OnBnClickedGamaSub()
{
	_gama -= 0.2f;
	_xdisplay_->SetGama(_gama);
}

void CXOperateDlg::OnBnClickedSaveImg()
{
	UpdateData();
	if(strstr(_file_name.GetBuffer(),".txt"))
		demo_dlg_->SaveImage(_file_name);
	else if(strstr(_file_name.GetBuffer(),".tif"))
	{
		XTifFormat tif_file(demo_dlg_->_cur_image_, &demo_dlg_->_xdevice);
		tif_file.Save(_file_name);
	}

}

void CXOperateDlg::OnBnClickedCalcOffset()
{
	AfxMessageBox("Please turn off X-Ray");
	_xoff_correct_->Reset();
	//Do offset correct
	_xoff_correct_->CalculatePara(1, _xacquisition_, _xtransfer_,0);
}

void CXOperateDlg::OnBnClickedCalcGain()
{
	AfxMessageBox("Please turn on X-Ray");
	UpdateData();
	//Do gain correct
	_xoff_correct_->CalculatePara(0, _xacquisition_, _xtransfer_, _gain_target);
}

void CXOperateDlg::OnBnClickedSaveGainOffset()
{
	UpdateData();
	_xcommand_->StopHeartbeat();
	_xoff_correct_->SaveFlash(_xcommand_, _flash_index);
	_xcommand_->StartHeartbeat();
}

void CXOperateDlg::OnBnClickedLoadGainOffset()
{
	UpdateData();
	_xoff_correct_->LoadFlash(_xcommand_, _flash_index);
}

void CXOperateDlg::OnBnClickedCheckEnGainOffset()
{
	UpdateData();
	_xcommand_->SetPara(XPARA_EN_GAIN_CORRECT, _en_gf_correct);
	_xcommand_->SetPara(XPARA_EN_OFFSET_CORRECT, _en_gf_correct);
}

void CXOperateDlg::OnBnClickedCheckEnBaseline()
{
	UpdateData();

	_xcommand_->SetPara(XPARA_BASE_LINE, _baseline_value);
	_xcommand_->SetPara(XPARA_EN_BASELINE_CORRECT, _en_baseline_correct);
}

void CXOperateDlg::OnBnClickedCheckEnExLineTrigger()
{
	UpdateData();
	_xcommand_->SetPara(XPARA_EN_LINE_TRIGGER, _en_line_trigger);
}

void CXOperateDlg::OnCbnSelchangeComboLineTrMode()
{
	UpdateData();
	_xcommand_->SetPara(XPARA_LINE_TRIGGER_MODE, _line_trigger_mode);
}

void CXOperateDlg::OnBnClickedLineFineDelay()
{
	UpdateData();
	_xcommand_->SetPara(XPARA_LINETRG_FINE_DELAY, _line_fine_delay);
}

void CXOperateDlg::OnBnClickedLineRawDelay()
{
	UpdateData();
	_xcommand_->SetPara(XPARA_LINETRG_RAW_DELAY, _line_raw_delay);
}

void CXOperateDlg::OnBnClickedCheckEnExFrameTrigger()
{
	UpdateData();
	if(_en_frame_trigger)
	{
		_xcommand_->SetPara(XPARA_EN_FRAME_TRIGGER, _xtransfer_->GetLineNum()/32);
	}
	else
		_xcommand_->SetPara(XPARA_EN_FRAME_TRIGGER, _en_frame_trigger);
}

void CXOperateDlg::OnCbnSelchangeComboFrameTrMode()
{
	UpdateData();
	_xcommand_->SetPara(XPARA_FRAME_TRIGGER_MODE, _frame_trigger_mode);
}

void CXOperateDlg::OnBnClickedFrameDelay()
{
	UpdateData();
	_xcommand_->SetPara(XPARA_FRAME_TRIGGER_DELAY, _frame_delay);
}

void CXOperateDlg::OnCbnSelchangeComboAverage()
{
	UpdateData();
	_xcommand_->SetPara(XPARA_AVERAGE_MODE, _average_filter);
}	

void CXOperateDlg::OnCbnSelchangeComboSum()
{
	UpdateData();
	_xcommand_->SetPara(XPARA_SUMMING_MODE, _sum_filter);
}

void CXOperateDlg::Initial()
{
	if(_xcommand_ && _xcommand_->GetIsOpen())
	{
		UpdateData(1);
		uint64_t para_data;
		
		_xcommand_->GetPara(XPARA_EN_GAIN_CORRECT, para_data);
		_en_gf_correct = static_cast<BOOL>(para_data);

		_xcommand_->GetPara(XPARA_BASE_LINE, para_data);
		_baseline_value = static_cast<UINT>(para_data);
		_xcommand_->GetPara(XPARA_EN_BASELINE_CORRECT, para_data);
		_en_baseline_correct = static_cast<BOOL>(para_data);

		_xcommand_->GetPara(XPARA_EN_LINE_TRIGGER, para_data);
		_en_line_trigger= static_cast<BOOL>(para_data);

		_xcommand_->GetPara(XPARA_LINE_TRIGGER_MODE, para_data);
		_line_trigger_mode = static_cast<int>(para_data);

		_xcommand_->GetPara(XPARA_LINETRG_FINE_DELAY, para_data);
		_line_fine_delay = static_cast<BYTE>(para_data);

		_xcommand_->GetPara(XPARA_LINETRG_RAW_DELAY, para_data);
		_line_raw_delay = static_cast<BYTE>(para_data);

		_xcommand_->GetPara(XPARA_EN_FRAME_TRIGGER, para_data);
		if(para_data)
			_en_frame_trigger = 1;
		else
			_en_frame_trigger = 0;

		_xcommand_->GetPara(XPARA_FRAME_TRIGGER_MODE, para_data);
		_frame_trigger_mode = static_cast<int>(para_data);

		_xcommand_->GetPara(XPARA_FRAME_TRIGGER_DELAY, para_data);
		_frame_delay = static_cast<BYTE>(para_data);

		_xcommand_->GetPara(XPARA_AVERAGE_MODE, para_data);
		_average_filter = static_cast<int>(para_data);

		_xcommand_->GetPara(XPARA_SUMMING_MODE, para_data);
		_sum_filter = static_cast<int>(para_data);

		
		UpdateData(0);
	}
}
