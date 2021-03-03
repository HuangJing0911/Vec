// CabDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SapClassBasic.h"
#include "SapClassGui.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Transfer mode table

TRANSFERMODEINFO CCabDlg::m_transferModeInfo[]=
{
	{CORCAB_VAL_TRANSFER_MODE_MULTICAST,		CORCAB_VAL_PORT_MULTICAST,			"Multicast"},
	{CORCAB_VAL_TRANSFER_MODE_ROUND_ROBIN,		CORCAB_VAL_PORT_ROUND_ROBIN,		"Round-Robin"},
	{CORCAB_VAL_TRANSFER_MODE_POINT_TO_POINT,	CORCAB_VAL_PORT_POINT_TO_POINT,	"Point-to-point"},
};

#define NUM_MODES	(sizeof(m_transferModeInfo) / sizeof(TRANSFERMODEINFO))

/////////////////////////////////////////////////////////////////////////////
// Channel control IDs

int CCabDlg::m_chControlId[] =
{
	IDC_SCG_CAB_CHANNEL_0,	IDC_SCG_CAB_CHANNEL_1,	IDC_SCG_CAB_CHANNEL_2,	IDC_SCG_CAB_CHANNEL_3,
	IDC_SCG_CAB_CHANNEL_4,	IDC_SCG_CAB_CHANNEL_5,	IDC_SCG_CAB_CHANNEL_6,	IDC_SCG_CAB_CHANNEL_7,
	IDC_SCG_CAB_CHANNEL_8,	IDC_SCG_CAB_CHANNEL_9,	IDC_SCG_CAB_CHANNEL_10,	IDC_SCG_CAB_CHANNEL_11,
	IDC_SCG_CAB_CHANNEL_12,	IDC_SCG_CAB_CHANNEL_13,	IDC_SCG_CAB_CHANNEL_14,	IDC_SCG_CAB_CHANNEL_15,
};

/////////////////////////////////////////////////////////////////////////////
// CCabDlg dialog


CCabDlg::CCabDlg(CWnd* pParent, SapCab *pCab, SapXferNode *pSrcNode, int cabIndex)
	: CDialog(CCabDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCabDlg)
	m_blockSize = 0;
	//}}AFX_DATA_INIT

	m_pCab= NULL;

	if( pCab)
	{
		m_pCab = pCab;
	}
	else
	{
      // Get some parameters from source node
      if (pSrcNode)
      {
         SapLocation loc(pSrcNode->GetLocation(), cabIndex);
         m_pCab = new SapCab(loc, pSrcNode);
      }
	}

	m_isTransmitter = FALSE;
	m_currentPort   = 0;
	m_allPortCap    = 0;
	m_bCreated      = FALSE;
	m_bAllocated    = (pCab == NULL)? TRUE: FALSE;
}

CCabDlg::~CCabDlg()
{
	if( m_bAllocated && m_pCab)
	{
		delete m_pCab;
	}
}

void CCabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCabDlg)
	DDX_Control(pDX, IDC_SCG_CAB_TRANSFER_MODE, m_cbTransferMode);
	DDX_Text(pDX, IDC_SCG_CAB_BLOCK_SIZE, m_blockSize);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate && GetDlgItem(IDC_SCG_CAB_BLOCK_SIZE)->IsWindowEnabled())
		DDV_MinMaxDWord(pDX, m_blockSize, m_blockSizeMinCap, m_blockSizeMaxCap);
}


BEGIN_MESSAGE_MAP(CCabDlg, CDialog)
	//{{AFX_MSG_MAP(CCabDlg)
	ON_BN_CLICKED(IDC_SCG_CAB_RECEIVER, OnCabReceiver)
	ON_BN_CLICKED(IDC_SCG_CAB_TRANSMITTER, OnCabTransmitter)
	ON_CBN_SELCHANGE(IDC_SCG_CAB_TRANSFER_MODE, OnSelchangeCabTransferMode)
	ON_NOTIFY(TCN_SELCHANGE, IDC_SCG_CAB_CHANNEL_TABLE, OnSelchangeCabChannelTable)
	ON_BN_CLICKED(IDC_SCG_CAB_CHANNEL_0, OnCabChannel0)
	ON_BN_CLICKED(IDC_SCG_CAB_CHANNEL_1, OnCabChannel1)
	ON_BN_CLICKED(IDC_SCG_CAB_CHANNEL_2, OnCabChannel2)
	ON_BN_CLICKED(IDC_SCG_CAB_CHANNEL_3, OnCabChannel3)
	ON_BN_CLICKED(IDC_SCG_CAB_CHANNEL_4, OnCabChannel4)
	ON_BN_CLICKED(IDC_SCG_CAB_CHANNEL_5, OnCabChannel5)
	ON_BN_CLICKED(IDC_SCG_CAB_CHANNEL_6, OnCabChannel6)
	ON_BN_CLICKED(IDC_SCG_CAB_CHANNEL_7, OnCabChannel7)
	ON_BN_CLICKED(IDC_SCG_CAB_CHANNEL_8, OnCabChannel8)
	ON_BN_CLICKED(IDC_SCG_CAB_CHANNEL_9, OnCabChannel9)
	ON_BN_CLICKED(IDC_SCG_CAB_CHANNEL_10, OnCabChannel10)
	ON_BN_CLICKED(IDC_SCG_CAB_CHANNEL_11, OnCabChannel11)
	ON_BN_CLICKED(IDC_SCG_CAB_CHANNEL_12, OnCabChannel12)
	ON_BN_CLICKED(IDC_SCG_CAB_CHANNEL_13, OnCabChannel13)
	ON_BN_CLICKED(IDC_SCG_CAB_CHANNEL_14, OnCabChannel14)
	ON_BN_CLICKED(IDC_SCG_CAB_CHANNEL_15, OnCabChannel15)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Utility functions

void CCabDlg::EnableControls(void)
{
	BOOL canTransmit = (m_allPortCap & CORCAB_VAL_PORT_TX) ? TRUE : FALSE;
	BOOL canReceive = (m_allPortCap & CORCAB_VAL_PORT_RX) ? TRUE : FALSE;

	// Make sure we have a valid transmitter/receiver setting
	if (m_isTransmitter && !canTransmit)
	{
		m_isTransmitter = FALSE;
	}
	if (!m_isTransmitter && !canReceive)
	{
		m_isTransmitter = TRUE;
	}

	GetDlgItem(IDC_SCG_CAB_TRANSMITTER)->EnableWindow(canTransmit);
	GetDlgItem(IDC_SCG_CAB_RECEIVER)->EnableWindow(canReceive);
	((CButton *)GetDlgItem(IDC_SCG_CAB_TRANSMITTER))->SetCheck(m_isTransmitter);
	((CButton *)GetDlgItem(IDC_SCG_CAB_RECEIVER))->SetCheck(!m_isTransmitter);

   int transferMode = (int)m_cbTransferMode.GetItemData(m_cbTransferMode.GetCurSel());
	BOOL isMulticast = (transferMode == CORCAB_VAL_TRANSFER_MODE_MULTICAST);
	BOOL isPointToPoint = (transferMode == CORCAB_VAL_TRANSFER_MODE_POINT_TO_POINT);
	BOOL isReadOnly = FALSE;

	// Check if block size parameter is read-only in point-to-point mode
	if (isPointToPoint && VALIDATE_HANDLE_CAB(m_pCab->GetHandle()) == StatusOk)
	{
		int saveMode;

      m_pCab->GetParameter(CORCAB_PRM_TRANSFER_MODE, &saveMode);
     	m_pCab->SetParameter(CORCAB_PRM_TRANSFER_MODE, CORCAB_VAL_TRANSFER_MODE_POINT_TO_POINT);

		SAPSTATUS status = CorCabSetPrm(m_pCab->GetHandle(), CORCAB_PRM_BLOCK_SIZE, m_blockSize);
		isReadOnly = (CORSTATUS_ID(status) == CORSTATUS_PRM_READ_ONLY);

     	m_pCab->SetParameter(CORCAB_PRM_TRANSFER_MODE, saveMode);
	}

	GetDlgItem(IDC_SCG_CAB_TRANSFER_MODE)->EnableWindow(m_isTransmitter);
	GetDlgItem(IDC_SCG_CAB_BLOCK_SIZE)->EnableWindow(m_isTransmitter && !isMulticast && !isReadOnly);

   // Do not declare in 'for' statement, as we get an error with Visual Studio 2005 in the next loop
   int i;

	for (i = 0; i < m_channelCountCap; i++)
	{
		int checked = (m_channel[m_currentPort] & CORCAB_VAL_CHANNEL(i)) ? 1 : 0;
		GetDlgItem(m_chControlId[i])->EnableWindow();
		((CButton *)GetDlgItem(m_chControlId[i]))->SetCheck(checked);
	}

	for (i = m_channelCountCap; i < MAX_NUM_CHANNELS; i++)
	{
		m_channel[m_currentPort] &= ~CORCAB_VAL_CHANNEL(i);
		GetDlgItem(m_chControlId[i])->EnableWindow(FALSE);
		((CButton *)GetDlgItem(m_chControlId[i]))->SetCheck(0);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCabDlg message handlers

BOOL CCabDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Check CAB object
	if( m_pCab == NULL)
	{
		CDialog::OnCancel();
		return FALSE;
	}
	
	// Check if CAB object has been already created
	if(!*m_pCab)
	{
		// Create CAB object
		if( !m_pCab->Create())	
		{
			CDialog::OnCancel();
			return FALSE;
		}
		
		// Set a flag that will tell us that CAB object has been created here
		m_bCreated= TRUE;
	}

	if (VALIDATE_HANDLE_CAB(m_pCab->GetHandle()) == StatusOk)
	{
		// Get capabilities common to all ports
   	if (!m_pCab->GetCapability(CORCAB_CAP_PORT_COUNT, &m_portCountCap))
		{
			CDialog::OnCancel();
			return FALSE;
		}

   	if (!m_pCab->GetCapability(CORCAB_CAP_CHANNEL_COUNT, &m_channelCountCap))
		{
			CDialog::OnCancel();
			return FALSE;
		}

   	if (!m_pCab->GetCapability(CORCAB_CAP_CLK_MIN, &m_clkMinCap))
		{
			CDialog::OnCancel();
			return FALSE;
		}

   	if (!m_pCab->GetCapability(CORCAB_CAP_CLK_MAX, &m_clkMaxCap))
		{
			CDialog::OnCancel();
			return FALSE;
		}

   	if (!m_pCab->GetCapability(CORCAB_CAP_BLOCK_SIZE_MIN, &m_blockSizeMinCap))
		{
			CDialog::OnCancel();
			return FALSE;
		}

   	if (!m_pCab->GetCapability(CORCAB_CAP_BLOCK_SIZE_MAX, &m_blockSizeMaxCap))
		{
			CDialog::OnCancel();
			return FALSE;
		}

		// Get parameters common to all ports
		int transferMode;

 	   if (!m_pCab->GetParameter(CORCAB_PRM_TRANSFER_MODE, &transferMode))
		{
			CDialog::OnCancel();
			return FALSE;
		}

 	   if (!m_pCab->GetParameter(CORCAB_PRM_BLOCK_SIZE, &m_blockSize))
		{
			CDialog::OnCancel();
			return FALSE;
		}

		// Get port specific capabilities and parameters. Also determine if the
		// Cab can transmit. If so, make it the default setting.
		m_isTransmitter = FALSE;

      // Do not declare in 'for' statement, as we get an error with Visual Studio 2005 in the next loop
      int i;

		for (i = 0; i < m_portCountCap; i++)
		{
        	if (!m_pCab->SetParameter(CORCAB_PRM_PORT_INDEX, i))
			{
				CDialog::OnCancel();
				return FALSE;
			}

      	if (!m_pCab->GetCapability(CORCAB_CAP_PORT, &m_portCap[i]))
			{
				CDialog::OnCancel();
				return FALSE;
			}

 	      if (!m_pCab->GetParameter(CORCAB_PRM_CHANNEL, &m_channel[i]))
			{
				CDialog::OnCancel();
				return FALSE;
			}

			m_allPortCap |= m_portCap[i];
			if (m_portCap[i] & CORCAB_VAL_PORT_TX)
			{
				m_isTransmitter = TRUE;
			}
		}

		m_currentPort = 0;
     	if (!m_pCab->SetParameter(CORCAB_PRM_PORT_INDEX, m_currentPort))
		{
			CDialog::OnCancel();
			return FALSE;
		}

		// Initialize transfer mode combo
		int curSel = NUM_MODES;

		for (i = 0; i < NUM_MODES; i++)
		{
			if (m_allPortCap & m_transferModeInfo[i].capMask)
			{
				m_cbTransferMode.AddString(m_transferModeInfo[i].name);
				m_cbTransferMode.SetItemData(i, m_transferModeInfo[i].value);
					
				if (m_transferModeInfo[i].value == transferMode)
				{
					m_cbTransferMode.SetCurSel(i);
					curSel = i;
				}
			}
		}

		// Default mode selection is not found above
		if (curSel == NUM_MODES)
		{
			m_cbTransferMode.SetCurSel(0);
		}

		// Add one property page for each port
		for (i = 0; i < m_portCountCap; i++)
		{
			CString label;
			label.Format("Port %ld", i);
			((CTabCtrl *)GetDlgItem(IDC_SCG_CAB_CHANNEL_TABLE))->InsertItem(i, label);
		}

		// Update control states
		EnableControls();

		// Update values in dialog box
		UpdateData(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCabDlg::OnOK() 
{
	if (VALIDATE_HANDLE_CAB(m_pCab->GetHandle()) == StatusOk)
	{
		// Get values from dialog box
		UpdateData(TRUE);

      // Do not declare in 'for' statement, as we get an error with Visual Studio 2005 in the next loop
      int i;

		// Make sure that all ports have at least one selected channel
		for (i = 0; i < m_portCountCap; i++)
		{
			if (m_channel[i] == 0)
			{
				CString msg;
				msg.Format("At least one channel must be checked for port %ld", i);
				MessageBox(msg, "CAB Error");
				return;
			}
		}

		// Set parameters common to all ports
      int transferMode = (int)m_cbTransferMode.GetItemData(m_cbTransferMode.GetCurSel());

     	if (!m_pCab->SetParameter(CORCAB_PRM_TRANSFER_MODE, transferMode))
		{
			return;
		}

		if (GetDlgItem(IDC_SCG_CAB_BLOCK_SIZE)->IsWindowEnabled())
		{
        	if (!m_pCab->SetParameter(CORCAB_PRM_BLOCK_SIZE, m_blockSize))
			{
				return;
			}
		}

		// Set port specific parameters.
		for (i = 0; i < m_portCountCap; i++)
		{
        	if (!m_pCab->SetParameter(CORCAB_PRM_PORT_INDEX, i))
			{
				return;
			}

        	if (!m_pCab->SetParameter(CORCAB_PRM_CHANNEL, m_channel[i]))
			{
				return;
			}
		}

     	if (!m_pCab->SetParameter(CORCAB_PRM_PORT_INDEX, 0))
		{
			return;
		}

	}

	CDialog::OnOK();
}

void CCabDlg::OnCabTransmitter() 
{
	m_isTransmitter = TRUE;
	EnableControls();
}

void CCabDlg::OnCabReceiver() 
{
	m_isTransmitter = FALSE;
	EnableControls();
}

void CCabDlg::OnSelchangeCabTransferMode() 
{
	EnableControls();
}

void CCabDlg::OnSelchangeCabChannelTable(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int port = ((CTabCtrl *)GetDlgItem((int)pNMHDR->idFrom))->GetCurSel();

	if (port >= 0)
	{
		m_currentPort = port;
		EnableControls();
	}
}

void CCabDlg::OnCabChannel0() {ToggleCabChannel(0);}
void CCabDlg::OnCabChannel1() {ToggleCabChannel(1);}
void CCabDlg::OnCabChannel2() {ToggleCabChannel(2);}
void CCabDlg::OnCabChannel3() {ToggleCabChannel(3);}
void CCabDlg::OnCabChannel4() {ToggleCabChannel(4);}
void CCabDlg::OnCabChannel5() {ToggleCabChannel(5);}
void CCabDlg::OnCabChannel6() {ToggleCabChannel(6);}
void CCabDlg::OnCabChannel7() {ToggleCabChannel(7);}
void CCabDlg::OnCabChannel8() {ToggleCabChannel(8);}
void CCabDlg::OnCabChannel9()  {ToggleCabChannel(9);}
void CCabDlg::OnCabChannel10() {ToggleCabChannel(10);}
void CCabDlg::OnCabChannel11() {ToggleCabChannel(11);}
void CCabDlg::OnCabChannel12() {ToggleCabChannel(12);}
void CCabDlg::OnCabChannel13() {ToggleCabChannel(13);}
void CCabDlg::OnCabChannel14() {ToggleCabChannel(14);}
void CCabDlg::OnCabChannel15() {ToggleCabChannel(15);}

void CCabDlg::ToggleCabChannel(int channel)
{
	m_channel[m_currentPort] ^= CORCAB_VAL_CHANNEL(channel);
	EnableControls();
}

BOOL CCabDlg::DestroyWindow() 
{
	if( m_bCreated)
	{
		m_pCab->Destroy();
		m_bCreated= FALSE;
	}
	
	return CDialog::DestroyWindow();
}
