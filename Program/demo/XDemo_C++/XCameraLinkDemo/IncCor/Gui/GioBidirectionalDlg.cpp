// GioBidirectionalDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SapClassBasic.h"
#include "SapClassGui.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RISING_EDGE_CALLBACK "Rising Edge"
#define FALLING_EDGE_CALLBACK "Falling Edge"
#define FAULT_CALLBACK "fault"

#ifndef CORGIO_VAL_EVENT_TYPE_FAULT
#define CORGIO_VAL_EVENT_TYPE_FAULT 4
#endif

/////////////////////////////////////////////////////////////////////////////
// CGioBidirectionalDlg dialog


CGioBidirectionalDlg::CGioBidirectionalDlg(CWnd* pParent,UINT iDevice, SapGio *pGio)
	: CDialog(CGioBidirectionalDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGioBidirectionalDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	int iIO;
	m_pParent = pParent;
	m_iDevice = iDevice;
	m_pGio = pGio;

	for (iIO=0; iIO<MAX_INPUT_GIO_IO; iIO++)
	{
		m_gioContext[iIO].pDlg = this;
		m_gioContext[iIO].io = iIO;
		m_GioEventCount[iIO] = 0;
	}
}


void CGioBidirectionalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGioBidirectionalDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_SCG_GIO_INPUT, m_Static_Label);
	DDX_Control(pDX, IDC_SCG_COMBO_SIGNAL_EVENT, m_Combo_Signal_Event);
	DDX_Control(pDX, IDC_SCG_COMBO_INPUT_LEVEL, m_Combo_Input_Level);
	//}}AFX_DATA_MAP
}

UINT CGioBidirectionalDlg::MoveWindow(UINT offset)
{
	RECT rect;
	UINT h;

	GetClientRect(&rect);
	h = rect.bottom-rect.top ;
	rect.top = 5;
	rect.left = offset; 
	rect.right = rect.left + rect.right;
	CDialog::MoveWindow(&rect,TRUE);
	return rect.right + 10;
}

BEGIN_MESSAGE_MAP(CGioBidirectionalDlg, CDialog)
	//{{AFX_MSG_MAP(CGioBidirectionalDlg)
	ON_BN_CLICKED(IDC_SCG_ENABLE1, OnEnableCallback)
	ON_CBN_SELCHANGE(IDC_SCG_COMBO_INPUT_LEVEL, OnSelchangeComboInputLevel)
	ON_CBN_SELCHANGE(IDC_SCG_COMBO_SIGNAL_EVENT, OnSelchangeComboSignalEvent)
   ON_CBN_SELCHANGE(IDC_SCG_DIR1, OnSelchangeComboDir1)
   ON_CBN_SELCHANGE(IDC_SCG_DIR2, OnSelchangeComboDir2)
   ON_CBN_SELCHANGE(IDC_SCG_DIR3, OnSelchangeComboDir3)
   ON_CBN_SELCHANGE(IDC_SCG_DIR4, OnSelchangeComboDir4)
   ON_CBN_SELCHANGE(IDC_SCG_DIR5, OnSelchangeComboDir5)
   ON_CBN_SELCHANGE(IDC_SCG_DIR6, OnSelchangeComboDir6)
   ON_CBN_SELCHANGE(IDC_SCG_DIR7, OnSelchangeComboDir7)
   ON_CBN_SELCHANGE(IDC_SCG_DIR8, OnSelchangeComboDir8)
   ON_CBN_SELCHANGE(IDC_SCG_DIR9, OnSelchangeComboDir9)
   ON_CBN_SELCHANGE(IDC_SCG_DIR10, OnSelchangeComboDir10)
   ON_CBN_SELCHANGE(IDC_SCG_DIR11, OnSelchangeComboDir11)
   ON_CBN_SELCHANGE(IDC_SCG_DIR12, OnSelchangeComboDir12)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SCG_ENABLE2, OnEnableCallback)
	ON_BN_CLICKED(IDC_SCG_ENABLE3, OnEnableCallback)
	ON_BN_CLICKED(IDC_SCG_ENABLE4, OnEnableCallback)
	ON_BN_CLICKED(IDC_SCG_ENABLE5, OnEnableCallback)
	ON_BN_CLICKED(IDC_SCG_ENABLE6, OnEnableCallback)
	ON_BN_CLICKED(IDC_SCG_ENABLE7, OnEnableCallback)
	ON_BN_CLICKED(IDC_SCG_ENABLE8, OnEnableCallback)
	ON_BN_CLICKED(IDC_SCG_ENABLE9, OnEnableCallback)
	ON_BN_CLICKED(IDC_SCG_ENABLE10, OnEnableCallback)
	ON_BN_CLICKED(IDC_SCG_ENABLE11, OnEnableCallback)
	ON_BN_CLICKED(IDC_SCG_ENABLE12, OnEnableCallback)
	ON_BN_CLICKED(IDC_SCG_BT_RESET, OnBtReset)
   ON_BN_CLICKED(IDC_SCG_PIN1, OnPin1)
	ON_BN_CLICKED(IDC_SCG_PIN2, OnPin2)
	ON_BN_CLICKED(IDC_SCG_PIN3, OnPin3)
	ON_BN_CLICKED(IDC_SCG_PIN4, OnPin4)
	ON_BN_CLICKED(IDC_SCG_PIN5, OnPin5)
	ON_BN_CLICKED(IDC_SCG_PIN6, OnPin6)
	ON_BN_CLICKED(IDC_SCG_PIN7, OnPin7)
	ON_BN_CLICKED(IDC_SCG_PIN8, OnPin8)
	ON_BN_CLICKED(IDC_SCG_PIN9, OnPin9)
	ON_BN_CLICKED(IDC_SCG_PIN10, OnPin10)
	ON_BN_CLICKED(IDC_SCG_PIN11, OnPin11)
	ON_BN_CLICKED(IDC_SCG_PIN12, OnPin12)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGioBidirectionalDlg message handlers

BOOL CGioBidirectionalDlg::PreTranslateMessage(MSG* pMsg) 
{ 
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) 
		return true; 

	return CDialog::PreTranslateMessage(pMsg); 
} 

void CGioBidirectionalDlg::GioCallbackInfo(SapGioCallbackInfo *pInfo)
{
CGioBidirectionalDlg* pInputDlg;
CString strEventCount;

    pInputDlg = (CGioBidirectionalDlg*)pInfo->GetContext();

    // Set the Input event counter
    strEventCount.Format("%d", pInfo->GetEventCount());
	pInputDlg->m_GioEventCount[pInfo->GetPinNumber()]++;
}


BOOL CGioBidirectionalDlg::Create()
{
	BOOL bCreate;
	bCreate = CDialog::Create(CGioBidirectionalDlg::IDD,m_pParent);
	if (!bCreate)
		return FALSE;

	return TRUE;
}


BOOL CGioBidirectionalDlg::OnInitDialog() 
{
	BOOL status;
	UINT32 capInputLevel=0,capInput=0,prmInputLevel=0,capInputControl=0,prmInputControl=0;
	UINT32 capInputControlLevel=0,prmInputControlLevel=0, prmDirection=0;
	UINT32 capConnector=0,prmConnector=0,capEventType=0;
	UINT32 iSelection,iIO;
   
	char label[128];

	CDialog::OnInitDialog();

	// Association of CallBack function for Input Module
	m_pGio->SetCallbackInfo(GioCallbackInfo,this);

	if (m_pGio->IsCapabilityValid(CORGIO_CAP_INPUT_LEVEL))
	   status = m_pGio->GetCapability(CORGIO_CAP_INPUT_LEVEL,&capInputLevel);
	if (m_pGio->IsCapabilityValid(CORGIO_CAP_DIR_INPUT))
	   status = m_pGio->GetCapability(CORGIO_CAP_DIR_INPUT,&capInput);
	if (m_pGio->IsParameterValid(CORGIO_PRM_INPUT_LEVEL))
	   status = m_pGio->GetParameter(CORGIO_PRM_INPUT_LEVEL,&prmInputLevel); 
	if (m_pGio->IsCapabilityValid(CORGIO_CAP_INPUT_CONTROL_METHOD))
	   status = m_pGio->GetCapability(CORGIO_CAP_INPUT_CONTROL_METHOD,&capInputControl); 
	if (m_pGio->IsParameterValid(CORGIO_PRM_INPUT_CONTROL_METHOD))
	   status = m_pGio->GetParameter(CORGIO_PRM_INPUT_CONTROL_METHOD,&prmInputControl); 

	if (m_pGio->IsCapabilityValid(CORGIO_CAP_CONNECTOR))
	   status = m_pGio->GetCapability(CORGIO_CAP_CONNECTOR,&capConnector); 
	if (m_pGio->IsParameterValid(CORGIO_PRM_CONNECTOR))
	   status = m_pGio->GetParameter(CORGIO_PRM_CONNECTOR,&prmConnector); 
	if (m_pGio->IsCapabilityValid(CORGIO_CAP_EVENT_TYPE))
	   status = m_pGio->GetCapability(CORGIO_CAP_EVENT_TYPE,&capEventType); 


	if (m_pGio->IsParameterValid(CORGIO_PRM_LABEL))
	   status = m_pGio->GetParameter(CORGIO_PRM_LABEL,label);
	if (m_pGio->IsCapabilityValid(CORGIO_CAP_INPUT_CONTROL_POLARITY))
	   status = m_pGio->GetCapability(CORGIO_CAP_INPUT_CONTROL_POLARITY,&capInputControlLevel); 
	if (m_pGio->IsParameterValid(CORGIO_PRM_INPUT_CONTROL_POLARITY))
	   status = m_pGio->GetParameter(CORGIO_PRM_INPUT_CONTROL_POLARITY,&prmInputControlLevel); 
   
   CorStrncat(label," (Bidirectional)", sizeof(label));

	m_Static_Label.SetWindowText(label);

	// enable only the IO available
	for (iIO=0; iIO < (UINT32)m_pGio->GetNumPins(); iIO++)
	{
		((CButton*)GetDlgItem(IDC_SCG_PIN1 + iIO))->EnableWindow(capInput & (1<<iIO) );
		((CButton*)GetDlgItem(IDC_SCG_ENABLE1  + iIO))->EnableWindow(capInput & (1<<iIO));		 
		((CButton*)GetDlgItem(IDC_SCG_ST_COUNT1 + iIO))->EnableWindow(capInput & (1<<iIO));	
      GetDlgItem(IDC_SCG_DIR1 + iIO)->EnableWindow(capInput & (1<<iIO));	
	}

   // fill direction combo boxes
   m_pGio->GetParameter(CORGIO_PRM_DIR_OUTPUT, &prmDirection);
	for (iIO=0; iIO < (UINT32)m_pGio->GetNumPins(); iIO++)
	{
		((CComboBox*)GetDlgItem(IDC_SCG_DIR1 + iIO))->AddString("Input");
      ((CComboBox*)GetDlgItem(IDC_SCG_DIR1 + iIO))->AddString("Output");
      ((CComboBox*)GetDlgItem(IDC_SCG_DIR1 + iIO))->SetCurSel(prmDirection & (1<<iIO)?1:0);
      ((CButton*)GetDlgItem(IDC_SCG_PIN1 + iIO))->EnableWindow(prmDirection & (1<<iIO)?1:0);
	}


	// Fill Input Level combo box
	if (capInputLevel & CORGIO_VAL_INPUT_LEVEL_LVTTL)
	{
		iSelection = m_Combo_Input_Level.AddString("3.3-Volts Single Ended");
		if (prmInputLevel & CORGIO_VAL_INPUT_LEVEL_LVTTL)
			m_Combo_Input_Level.SetCurSel(iSelection);
	}
	if (capInputLevel & CORGIO_VAL_INPUT_LEVEL_TTL)
	{
		iSelection = m_Combo_Input_Level.AddString("5-Volts Single Ended");
		if (prmInputLevel & CORGIO_VAL_INPUT_LEVEL_TTL)
			m_Combo_Input_Level.SetCurSel(iSelection);
	}
	if (capInputLevel & CORGIO_VAL_INPUT_LEVEL_12VOLTS)
	{
		iSelection = m_Combo_Input_Level.AddString("12-Volts Single Ended");
		if (prmInputLevel & CORGIO_VAL_INPUT_LEVEL_12VOLTS)
			m_Combo_Input_Level.SetCurSel(iSelection);
	}
	if (capInputLevel & CORGIO_VAL_INPUT_LEVEL_24VOLTS)
	{
		iSelection = m_Combo_Input_Level.AddString("24-Volts Single Ended");
		if (prmInputLevel & CORGIO_VAL_INPUT_LEVEL_24VOLTS)
			m_Combo_Input_Level.SetCurSel(iSelection);
	}
	if (capInputLevel & CORGIO_VAL_INPUT_LEVEL_LVDS)
	{
		iSelection = m_Combo_Input_Level.AddString("LVDS RS-644");
		if (prmInputLevel & CORGIO_VAL_INPUT_LEVEL_LVDS)
			m_Combo_Input_Level.SetCurSel(iSelection);
	}
	if (capInputLevel & CORGIO_VAL_INPUT_LEVEL_422)
	{
		iSelection = m_Combo_Input_Level.AddString("RS-422");
		if (prmInputLevel & CORGIO_VAL_INPUT_LEVEL_422)
			m_Combo_Input_Level.SetCurSel(iSelection);
	}

	m_Combo_Input_Level.EnableWindow(prmInputLevel != 0);

	// Fill Combo Signal Event
	if (capEventType & CORGIO_VAL_EVENT_TYPE_RISING_EDGE)
		m_Combo_Signal_Event.AddString(RISING_EDGE_CALLBACK);
	if (capEventType & CORGIO_VAL_EVENT_TYPE_FALLING_EDGE)
		m_Combo_Signal_Event.AddString(FALLING_EDGE_CALLBACK);
	if (capEventType & CORGIO_VAL_EVENT_TYPE_FAULT)
		m_Combo_Signal_Event.AddString(FAULT_CALLBACK);
	if (capEventType)
		m_Combo_Signal_Event.SetCurSel(0);
	else
		m_Combo_Signal_Event.EnableWindow(FALSE);

	if (m_Combo_Signal_Event.GetCount()==0)
	{
		m_Combo_Signal_Event.EnableWindow(FALSE);
		// enable only the IO available
		for (iIO=0; iIO < (UINT32)m_pGio->GetNumPins(); iIO++)
		{
			((CButton*)GetDlgItem(IDC_SCG_ENABLE1  + iIO))->EnableWindow(FALSE);		 
		}
	}
	if (m_Combo_Input_Level.GetCount()==0)
		m_Combo_Input_Level.EnableWindow(FALSE);

	return TRUE;
}

void CGioBidirectionalDlg::OnSelchangeComboSignalEvent() 
{
	UpdateData(TRUE);
	UINT32 iSelection;

	iSelection = m_Combo_Signal_Event.GetCurSel();

	OnEnableCallback();
}

void CGioBidirectionalDlg::ChangePinDirection(UINT32 pinNumber, int direction)
{
   BOOL status;
   pinNumber--;
   switch (direction)
   {
      case 0:
      {
         ((CButton*)GetDlgItem(IDC_SCG_PIN1 + pinNumber))->EnableWindow(FALSE);
         ((CButton*)GetDlgItem(IDC_SCG_ENABLE1 + pinNumber))->EnableWindow(m_Combo_Signal_Event.GetCount()>0);
         status = m_pGio->SetPinConfig(pinNumber, SapGio::PinInput);
         break;
      }
      case 1:
      {
         ((CButton*)GetDlgItem(IDC_SCG_PIN1 + pinNumber))->EnableWindow(TRUE);
         ((CButton*)GetDlgItem(IDC_SCG_ENABLE1 + pinNumber))->EnableWindow(FALSE);
         status = m_pGio->SetPinConfig(pinNumber, SapGio::PinOutput);
         break;
      }
   }
}

void CGioBidirectionalDlg::OnSelchangeComboDir1()
{
   ChangePinDirection(1,((CComboBox*)GetDlgItem(IDC_SCG_DIR1))->GetCurSel());
}
void CGioBidirectionalDlg::OnSelchangeComboDir2()
{
   ChangePinDirection(2,((CComboBox*)GetDlgItem(IDC_SCG_DIR2))->GetCurSel());
}
void CGioBidirectionalDlg::OnSelchangeComboDir3()
{
   ChangePinDirection(3,((CComboBox*)GetDlgItem(IDC_SCG_DIR3))->GetCurSel());
}
void CGioBidirectionalDlg::OnSelchangeComboDir4()
{
   ChangePinDirection(4,((CComboBox*)GetDlgItem(IDC_SCG_DIR4))->GetCurSel());
}
void CGioBidirectionalDlg::OnSelchangeComboDir5()
{
   ChangePinDirection(5,((CComboBox*)GetDlgItem(IDC_SCG_DIR5))->GetCurSel());
}
void CGioBidirectionalDlg::OnSelchangeComboDir6()
{
   ChangePinDirection(6,((CComboBox*)GetDlgItem(IDC_SCG_DIR6))->GetCurSel());
}
void CGioBidirectionalDlg::OnSelchangeComboDir7()
{
   ChangePinDirection(7,((CComboBox*)GetDlgItem(IDC_SCG_DIR7))->GetCurSel());
}
void CGioBidirectionalDlg::OnSelchangeComboDir8()
{
   ChangePinDirection(8,((CComboBox*)GetDlgItem(IDC_SCG_DIR8))->GetCurSel());
}
void CGioBidirectionalDlg::OnSelchangeComboDir9()
{
   ChangePinDirection(9,((CComboBox*)GetDlgItem(IDC_SCG_DIR9))->GetCurSel());
}
void CGioBidirectionalDlg::OnSelchangeComboDir10()
{
   ChangePinDirection(10,((CComboBox*)GetDlgItem(IDC_SCG_DIR10))->GetCurSel());
}
void CGioBidirectionalDlg::OnSelchangeComboDir11()
{
   ChangePinDirection(11,((CComboBox*)GetDlgItem(IDC_SCG_DIR11))->GetCurSel());
}
void CGioBidirectionalDlg::OnSelchangeComboDir12()
{
   ChangePinDirection(12,((CComboBox*)GetDlgItem(IDC_SCG_DIR12))->GetCurSel());
}



void CGioBidirectionalDlg::OnEnableCallback() 
{
	BOOL status;
	UINT32 level;
	UINT32 iIO, jIO;
	int iSelection = -1;
	char str[64]={0};

	UpdateData(TRUE);

	iSelection = m_Combo_Signal_Event.GetCurSel();
	m_Combo_Signal_Event.GetLBText(iSelection,str);

   if (strcmp(str,RISING_EDGE_CALLBACK))
		level = CORGIO_VAL_EVENT_TYPE_FALLING_EDGE;
   else if (strcmp(str,FALLING_EDGE_CALLBACK))
		level = CORGIO_VAL_EVENT_TYPE_RISING_EDGE;
	else
		level = CORGIO_VAL_EVENT_TYPE_RISING_EDGE;


	for (iIO=0; iIO < (UINT32)m_pGio->GetNumPins(); iIO++)
	{
			status = m_pGio->DisableCallback(iIO);
	}

	for (jIO=0; jIO < (UINT32)m_pGio->GetNumPins(); jIO++)
	{
		if (((CButton*)GetDlgItem(IDC_SCG_ENABLE1+jIO))->GetCheck())
		{
			status = m_pGio->EnableCallback(jIO, level);
		}
	}
}

	
BOOL CGioBidirectionalDlg::Update()
{
	SapGio::PinState *states = NULL;
	BOOL status = true;
	UINT32 iIO;
	UINT32 jIO;

	if (m_pGio == NULL)
		return FALSE;

   // Get the state of all input pins of the current I/O group.
   // Note: It is also possible to get the state of a single input pin if
	//       desired. See SapGio::GetPinState(int pinNumber, PinState *pPinState)
	states = new SapGio::PinState[m_pGio->GetNumPins()];
	m_pGio->SetDisplayStatusMode(SapManager::StatusLog, NULL);
	status = m_pGio->GetPinState(states);
	m_pGio->SetDisplayStatusMode(SapManager::StatusNotify, NULL);

	for (iIO=0; iIO < (UINT32)m_pGio->GetNumPins(); iIO++)
	{
		// Pins Input Edit controls identifiers is a sequence.
		if (states[iIO] == SapGio::PinLow)
			((CButton*)GetDlgItem(IDC_SCG_PIN1 + iIO))->SetWindowText("LOW");
		else if (states[iIO] == SapGio::PinHigh)
			((CButton*)GetDlgItem(IDC_SCG_PIN1 + iIO))->SetWindowText("HIGH");
	}

	delete [] states;
	states = NULL;

	for (jIO=0; jIO < (UINT32)m_pGio->GetNumPins(); jIO++)
	{
		CString str;
		str.Format("%d",this->m_GioEventCount[jIO]);
		((CButton*)GetDlgItem(IDC_SCG_ST_COUNT1 + jIO))->SetWindowText(str);
	}
	return status;
}


void CGioBidirectionalDlg::OnSelchangeComboInputLevel() 
{
	UINT32 iSelection=0,prmInputLevel=0;
	char str[64]={0};
	CORSTATUS status=0;

	UpdateData(TRUE);

	iSelection = m_Combo_Input_Level.GetCurSel();
	m_Combo_Input_Level.GetLBText(iSelection,str);
	
	switch (str[0])
	{
		case '3':
			prmInputLevel = CORGIO_VAL_INPUT_LEVEL_LVTTL;
			break;
		case '5':
			prmInputLevel = CORGIO_VAL_INPUT_LEVEL_TTL;
			break;
		case '1':
			prmInputLevel = CORGIO_VAL_INPUT_LEVEL_12VOLTS;
			break;
		case '2':
			prmInputLevel = CORGIO_VAL_INPUT_LEVEL_24VOLTS;
			break;
		case 'L':
			prmInputLevel = CORGIO_VAL_INPUT_LEVEL_LVDS;
			break;
		case 'R':
			prmInputLevel = CORGIO_VAL_INPUT_LEVEL_422;
			break;
	}
   status = m_pGio->SetParameter(CORGIO_PRM_INPUT_LEVEL,prmInputLevel);
}


HBRUSH CGioBidirectionalDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	switch ( nCtlColor )
	{
		case CTLCOLOR_STATIC:
           break;

		case CTLCOLOR_DLG:

		return (HBRUSH)(m_bkBrush.GetSafeHandle());	

		default:
			break;
	}

	return hbr;
}

BOOL CGioBidirectionalDlg::ResetInterruptCount()
{
	UINT32 iIO;

	if (m_pGio == NULL)
		return FALSE;

	for (iIO=0; iIO < (UINT32)m_pGio->GetNumPins(); iIO++)
	{
		CString str;
		m_GioEventCount[iIO]=0;
		str.Format("%d",m_GioEventCount[iIO]);
		((CButton*)GetDlgItem(IDC_SCG_ST_COUNT1 + iIO))->SetWindowText(str);
	}
	return TRUE;
}



void CGioBidirectionalDlg::OnBtReset() 
{
	ResetInterruptCount();
}


void CGioBidirectionalDlg::ChangePinState(int pinNumber)
{
	CString pinButtonText;
   UINT32 state=0;
   BOOL status = FALSE;

   //start counting at 0
   pinNumber--;

   CButton* pinButton = (CButton*)GetDlgItem(IDC_SCG_PIN1 + pinNumber);
	pinButton->GetWindowText(pinButtonText);
	if (pinButtonText.CompareNoCase("HIGH") == 0)
		pinButton->SetWindowText("LOW");
	else if (pinButtonText.CompareNoCase("LOW") == 0)
	{
		pinButton->SetWindowText("HIGH");
		pinButton->SetButtonStyle(1,true);
	}
	else if (pinButtonText.CompareNoCase("N/A") == 0)
	{
		pinButton->SetWindowText("LOW");		
		pinButton->SetButtonStyle(1,true);
	}

   if(pinButtonText.CompareNoCase("HIGH") == 0)
	   state = 0;
	else
	   state = 1;
		
   status = m_pGio->SetPinState(pinNumber, (SapGio::PinState)state);
}

void CGioBidirectionalDlg::OnPin1() 
{
	ChangePinState(1);	
}
void CGioBidirectionalDlg::OnPin2() 
{
	ChangePinState(2);	
}
void CGioBidirectionalDlg::OnPin3() 
{
	ChangePinState(3);	
}
void CGioBidirectionalDlg::OnPin4() 
{
	ChangePinState(4);	
}
void CGioBidirectionalDlg::OnPin5() 
{
	ChangePinState(5);	
}
void CGioBidirectionalDlg::OnPin6() 
{
	ChangePinState(6);	
}
void CGioBidirectionalDlg::OnPin7() 
{
	ChangePinState(7);	
}
void CGioBidirectionalDlg::OnPin8() 
{
	ChangePinState(8);	
}
void CGioBidirectionalDlg::OnPin9() 
{
	ChangePinState(9);	
}
void CGioBidirectionalDlg::OnPin10() 
{
	ChangePinState(10);	
}
void CGioBidirectionalDlg::OnPin11() 
{
	ChangePinState(11);	
}
void CGioBidirectionalDlg::OnPin12() 
{
	ChangePinState(12);	
}

