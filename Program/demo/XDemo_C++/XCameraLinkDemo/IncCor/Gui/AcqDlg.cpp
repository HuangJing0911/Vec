// AcqDlg.cpp : implementation file
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
// CAcqDlg dialog


CAcqDlg::CAcqDlg(CWnd* pParent, SapAcquisition *pAcq)
	: CDialog(CAcqDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAcqDlg)
	m_extTrigEnable = FALSE;
	//}}AFX_DATA_INIT
	m_pAcq = pAcq;
}


void CAcqDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAcqDlg)
	DDX_CBIndex(pDX, IDC_SCG_ACQ_CAMSEL, m_camSel);
	DDX_Check(pDX, IDC_SCG_ACQ_EXTERNAL_TRIGGER_ENABLE, m_extTrigEnable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAcqDlg, CDialog)
	//{{AFX_MSG_MAP(CAcqDlg)
	ON_BN_CLICKED(IDC_SCG_ACQ_EXTERNAL_TRIGGER_ENABLE, OnExternalTriggerEnable)
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAcqDlg message handlers

void CAcqDlg::OnOK() 
{
	// Get current setting
	UpdateData( TRUE);

	// Update Camera selection PRM
	if( m_camSelMax > 1)
	{
      if (!m_pAcq->SetParameter(CORACQ_PRM_CAMSEL, m_camSel))
		{
			CDialog::OnCancel();
			return;
		}
	}

	// Update Output Enable PRM
	if( m_extTrigCap)
	{
		int extTriggerDetectionPrm = CORACQ_VAL_RISING_EDGE;

      if (!m_pAcq->SetParameter(CORACQ_PRM_OUTPUT_ENABLE, m_extTrigEnable ? CORACQ_VAL_OUTPUT_ENABLE_ON_EXTRIG : CORACQ_VAL_OUTPUT_ENABLE_AUTO))
		{
			CDialog::OnCancel();
			return;
		}

		// Update External Trigger Detection PRM
		if( ((CButton *)GetDlgItem(IDC_SCG_ACQ_EXT_TRIGGER_RISING_EDGE))->GetCheck())
		{
			extTriggerDetectionPrm = CORACQ_VAL_RISING_EDGE;
		}
		else if( ((CButton *)GetDlgItem(IDC_SCG_ACQ_EXT_TRIGGER_FALLING_EDGE))->GetCheck())
		{
			extTriggerDetectionPrm = CORACQ_VAL_FALLING_EDGE;
		}
		else if( ((CButton *)GetDlgItem(IDC_SCG_ACQ_EXT_TRIGGER_ACTIVE_HIGH))->GetCheck())
		{
			extTriggerDetectionPrm = CORACQ_VAL_ACTIVE_HIGH;
		}
		else if( ((CButton *)GetDlgItem(IDC_SCG_ACQ_EXT_TRIGGER_ACTIVE_LOW))->GetCheck())
		{
			extTriggerDetectionPrm = CORACQ_VAL_ACTIVE_LOW;
		}

      if (!m_pAcq->SetParameter(CORACQ_PRM_EXT_TRIGGER_DETECTION, extTriggerDetectionPrm))
		{
			CDialog::OnCancel();
			return;
		}
	}

	// Update Bit Ordering PRM
	if( m_bitOrderingCap & ( m_bitOrderingCap != CORACQ_VAL_BIT_ORDERING_STD))
	{
		int bitOrdering;

		if( ((CButton *)GetDlgItem(IDC_SCG_ACQ_BIT_ORDERING_9_10))->GetCheck())
		{
			bitOrdering = CORACQ_VAL_BIT_ORDERING_9_10;
		}
		else if( ((CButton *)GetDlgItem(IDC_SCG_ACQ_BIT_ORDERING_MSB_10))->GetCheck())
		{
			bitOrdering = CORACQ_VAL_BIT_ORDERING_MSB_10;
		}
		else 
		{
			bitOrdering = CORACQ_VAL_BIT_ORDERING_STD;
		}

      if (!m_pAcq->SetParameter(CORACQ_PRM_BIT_ORDERING, bitOrdering))
		{
			CDialog::OnCancel();
			return;
		}
	}

	CDialog::OnOK();
}

BOOL CAcqDlg::OnInitDialog() 
{
	int i;
	int videoType;
	int camSelCap;
	int outputEnable;
	int interfaceType;
	char str[32];

	CDialog::OnInitDialog();

	// Verifie acquisition object
	if (m_pAcq == NULL)
	{
		AfxMessageBox("Invalid acquisition object");
		CDialog::OnCancel();
		return FALSE;
	}

	// Get the maximum number of cameras of the specified type that can be selected from
	// the acquisition module
   if (!m_pAcq->GetParameter(CORACQ_PRM_CAMSEL, &m_camSel))
	{
		CDialog::OnCancel();
		return FALSE;
	}

   if (!m_pAcq->GetParameter(CORACQ_PRM_VIDEO, &videoType))
	{
		CDialog::OnCancel();
		return FALSE;
	}

	switch (videoType)
	{
		case CORCAM_VAL_VIDEO_MONO:	
			camSelCap = CORACQ_CAP_CAMSEL_MONO; 
			break;

		case CORCAM_VAL_VIDEO_COLOR: 
			camSelCap = CORACQ_CAP_CAMSEL_COLOR; 
			break;

		case CORCAM_VAL_VIDEO_YC: 
			camSelCap = CORACQ_CAP_CAMSEL_YC; 
			break;

		case CORCAM_VAL_VIDEO_RGB: 
			camSelCap = CORACQ_CAP_CAMSEL_RGB; 
			break;

		default: 
			camSelCap = CORACQ_CAP_CAMSEL_MONO; 
			break;
	};

   if (!m_pAcq->GetCapability(camSelCap, &m_camSelMax))
	{
		CDialog::OnCancel();
		return FALSE;
	}

	// Initialize camera selector combo
	CComboBox *pCamSel = (CComboBox *) GetDlgItem(IDC_SCG_ACQ_CAMSEL);
	if (pCamSel != NULL)
	{
		for( i = 0; i < m_camSelMax; i++)
		{
			CorSnprintf(str, sizeof(str), "%d", i + 1);
			pCamSel->AddString(str);
		}
		pCamSel->SetCurSel( m_camSel);
	}

	if( m_camSelMax == 1)
	{
		// Gray out the menu
		GetDlgItem(IDC_SCG_ACQ_CAMSEL)->EnableWindow( FALSE);
	}

	// Initialize the External Trigger info
   if (!m_pAcq->GetCapability(CORACQ_CAP_EXT_TRIGGER, &m_extTrigCap))
	{
		CDialog::OnCancel();
		return FALSE;
	}

	if( m_extTrigCap)
	{
		int extTriggerDetectionPrm;

      if (!m_pAcq->GetCapability(CORACQ_CAP_EXT_TRIGGER_DETECTION, &m_extTrigDetectionCap))
		{
			CDialog::OnCancel();
			return FALSE;
		}
	
      if (!m_pAcq->GetParameter(CORACQ_PRM_EXT_TRIGGER_DETECTION, &extTriggerDetectionPrm))
		{
			CDialog::OnCancel();
			return FALSE;
		}

      if (!m_pAcq->GetParameter(CORACQ_PRM_OUTPUT_ENABLE, &outputEnable))
		{
			CDialog::OnCancel();
			return FALSE;
		}


		if( outputEnable == CORACQ_VAL_OUTPUT_ENABLE_ON_EXTRIG)
		{
			m_extTrigEnable = TRUE;
		}

		// Check the current state of the external trigger detection 
		if( extTriggerDetectionPrm & CORACQ_VAL_RISING_EDGE)
		{
			((CButton *)GetDlgItem(IDC_SCG_ACQ_EXT_TRIGGER_RISING_EDGE))->SetCheck(TRUE);
		}
		else if( extTriggerDetectionPrm & CORACQ_VAL_FALLING_EDGE)
		{
			((CButton *)GetDlgItem(IDC_SCG_ACQ_EXT_TRIGGER_FALLING_EDGE))->SetCheck(TRUE);
		}
		else if( extTriggerDetectionPrm & CORACQ_VAL_ACTIVE_HIGH)
		{
			((CButton *)GetDlgItem(IDC_SCG_ACQ_EXT_TRIGGER_ACTIVE_HIGH))->SetCheck(TRUE);
		}
		else if( extTriggerDetectionPrm & CORACQ_VAL_ACTIVE_LOW)
		{
			((CButton *)GetDlgItem(IDC_SCG_ACQ_EXT_TRIGGER_ACTIVE_LOW))->SetCheck(TRUE);
		}
	}
	else
	{
		// Disable all items related to the external trigger
		GetDlgItem(IDC_SCG_ACQ_EXT_TRIGGER_RISING_EDGE)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_ACQ_EXT_TRIGGER_FALLING_EDGE)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_ACQ_EXT_TRIGGER_ACTIVE_HIGH)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_ACQ_EXT_TRIGGER_ACTIVE_LOW)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_ACQ_EXTERNAL_TRIGGER_ENABLE)->EnableWindow(FALSE);
	}


   if (!m_pAcq->GetParameter(CORACQ_PRM_INTERFACE, &interfaceType))
	{
		CDialog::OnCancel();
		return FALSE;
	}

	if( (interfaceType == CORACQ_VAL_INTERFACE_DIGITAL) && m_pAcq->GetCapability(CORACQ_CAP_BIT_ORDERING, &m_bitOrderingCap) && m_bitOrderingCap)
	{
		int bitOrdering;
		SapFormat outputFormat;
		int pixelDepth;

		// Get current state
      if (!m_pAcq->GetParameter(CORACQ_PRM_BIT_ORDERING, &bitOrdering))
		{
			CDialog::OnCancel();
			return FALSE;
		}

      if (!m_pAcq->GetParameter(CORACQ_PRM_OUTPUT_FORMAT, &outputFormat))
		{
			CDialog::OnCancel();
			return FALSE;
		}

		pixelDepth= CORDATA_FORMAT_DATADEPTH( outputFormat);

		// Enable only the options supported by the driver and when required
		if( !m_bitOrderingCap || ( m_bitOrderingCap == CORACQ_VAL_BIT_ORDERING_STD))
		{
			// Select Standard bit ordering, and gray out all of the options
			((CButton *)GetDlgItem(IDC_SCG_ACQ_BIT_ORDERING_STD))->SetCheck(TRUE);

			GetDlgItem(IDC_SCG_ACQ_BIT_ORDERING_STD)->EnableWindow( FALSE);
			GetDlgItem(IDC_SCG_ACQ_BIT_ORDERING_9_10)->EnableWindow( FALSE);
			GetDlgItem(IDC_SCG_ACQ_BIT_ORDERING_MSB_10)->EnableWindow( FALSE);
		}
		else
		{
			BOOL multipleOptions = FALSE;

			//
			// Enable only the relevant options
			//
			if(( m_bitOrderingCap & CORACQ_VAL_BIT_ORDERING_9_10) && ( pixelDepth == 10))
			{
				GetDlgItem(IDC_SCG_ACQ_BIT_ORDERING_9_10)->EnableWindow( TRUE);

				multipleOptions = TRUE;
			}
			else
			{
				GetDlgItem(IDC_SCG_ACQ_BIT_ORDERING_9_10)->EnableWindow( FALSE);
			}

			if(( m_bitOrderingCap & CORACQ_VAL_BIT_ORDERING_MSB_10) && ( pixelDepth == 8))
			{
				GetDlgItem(IDC_SCG_ACQ_BIT_ORDERING_MSB_10)->EnableWindow( TRUE);

				multipleOptions = TRUE;
			}
			else
			{
				GetDlgItem(IDC_SCG_ACQ_BIT_ORDERING_MSB_10)->EnableWindow( FALSE);
			}

			if(( m_bitOrderingCap & CORACQ_VAL_BIT_ORDERING_STD) && multipleOptions)
			{
				GetDlgItem(IDC_SCG_ACQ_BIT_ORDERING_STD)->EnableWindow( TRUE);
			}
			else
			{
				GetDlgItem(IDC_SCG_ACQ_BIT_ORDERING_MSB_10)->EnableWindow( FALSE);
			}

			//
			// Select the current options
			//
			if( bitOrdering & CORACQ_VAL_BIT_ORDERING_9_10)
			{
				((CButton *)GetDlgItem(IDC_SCG_ACQ_BIT_ORDERING_9_10))->SetCheck(TRUE);

			}

			if( bitOrdering & CORACQ_VAL_BIT_ORDERING_MSB_10)
			{
				((CButton *)GetDlgItem(IDC_SCG_ACQ_BIT_ORDERING_MSB_10))->SetCheck(TRUE);
			}

			if( bitOrdering & CORACQ_VAL_BIT_ORDERING_STD)
			{
				((CButton *)GetDlgItem(IDC_SCG_ACQ_BIT_ORDERING_STD))->SetCheck(TRUE);

			}
		}
	}
	else
	{
		m_bitOrderingCap = CORACQ_VAL_BIT_ORDERING_STD;

		// Disable all items related to the bit ordering
		GetDlgItem(IDC_SCG_ACQ_BIT_ORDERING_STD)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_ACQ_BIT_ORDERING_9_10)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_ACQ_BIT_ORDERING_MSB_10)->EnableWindow( FALSE);
	}


	UpdateWindow();
	
	UpdateData( FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CAcqDlg::UpdateWindow() 
{
	// Initialize the External Trigger info
	if( m_extTrigEnable)
	{
		// Enable selection of the external trigger detection 
		GetDlgItem(IDC_SCG_ACQ_EXT_TRIGGER_RISING_EDGE)->EnableWindow( m_extTrigDetectionCap & CORACQ_VAL_RISING_EDGE ? TRUE:FALSE);
		GetDlgItem(IDC_SCG_ACQ_EXT_TRIGGER_FALLING_EDGE)->EnableWindow( m_extTrigDetectionCap & CORACQ_VAL_FALLING_EDGE ? TRUE:FALSE);
		GetDlgItem(IDC_SCG_ACQ_EXT_TRIGGER_ACTIVE_HIGH)->EnableWindow( m_extTrigDetectionCap & CORACQ_VAL_ACTIVE_HIGH ? TRUE:FALSE);
		GetDlgItem(IDC_SCG_ACQ_EXT_TRIGGER_ACTIVE_LOW)->EnableWindow( m_extTrigDetectionCap & CORACQ_VAL_ACTIVE_LOW ? TRUE:FALSE);
	}
	else
	{
		// Disable selection of the external trigger detection 
		GetDlgItem(IDC_SCG_ACQ_EXT_TRIGGER_RISING_EDGE)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_ACQ_EXT_TRIGGER_FALLING_EDGE)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_ACQ_EXT_TRIGGER_ACTIVE_HIGH)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_ACQ_EXT_TRIGGER_ACTIVE_LOW)->EnableWindow( FALSE);
	}
}

void CAcqDlg::OnExternalTriggerEnable() 
{
	UpdateData( TRUE);
	UpdateWindow();
}



