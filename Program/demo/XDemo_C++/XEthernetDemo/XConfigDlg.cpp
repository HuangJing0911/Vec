// XConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XEthernetDemo.h"
#include "XConfigDlg.h"
#include "XDevice.h"
#include "XEthernetDemoDlg.h"
// CXConfigDlg dialog

extern CXEthernetDemoDlg* demo_dlg_;
IMPLEMENT_DYNAMIC(CXConfigDlg, CPropertyPage)

CXConfigDlg::CXConfigDlg()
	: CPropertyPage(CXConfigDlg::IDD)
	, _mac_str(_T(""))
	, _cmd_port(0)
	, _img_port(0)
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
	, _dm_pixel_num(0)
	, _energy_mode(0)
{

}

CXConfigDlg::~CXConfigDlg()
{
}

void CXConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LOCAL_ADAPTER, _local_ip_combo);
	DDX_Text(pDX, IDC_EDIT_GCU_MAC, _mac_str);
	DDX_Control(pDX, IDC_IPADDRESS_GCU_IP, _ip_control);
	DDX_Text(pDX, IDC_EDIT_GCU_CMD_PORT, _cmd_port);
	DDX_Text(pDX, IDC_EDIT_GCU_IMG_PORT, _img_port);
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
	DDX_CBIndex(pDX, IDC_COMBO_DM_PIXEL, _dm_pixel_num);
	DDX_CBIndex(pDX, IDC_ENERGY_MODE, _energy_mode);
}


BEGIN_MESSAGE_MAP(CXConfigDlg, CPropertyPage)

	ON_CBN_SELCHANGE(IDC_COMBO_LOCAL_ADAPTER, &CXConfigDlg::OnCbnSelchangeComboLocalAdapter)
	ON_BN_CLICKED(IDC_RECOVER, &CXConfigDlg::OnBnClickedRecover)
	ON_BN_CLICKED(IDC_RESET, &CXConfigDlg::OnBnClickedReset)
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
	UpdateData(1);
	WSADATA wsData;  
    ::WSAStartup(MAKEWORD(2,2), &wsData);  
	char host_name[128];
	
	if( gethostname(host_name, 128) == 0 )
	{
		// Get host adresses
		struct hostent * host_;	
		int i; 	
		host_ = gethostbyname(host_name); 	
		for( i = 0; host_!= NULL && host_->h_addr_list[i]!= NULL; i++ ) 	
		{
	
			 LPCSTR psz=inet_ntoa (*(struct in_addr *)host_->h_addr_list[i]);
			 _local_ip_combo.AddString(psz);
		}

	}
	UpdateData(0);
	return TRUE;

}

DWORD ConvertIP(DWORD IP)
{
	DWORD temp;
	temp = (BYTE)IP;
	temp <<=8;
	temp |= (BYTE)(IP>>8);
	temp <<=8;
	temp |= (BYTE)(IP>>16);
	temp <<=8;
	temp |= (BYTE)(IP>>24);
	return temp;
}

CString GetMacString(uint8_t* mac_)
{
	char mac_str[20];
	for(int8_t i=0; i<6; i++)
		sprintf(&mac_str[2*i], "%02X", mac_[i]);
	mac_str[12] = 0;
	CString str(mac_str);
	return str;
}


void CXConfigDlg::OnCbnSelchangeComboLocalAdapter()
{
	// TODO: Add your control notification handler code here
	UpdateData(1);
	int cur_sel = _local_ip_combo.GetCurSel();
	CString ip_str;
	_local_ip_combo.GetLBText(cur_sel, ip_str);

	//Clse xsystem object, if it is openning
	_xsystem_->Close();
	_xsystem_->SetLocalIP(ip_str.GetBuffer());
	if(_xsystem_->Open())
	{
		//Find device
		int dev_num = _xsystem_->FindDevice();
		if(dev_num > 0)
		{
			//Trace the first device info
			XDevice* dev_= _xsystem_->GetDevice(0);
			uint8_t* mac_ = dev_->GetMAC();
			_mac_str = GetMacString(mac_);
			DWORD ip_value = ConvertIP(inet_addr(dev_->GetIP()));
			_ip_control.SetAddress(ip_value);
			_cmd_port = dev_->GetCmdPort();
			_img_port = dev_->GetImgPort();
		}
		else
		{
			DWORD ip_value = 0;
			_ip_control.SetAddress(ip_value);
			_cmd_port = 0;
			_img_port = 0;
			AfxMessageBox("Find no device");
		}
		
	}
	else
		AfxMessageBox("XSystem object fail to open");

	UpdateData(0);
}

void CXConfigDlg::OnBnClickedRecover()
{
	// TODO: Add your control notification handler code here
	//Recover to default network settings
	_xsystem_->RecoverDevice();
	//Find device after recover
	_xsystem_->FindDevice();
}

void CXConfigDlg::OnBnClickedReset()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	DWORD ip_value;
	_ip_control.GetAddress(ip_value);
	in_addr in;
	in.S_un.S_addr = ConvertIP(ip_value);
	char* ip_ = inet_ntoa(in);
	
	//Get device object
	XDevice* dev_ = _xsystem_->GetDevice(0);
	if (NULL == dev_)
		return;
	//Chage device info
	dev_->SetIP(ip_);
	dev_->SetCmdPort(_cmd_port);
	dev_->SetImgPort(_img_port);

	//Reset network settings
	_xsystem_->ConfigureDevice(dev_);
	//Find device after resetting
	_xsystem_->FindDevice();
}

void CXConfigDlg::OnBnClickedOpen()
{
	UpdateData();
	XDevice* dev_ = _xsystem_->GetDevice(0);
	if(NULL == dev_)
		return;
	_xcommand_->SetFactory(_xfactory_);
	_xacquisition_->SetFactory(_xfactory_);
	_xtransfer_->SetLineNum(_line_number);
	_xacquisition_->RegisterFrameTransfer(_xtransfer_);
	
	//Add info header to image line
	_xacquisition_->EnableLineInfo(1);



	if(_xcommand_->Open(dev_))
	{
		//Check whether it is dual energy LCS system
		demo_dlg_->_is_non_hilo = 0;
		demo_dlg_->_is_dual_line_lcs = 0;

		uint32_t op_mode = dev_->GetOPMode();
		if(op_mode == XOP_MODE_NON_HILO)
		{
			//For hi/lo trigger mode the correction object is XOffHLCorrect 
			demo_dlg_->_xoff_correct_ = &(demo_dlg_->_xoff_hl_correct);
		}
		else
		{
			//For other mode the correction object is XOffCorrect
			demo_dlg_->_xoff_correct_ = &(demo_dlg_->_xoff_correct);
		}
		//Update correction pointers
		demo_dlg_->_config_dlg._xoff_correct_ = demo_dlg_->_xoff_correct_;
		demo_dlg_->_operate_dlg._xoff_correct_ = demo_dlg_->_xoff_correct_;
		
		if(dev_->GetCardType() == XCARD_TYPE_DM_DOUBLE)
		{
			//Dual line lcs system		
			if(op_mode == XOP_MODE_NON_HILO)
			{
				demo_dlg_->_is_dual_line_lcs = 2;  //Dual energy
				demo_dlg_->_xdual_line_correct.SetDualEnergy(1);
			}
			else
			{
				demo_dlg_->_is_dual_line_lcs = 1;	//Single energy
				demo_dlg_->_xdual_line_correct.SetDualEnergy(0);
			}
		}
		else 
		{
		
			if(op_mode == XOP_MODE_NON_HILO)
			{
				demo_dlg_->_is_non_hilo = 1;
			}
		} 
		
		if(_xacquisition_->Open(dev_, _xcommand_))
		{
			if(!demo_dlg_->_is_non_hilo && !demo_dlg_->_is_dual_line_lcs)
				_xdisplay_->Open(dev_, _line_number, demo_dlg_->GetDispWnd()->m_hWnd, _color_mode);
			else if(demo_dlg_->_is_non_hilo)
			{
				//For hilo trigger mode system, it combine two lines of different energy into one line
				_xdisplay_->Open(dev_->GetPixelNumber()*2, _line_number/2, dev_->GetPixelDepth(), demo_dlg_->GetDispWnd()->m_hWnd, _color_mode);
			}
			else if(1 == demo_dlg_->_is_dual_line_lcs)
			{
				//For dual line LCS system of single energy, it separates one line into 2 sub lines
				_xdisplay_->Open(dev_->GetPixelNumber()/2, _line_number*2, dev_->GetPixelDepth(), demo_dlg_->GetDispWnd()->m_hWnd, _color_mode);
			}
			else if(2 == demo_dlg_->_is_dual_line_lcs)
			{
				//Dual line LCS system of dual energy
				_xdisplay_->Open(dev_, _line_number, demo_dlg_->GetDispWnd()->m_hWnd, _color_mode);
			}

				
			_xoff_correct_->Open(dev_);
			_xpiecewise_correct_->Open(dev_);
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
	_xpiecewise_correct_->Close();
	_xdisplay_->Close();
	_xacquisition_->Close();
	_xcommand_->Close();

}

void CXConfigDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	OnBnClickedClose();
	_xsystem_->Close();
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
