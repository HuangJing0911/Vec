// XControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XCameraLinkDemo.h"
#include "XControlDlg.h"


// CXControlDlg dialog

IMPLEMENT_DYNAMIC(CXControlDlg, CPropertyPage)

CXControlDlg::CXControlDlg()
	: CPropertyPage(CXControlDlg::IDD)
	, _int_time(0)
	, _non_int_time(0)
	, _cmd_str(_T(""))
	, _cur_dm(0)
	, _high_gain(0)
	, _low_gain(0)
	, _download_thread(DownloadThread, this)
	, _firmware_type(0)
	, _script_thread(ScriptThread,this)
	, _en_led(FALSE)
	, _gcu_test_pattern(0)
	, _dm_test_pattern(0)
{

}

CXControlDlg::~CXControlDlg()
{
}

void CXControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INT_TIME, _int_time);
	DDX_Text(pDX, IDC_EDIT_NON_INT_TIME, _non_int_time);
	DDX_Text(pDX, IDC_EDIT_CMD, _cmd_str);
	DDX_Control(pDX, IDC_COMBO_DM_INDEX, _dm_index);
	DDX_Text(pDX, IDC_EDIT_HIGH_GAIN, _high_gain);
	DDX_Text(pDX, IDC_EDIT_LOW_GAIN, _low_gain);
	DDX_Check(pDX, IDC_CHECK_EN_LED, _en_led);
	DDX_CBIndex(pDX, IDC_COMBO_GCU_TEST_PAT, _gcu_test_pattern);
	DDX_CBIndex(pDX, IDC_COMBO_DM_TEST_PAT, _dm_test_pattern);
}


BEGIN_MESSAGE_MAP(CXControlDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_SET_INT_TIME, &CXControlDlg::OnBnClickedSetIntTime)
	ON_BN_CLICKED(IDC_SET_NON_INT_TIME, &CXControlDlg::OnBnClickedSetNonIntTime)
	ON_BN_CLICKED(IDC_SEND_CMD, &CXControlDlg::OnBnClickedSendCmd)
	ON_CBN_SELCHANGE(IDC_COMBO_DM_INDEX, &CXControlDlg::OnCbnSelchangeComboDmIndex)
	ON_BN_CLICKED(IDC_RESET_DM_GAIN, &CXControlDlg::OnBnClickedResetDmGain)
	ON_BN_CLICKED(IDC_RESET_ALL_DM, &CXControlDlg::OnBnClickedResetAllDm)
	ON_BN_CLICKED(IDC_DOWN_GCU, &CXControlDlg::OnBnClickedDownGcu)
	ON_BN_CLICKED(IDC_DOWN_DM, &CXControlDlg::OnBnClickedDownDm)
	ON_BN_CLICKED(IDC_DOWN_SCRIPT, &CXControlDlg::OnBnClickedDownScript)
	ON_BN_CLICKED(IDC_CHECK_EN_LED, &CXControlDlg::OnBnClickedCheckEnLed)
	ON_CBN_SELCHANGE(IDC_COMBO_GCU_TEST_PAT, &CXControlDlg::OnCbnSelchangeComboGcuTestPat)
	ON_CBN_SELCHANGE(IDC_COMBO_DM_TEST_PAT, &CXControlDlg::OnCbnSelchangeComboDmTestPat)
	ON_BN_CLICKED(IDC_GCU_HEALTH, &CXControlDlg::OnBnClickedGcuHealth)
END_MESSAGE_MAP()


// CXControlDlg message handlers

void CXControlDlg::Initial()
{
	if(_xcommand_ && _xcommand_->GetIsOpen())
	{
		UpdateData(1);
		uint64_t para_data;
		_xcommand_->GetPara(XPARA_INT_TIME, para_data);
		_int_time = static_cast<UINT>(para_data);

		_xcommand_->GetPara(XPARA_NON_INTTIME, para_data);
		_non_int_time = static_cast<UINT>(para_data);

		_xcommand_->GetPara(XPARA_EN_LED, para_data);
		_en_led = static_cast<BOOL>(para_data);

		_xcommand_->GetPara(XPARA_GCU_TEST_MODE, para_data);
		_gcu_test_pattern = static_cast<int>(para_data);

		_xcommand_->GetPara(XPARA_CH_NUM, para_data);
		uint8_t total_card = 0;
		total_card += static_cast<uint8_t>(para_data);
		total_card += static_cast<uint8_t>(para_data>>8);
		total_card += static_cast<uint8_t>(para_data>>16);
		total_card += static_cast<uint8_t>(para_data>>24);
		total_card += static_cast<uint8_t>(para_data>>32);
		
		_dm_index.ResetContent();
		for(int32_t i=0; i<total_card; i++)
		{
			CString str;
			str.Format("DM%d", i+1);
			_dm_index.AddString(str);
		}
		UpdateData(0);
	}

}
void CXControlDlg::OnBnClickedSetIntTime()
{
	UpdateData();
	_xcommand_->SetPara(XPARA_INT_TIME, _int_time);
}

void CXControlDlg::OnBnClickedSetNonIntTime()
{
	UpdateData();
	_xcommand_->SetPara(XPARA_NON_INTTIME, _non_int_time);
}

void CXControlDlg::OnBnClickedSendCmd()
{
	UpdateData();
	char recv_str[512];
	_xcommand_->SendAscCmd(_cmd_str.GetBuffer(), recv_str);
	_cmd_str = _cmd_str + recv_str;
	UpdateData(0);
}


void CXControlDlg::OnCbnSelchangeComboDmIndex()
{
	UpdateData(1);
	_cur_dm = _dm_index.GetCurSel()+1;
	uint64_t para_data;
	_xcommand_->GetPara(XPARA_DM_GAIN,para_data, _cur_dm);
	_low_gain = static_cast<UCHAR>(para_data);
	_high_gain = static_cast<UCHAR>(para_data>>8);

	_xcommand_->GetPara(XPARA_DM_TEST_MODE, para_data, _cur_dm);
	_dm_test_pattern = static_cast<int>(para_data);

	float temperature;
	float high_temp;
	float humidity;
	float v1 =0.0f;
	float v2=0.0f;
	float v3=0.0f;
	float v4=0.0f;
	float v5=0.0f;
	float v6=0.0f;
	float v7=0.0f;

	if(_xcommand_->GetPara(XPARA_DM_HEALTH,temperature, humidity, v1,v2,v3,v4,v5,v6,v7,high_temp, _cur_dm))
	{
		CString str;
		str.Format("DM%u Health Info:\nTemperature: %.2fC\nHumidity: %.2f%%\nV1: %.2fV\nV2: %.2fV\nV3: %.2fV\nV4: %.2fV\nV5: %.2fV\nV6: %.2fV\nV7: %.2fV\nHigh Precision Temperature: %.2fC\n", _cur_dm, temperature, humidity, 
			v1,v2,v3,v4,v5,v6,v7, high_temp);
		GetDlgItem(IDC_HEALTH)->SetWindowText(str);
	}

	UpdateData(0);
}

void CXControlDlg::OnBnClickedResetDmGain()
{
	UpdateData();
	uint64_t para_data = 0;
	para_data += _high_gain;
	para_data = (para_data<<8) + _low_gain;
	_xcommand_->SetPara(XPARA_DM_GAIN, para_data, _cur_dm);
}

void CXControlDlg::OnBnClickedResetAllDm()
{
	UpdateData();
	uint64_t para_data = 0;
	para_data += _high_gain;
	para_data = (para_data<<8) + _low_gain;
	_xcommand_->SetPara(XPARA_DM_GAIN, para_data, 0xFF);
}

void CXControlDlg::OnBnClickedDownGcu()
{
	_firmware_type = 0x00;
	_download_thread.Start();
}

void CXControlDlg::OnBnClickedDownDm()
{
	_firmware_type = 0xFF;
	_download_thread.Start();
}

XTHREAD_CALL CXControlDlg::DownloadThread(void* arg)
{
	CXControlDlg* dlg_ = (CXControlDlg*) arg;
	dlg_->DownloadThreadMember();
	return 0;
}

#define FIRMWARE_SET_SIZE	512
#define CMD_FRAME_SIZE		516
uint32_t CXControlDlg::DownloadThreadMember()
{
		
	CString file_type = "RBF file(*.rbf)|*.rbf|All files(*.*)|*.*||";
	CFileDialog file_dlg(TRUE, "*.rbf", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,file_type);
	file_dlg.DoModal();
	CFile file;
	if(!file.Open(file_dlg.GetPathName(),CFile::modeRead | CFile::shareDenyWrite))
	{
		AfxMessageBox("Fails to open firmware file");
		return 0;
	}

	uint32_t file_size = GetFileSize((HANDLE)file.m_hFile, NULL);
	uint8_t cmd_data[CMD_FRAME_SIZE];
	uint8_t rev_data[CMD_FRAME_SIZE];
	int32_t len;
	uint32_t sect_num = 0;
	uint32_t send_len = 0;

	_xcommand_->StopHeartbeat();
	while(1)
	{
		memset(cmd_data, 0xFF, CMD_FRAME_SIZE);
		len = file.Read(cmd_data+4, FIRMWARE_SET_SIZE);
		uint32_t sect_num_n = htonl(sect_num);
		memcpy(cmd_data, &sect_num_n, 4);
		CProgressCtrl *pro_bar_ = (CProgressCtrl*) GetDlgItem(IDC_PROGRESS1);
		send_len += len;
		pro_bar_->SetPos(send_len*100/file_size);
		if(len < FIRMWARE_SET_SIZE)
		{
			//The last sect	
			if((_xcommand_->SendCommand(0xF3, 0x01, _firmware_type,CMD_FRAME_SIZE, cmd_data, rev_data)) <0)
			{
				AfxMessageBox("Error!!!");
			}
			break;
		}
		else
		{
			if((_xcommand_->SendCommand(0xF3, 0x01, _firmware_type,CMD_FRAME_SIZE, cmd_data, rev_data)) <0)
			{
				AfxMessageBox("Error!!!");
				break;
			}
		}
		sect_num++;
	}
	file.Close();
	
	_download_thread.Exit();
	AfxMessageBox("Finish!");
	_xcommand_->StartHeartbeat();
	return 0;
}


void CXControlDlg::OnBnClickedDownScript()
{
	_script_thread.Start();
}

XTHREAD_CALL CXControlDlg::ScriptThread(void* arg)
{
	CXControlDlg* dlg_ = (CXControlDlg*) arg;
	dlg_->ScriptThreadMember();
	return 0;
}

#define CMD_SIZE  64
uint32_t CXControlDlg::ScriptThreadMember()
{
	CString		file_type = "TXT file(*.txt)|*.txt|All files(*.*)|*.*||";
	CFileDialog	file_dlg(TRUE, "*.txt", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, file_type); 
	file_dlg.DoModal();

	FILE* file_ = NULL;
	char* end_;
	char send_buf[CMD_SIZE];
	char recv_str[CMD_SIZE];

	file_ = fopen(file_dlg.GetPathName(), "r");
	if(NULL == file_)
	{
		AfxMessageBox("Fails to open script file!");
		return 0;
	}

	while(fgets(send_buf, CMD_SIZE, file_)!= NULL)
	{
		end_ = strchr(send_buf,']');
		if(end_)
		{
			*(end_+1) = 0;
			_xcommand_->SendAscCmd(send_buf, recv_str);
		}
	}
	_script_thread.Exit();
	AfxMessageBox("Finish!");
	return 1;
}
void CXControlDlg::OnBnClickedCheckEnLed()
{
	UpdateData();
	_xcommand_->SetPara(XPARA_EN_LED, _en_led);
}

void CXControlDlg::OnCbnSelchangeComboGcuTestPat()
{
	UpdateData();
	_xcommand_->SetPara(XPARA_GCU_TEST_MODE, _gcu_test_pattern);
}

void CXControlDlg::OnCbnSelchangeComboDmTestPat()
{
	UpdateData();
	_xcommand_->SetPara(XPARA_DM_TEST_MODE, _dm_test_pattern, _cur_dm);
}

BOOL CXControlDlg::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN )
    {
 
        switch( pMsg->wParam )
        { 
		case VK_RETURN:
			OnBnClickedSendCmd();
			return TRUE;
		}
	}
	return CPropertyPage::PreTranslateMessage(pMsg);
}



void CXControlDlg::OnBnClickedGcuHealth()
{
	UpdateData(1);
	float temperature;
	float high_temp;
	float humidity;
	float v1 =0.0f;
	float v2=0.0f;
	float v3=0.0f;
	float v4=0.0f;
	float v5=0.0f;
	float v6=0.0f;
	float v7=0.0f;

	if(_xcommand_->GetPara(XPARA_GCU_HEALTH,temperature, humidity, v1,v2,v3,v4,v5,v6,v7,high_temp, _cur_dm))
	{
		CString str;
		str.Format("GCU Health Info:\nTemperature: %.2fC\nHumidity: %.2f%%\nV1: %.2fV\nV2: %.2fV\nV3: %.2fV\nV4: %.2fV\n",  temperature, humidity, 
			v1,v2,v3,v4);
		GetDlgItem(IDC_HEALTH)->SetWindowText(str);
	}
	UpdateData(0);
}
