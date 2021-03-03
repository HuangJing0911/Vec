// LScanDlg.cpp : implementation file
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
// CLScanDlg dialog


CLScanDlg::CLScanDlg(CWnd* pParent, SapAcquisition *pAcq)
	: CDialog(CLScanDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLScanDlg)
	m_pixels = 0;
	m_extFrameTriggerEnable = FALSE;
	m_extLineTriggerEnable = FALSE;
	m_lineTriggerEnable = FALSE;
	m_lineIntegrateEnable = FALSE;
	m_IntLineTrigger = 0;
	m_intLineTriggerEnable = FALSE;
	m_linescanDirection = 0;
	m_shaftEncoderEnable = FALSE;
	//}}AFX_DATA_INIT
	m_pAcq = pAcq;
}


void CLScanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLScanDlg)
	DDX_Control(pDX, IDC_SCG_LS_SCROLLBAR_INTERNAL_LINE_TRIGGER, m_intLineTrigger_hz);
	DDX_Control(pDX, IDC_SCG_LS_SCROLLBAR_PIXELS, m_ScrollBar_Pixels);
	DDX_Text(pDX, IDC_SCG_LS_EDIT_LINE_INTEGRATION, m_pixels);
	DDX_Check(pDX, IDC_SCG_LS_EXTERNAL_FRAME_TRIGGER_ENABLE, m_extFrameTriggerEnable);
	DDX_Check(pDX, IDC_SCG_LS_EXTERNAL_LINE_TRIGGER_ENABLE, m_extLineTriggerEnable);
	DDX_Check(pDX, IDC_SCG_LS_LINE_TRIGGER_ENABLE, m_lineTriggerEnable);
	DDX_Text(pDX, IDC_SCG_LS_EDIT_INTERNAL_LINE_TRIGGER, m_IntLineTrigger);
	DDX_Check(pDX, IDC_SCG_LS_INTERNAL_LINE_TRIGGER_ENABLE, m_intLineTriggerEnable);
	DDX_Text(pDX, IDC_SCG_LS_LINESCAN_DIRECTION, m_linescanDirection);
	DDX_Check(pDX, IDC_SCG_LS_SHAFT_ENCODER_ENABLE, m_shaftEncoderEnable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLScanDlg, CDialog)
	//{{AFX_MSG_MAP(CLScanDlg)
	ON_EN_KILLFOCUS(IDC_SCG_LS_EDIT_LINE_INTEGRATION, OnKillfocusEditLineIntegration)
	ON_BN_CLICKED(IDC_SCG_LS_EXTERNAL_FRAME_TRIGGER_ENABLE, OnExternalFrameTriggerEnable)
	ON_BN_CLICKED(IDC_SCG_LS_EXTERNAL_LINE_TRIGGER_ENABLE, OnExternalLineTriggerEnable)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_SCG_LS_INTERNAL_LINE_TRIGGER_ENABLE, OnInternalLineTriggerEnable)
	ON_BN_CLICKED(IDC_SCG_LS_LINE_INTEGRATE_ENABLE, OnLineIntegrateEnable)
	ON_BN_CLICKED(IDC_SCG_LS_SHAFT_ENCODER_ENABLE, OnShaftEncoderEnable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLScanDlg message handlers
void CLScanDlg::OnOK() 
{
	// Get current setting
	UpdateData( TRUE);

	// Update Line Trigger PRMs
	if( m_lineTriggerCap)
	{
      if (!m_pAcq->SetParameter(CORACQ_PRM_LINE_TRIGGER_ENABLE, m_lineTriggerEnable ? TRUE : FALSE))
		{
			CDialog::OnCancel();
			return;
		}
	}


	// Update Line Integrate PRMs
	if( m_lineIntegrateCap)
	{
		VerifyLineIntegrateDuration();

      if (!m_pAcq->SetParameter(CORACQ_PRM_LINE_INTEGRATE_ENABLE, m_lineIntegrateEnable ? TRUE : FALSE))
		{
			CDialog::OnCancel();
			return;
		}

      if (!m_pAcq->SetParameter(CORACQ_PRM_LINE_INTEGRATE_DURATION, m_pixels))
		{
			CDialog::OnCancel();
			return;
		}
	}

	// Update External Frame Trigger PRMs
	if( m_extFrameTriggerCap)
	{
		int extFrameTriggerDetectionPrm;
		int extFrameTriggerLevelPrm;

      if (!m_pAcq->SetParameter(CORACQ_PRM_EXT_FRAME_TRIGGER_ENABLE, m_extFrameTriggerEnable ? TRUE : FALSE))
		{
			CDialog::OnCancel();
			return;
		}

		if( ((CButton *)GetDlgItem(IDC_SCG_LS_EXT_FRAME_TRIGGER_RISING_EDGE))->GetCheck())
		{
			extFrameTriggerDetectionPrm = CORACQ_VAL_RISING_EDGE;
		}
		else
		{
			extFrameTriggerDetectionPrm = CORACQ_VAL_FALLING_EDGE;
		}

		if (m_extFrameTriggerEnable)
      {
         if (!m_pAcq->SetParameter(CORACQ_PRM_EXT_FRAME_TRIGGER_DETECTION, extFrameTriggerDetectionPrm))
			{
				CDialog::OnCancel();
				return;
			}
      }

		if( ((CButton *)GetDlgItem(IDC_SCG_LS_EXT_FRAME_TRIGGER_LEVEL_TTL))->GetCheck())
		{
			extFrameTriggerLevelPrm = CORACQ_VAL_LEVEL_TTL;
		}
		else
		{
			extFrameTriggerLevelPrm = CORACQ_VAL_LEVEL_422;
		}

		if (m_extFrameTriggerEnable)
      {
         if (!m_pAcq->SetParameter(CORACQ_PRM_EXT_FRAME_TRIGGER_LEVEL, extFrameTriggerLevelPrm))
			{
				CDialog::OnCancel();
				return;
			}
      }
	}

	// Update External Line Trigger PRMs
	if( m_extLineTriggerCap)
	{
		int extLineTriggerDetectionPrm;
		int extLineTriggerLevelPrm;

		// external line trigger and internal line trigger are mutualy exclusif
		if (m_extLineTriggerEnable)
		{
			// ext trigger is enable then desable int line trigger
         if (!m_pAcq->SetParameter(CORACQ_PRM_INT_LINE_TRIGGER_ENABLE, m_intLineTriggerEnable=FALSE))
			{
				CDialog::OnCancel();
				return;
			}
         if (!m_pAcq->SetParameter(CORACQ_PRM_SHAFT_ENCODER_ENABLE, m_shaftEncoderEnable=FALSE))
			{
				CDialog::OnCancel();
				return;
			}
		}

      if (!m_pAcq->SetParameter(CORACQ_PRM_EXT_LINE_TRIGGER_ENABLE, m_extLineTriggerEnable ? TRUE : FALSE))
		{
			CDialog::OnCancel();
			return;
		}

		if( ((CButton *)GetDlgItem(IDC_SCG_LS_EXT_LINE_TRIGGER_RISING_EDGE))->GetCheck())
		{
			extLineTriggerDetectionPrm = CORACQ_VAL_RISING_EDGE;
		}
		else
		{
			extLineTriggerDetectionPrm = CORACQ_VAL_FALLING_EDGE;
		}

		if (m_extLineTriggerEnable)
      {
         if (!m_pAcq->SetParameter(CORACQ_PRM_EXT_LINE_TRIGGER_DETECTION, extLineTriggerDetectionPrm))
		   {
			   CDialog::OnCancel();
			   return;
		   }
      }

		if( ((CButton *)GetDlgItem(IDC_SCG_LS_EXT_LINE_TRIGGER_LEVEL_TTL))->GetCheck())
		{
			extLineTriggerLevelPrm = CORACQ_VAL_LEVEL_TTL;
		}
		else
		{
			extLineTriggerLevelPrm = CORACQ_VAL_LEVEL_422;
		}

		if (m_extLineTriggerEnable)
      {
         if (!m_pAcq->SetParameter(CORACQ_PRM_EXT_LINE_TRIGGER_LEVEL, extLineTriggerLevelPrm))
			{
				CDialog::OnCancel();
				return;
			}
      }
	}

	// Update Internal Line Trigger PRMs
	if( m_intlineTriggerCap)
	{
		if (m_intLineTriggerEnable)
		{
         if (!m_pAcq->SetParameter(CORACQ_PRM_INT_LINE_TRIGGER_FREQ, m_IntLineTrigger))
			{
				CDialog::OnCancel();
				return;
			}
		}

		// external line trigger and internal line trigger are mutualy exclusif
		if (m_intLineTriggerEnable)
		{
			// int trigger is enable then desable ext line trigger and shaft encoder
         if (!m_pAcq->SetParameter(CORACQ_PRM_EXT_LINE_TRIGGER_ENABLE, m_extLineTriggerEnable = FALSE))
			{
				CDialog::OnCancel();
				return;
			}
         if (!m_pAcq->SetParameter(CORACQ_PRM_SHAFT_ENCODER_ENABLE, m_shaftEncoderEnable =  FALSE))
			{
				CDialog::OnCancel();
				return;
			}
		}

      if (!m_pAcq->SetParameter(CORACQ_PRM_INT_LINE_TRIGGER_ENABLE, m_intLineTriggerEnable ? TRUE : FALSE))
		{
			CDialog::OnCancel();
			return;
		}

	}

	// Update Internal Linescan Direction PRM
	if (m_linescanDirectionCap)
	{
		if( ((CButton *)GetDlgItem(IDC_SCG_LS_LINESCAN_DIRECTION_FORWARD))->GetCheck())
		{
			m_linescanDirection = CORACQ_VAL_LINESCAN_DIRECTION_FORWARD;
		}
		else
		{
			m_linescanDirection = CORACQ_VAL_LINESCAN_DIRECTION_REVERSE;
		}

      if (!m_pAcq->SetParameter(CORACQ_PRM_LINESCAN_DIRECTION_OUTPUT, m_linescanDirection))
		{
			CDialog::OnCancel();
			return;
		}
	}


	// Update Shaft Encoder PRMs
	if( m_shaftEncoderCap)
	{
		// shaft encoder, external line trigger and internal line trigger are mutualy exclusif
		if (m_shaftEncoderEnable)
		{
			// if shaft encoder is enable then desable ext. line trigger and int. line trigger
         if (!m_pAcq->SetParameter(CORACQ_PRM_EXT_LINE_TRIGGER_ENABLE, m_extLineTriggerEnable = FALSE))
			{
				CDialog::OnCancel();
				return;
			}

         if (!m_pAcq->SetParameter(CORACQ_PRM_INT_LINE_TRIGGER_ENABLE, m_intLineTriggerEnable = FALSE))
			{
				CDialog::OnCancel();
				return;
			}
		}

      if (!m_pAcq->SetParameter(CORACQ_PRM_SHAFT_ENCODER_ENABLE, m_shaftEncoderEnable))
		{
			CDialog::OnCancel();
			return;
		}
	}

	CDialog::OnOK();
}


BOOL CLScanDlg::OnInitDialog() 
{
	int lineTriggerMethodPrm;
	int lineIntegrateMethodPrm;

	CDialog::OnInitDialog();

	// Verifie acquisition object
	if (m_pAcq == NULL)
	{
		AfxMessageBox("Invalid acquisition object");
		CDialog::OnCancel();
		return FALSE;
	}

	// Initialize Line Trigger feature
   if (!m_pAcq->GetCapability(CORACQ_CAP_LINE_TRIGGER, &m_lineTriggerCap))
	{
		CDialog::OnCancel();
		return FALSE;
	}

   if (!m_pAcq->GetParameter(CORACQ_PRM_LINE_TRIGGER_METHOD, &lineTriggerMethodPrm))
	{
		CDialog::OnCancel();
		return FALSE;
	}

	if( m_lineTriggerCap && lineTriggerMethodPrm)
	{
		int lineTriggerEnable;

      if (!m_pAcq->GetParameter(CORACQ_PRM_LINE_TRIGGER_ENABLE, &lineTriggerEnable))
		{
			CDialog::OnCancel();
			return FALSE;
		}

		if( lineTriggerEnable)
		{
			m_lineTriggerEnable = TRUE;
		}
	}
	else
	{
     	GetDlgItem(IDC_SCG_LS_LINE_TRIGGER_ENABLE)->EnableWindow(FALSE);
	}

	
	// Initialize Line Integrate feature
   if (!m_pAcq->GetCapability(CORACQ_CAP_LINE_INTEGRATE, &m_lineIntegrateCap))
	{
		CDialog::OnCancel();
		return FALSE;
	}

   if (!m_pAcq->GetParameter(CORACQ_PRM_LINE_INTEGRATE_METHOD, &lineIntegrateMethodPrm))
	{
		CDialog::OnCancel();
		return FALSE;
	}

	if( m_lineIntegrateCap /*&& lineIntegrateMethodPrm*/)
	{
		int lineIntegrateEnable;

      if (!m_pAcq->GetParameter(CORACQ_PRM_LINE_INTEGRATE_ENABLE, &lineIntegrateEnable))
		{
			CDialog::OnCancel();
			return FALSE;
		}

		if( lineIntegrateEnable)
		{
			m_lineIntegrateEnable = TRUE;
			((CButton *) GetDlgItem(IDC_SCG_LS_LINE_INTEGRATE_ENABLE))->SetCheck(TRUE);
		}

      if (!m_pAcq->GetCapability(CORACQ_CAP_LINE_INTEGRATE_DURATION_MIN, &m_lineIntegrateDurationMinCap))
		{
			CDialog::OnCancel();
			return FALSE;
		}

      if (!m_pAcq->GetCapability(CORACQ_CAP_LINE_INTEGRATE_DURATION_MAX, &m_lineIntegrateDurationMaxCap))
		{
			CDialog::OnCancel();
			return FALSE;
		}

      if (!m_pAcq->GetParameter(CORACQ_PRM_LINE_INTEGRATE_DURATION, &m_pixels))
		{
			CDialog::OnCancel();
			return FALSE;
		}
	}
	else
	{
		GetDlgItem(IDC_SCG_LS_LINE_INTEGRATE_ENABLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SCG_LS_EDIT_LINE_INTEGRATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_SCG_LS_SCROLLBAR_PIXELS)->EnableWindow(FALSE);
	}


	// Initialize the External Frame Trigger feature
   if (!m_pAcq->GetCapability(CORACQ_CAP_EXT_FRAME_TRIGGER, &m_extFrameTriggerCap))
	{
		CDialog::OnCancel();
		return FALSE;
	}

	if( m_extFrameTriggerCap)
	{
		int extFrameTriggerDetectionPrm;
		int extFrameTriggerEnable;
		int extFrameTriggerLevelPrm;

      if (!m_pAcq->GetParameter(CORACQ_PRM_EXT_FRAME_TRIGGER_ENABLE, &extFrameTriggerEnable))
		{
			CDialog::OnCancel();
			return FALSE;
		}

		if( extFrameTriggerEnable)
		{
			m_extFrameTriggerEnable = TRUE;
		}


      if (!m_pAcq->GetCapability(CORACQ_CAP_EXT_FRAME_TRIGGER_DETECTION, &m_extFrameTriggerDetectionCap))
		{
			CDialog::OnCancel();
			return FALSE;
		}

      if (!m_pAcq->GetParameter(CORACQ_PRM_EXT_FRAME_TRIGGER_DETECTION, &extFrameTriggerDetectionPrm))
		{
			CDialog::OnCancel();
			return FALSE;
		}

      if (!m_pAcq->GetCapability(CORACQ_CAP_EXT_FRAME_TRIGGER_LEVEL, &m_extFrameTriggerLevelCap))
		{
			CDialog::OnCancel();
			return FALSE;
		}

      if (!m_pAcq->GetParameter(CORACQ_PRM_EXT_FRAME_TRIGGER_LEVEL, &extFrameTriggerLevelPrm))
		{
			CDialog::OnCancel();
			return FALSE;
		}



		// Check the current state of the external frame detection
		if( extFrameTriggerDetectionPrm & CORACQ_VAL_RISING_EDGE)
		{
			((CButton *)GetDlgItem(IDC_SCG_LS_EXT_FRAME_TRIGGER_RISING_EDGE))->SetCheck(TRUE);
		}
		else
		{
			((CButton *) GetDlgItem(IDC_SCG_LS_EXT_FRAME_TRIGGER_FALLING_EDGE))->SetCheck(TRUE);
		}

		// Check the current state of the external frame level
		if( extFrameTriggerLevelPrm & CORACQ_VAL_LEVEL_TTL)
		{
			((CButton *)GetDlgItem(IDC_SCG_LS_EXT_FRAME_TRIGGER_LEVEL_TTL))->SetCheck(TRUE);
		}
		else
		{
			((CButton *) GetDlgItem(IDC_SCG_LS_EXT_FRAME_TRIGGER_LEVEL_RS422))->SetCheck(TRUE);
		}

	}
	else
	{
		GetDlgItem(IDC_SCG_LS_EXT_FRAME_TRIGGER_RISING_EDGE)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_LS_EXT_FRAME_TRIGGER_FALLING_EDGE)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_LS_EXT_FRAME_TRIGGER_LEVEL_TTL)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_LS_EXT_FRAME_TRIGGER_LEVEL_RS422)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_LS_EXTERNAL_FRAME_TRIGGER_ENABLE)->EnableWindow(FALSE);
	}

	// Initialize the External Line Trigger feature
   if (!m_pAcq->GetCapability(CORACQ_CAP_EXT_LINE_TRIGGER, &m_extLineTriggerCap))
	{
		CDialog::OnCancel();
		return FALSE;
	}
		
	if( m_extLineTriggerCap)
	{
		int extLineTriggerDetectionPrm;
		int extLineTriggerEnable;
		int extLineTriggerLevelPrm;

      if (!m_pAcq->GetParameter(CORACQ_PRM_EXT_LINE_TRIGGER_ENABLE, &extLineTriggerEnable))
		{
			CDialog::OnCancel();
			return FALSE;
		}

		if( extLineTriggerEnable)
		{
			m_extLineTriggerEnable = TRUE;
		}

      if (!m_pAcq->GetCapability(CORACQ_CAP_EXT_LINE_TRIGGER_DETECTION, &m_extLineTriggerDetectionCap))
		{
			CDialog::OnCancel();
			return FALSE;
		}

      if (!m_pAcq->GetParameter(CORACQ_PRM_EXT_LINE_TRIGGER_DETECTION, &extLineTriggerDetectionPrm))
		{
			CDialog::OnCancel();
			return FALSE;
		}

      if (!m_pAcq->GetCapability(CORACQ_CAP_EXT_LINE_TRIGGER_LEVEL, &m_extLineTriggerLevelCap))
		{
			CDialog::OnCancel();
			return FALSE;
		}

      if (!m_pAcq->GetParameter(CORACQ_PRM_EXT_LINE_TRIGGER_LEVEL, &extLineTriggerLevelPrm))
		{
			CDialog::OnCancel();
			return FALSE;
		}

		// Check the current state of the external line detection
      if( extLineTriggerDetectionPrm & CORACQ_VAL_RISING_EDGE)
		{
			((CButton *) GetDlgItem(IDC_SCG_LS_EXT_LINE_TRIGGER_RISING_EDGE))->SetCheck(TRUE);
		}
		else
		{
			((CButton *) GetDlgItem(IDC_SCG_LS_EXT_LINE_TRIGGER_FALLING_EDGE))->SetCheck(TRUE);
		}

		// Check the current state of the external line level
		if( extLineTriggerLevelPrm & CORACQ_VAL_LEVEL_TTL)
		{
			((CButton *)GetDlgItem(IDC_SCG_LS_EXT_LINE_TRIGGER_LEVEL_TTL))->SetCheck(TRUE);
		}
		else
		{
			((CButton *) GetDlgItem(IDC_SCG_LS_EXT_LINE_TRIGGER_LEVEL_RS422))->SetCheck(TRUE);
		}
	}
	else
	{
		GetDlgItem(IDC_SCG_LS_EXT_LINE_TRIGGER_RISING_EDGE)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_LS_EXT_LINE_TRIGGER_FALLING_EDGE)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_LS_EXT_LINE_TRIGGER_LEVEL_TTL)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_LS_EXT_LINE_TRIGGER_LEVEL_RS422)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_LS_EXTERNAL_LINE_TRIGGER_ENABLE)->EnableWindow(FALSE);
	}


	// Initialize the Internal Line Trigger feature
   if (!m_pAcq->GetCapability(CORACQ_CAP_INT_LINE_TRIGGER, &m_intlineTriggerCap))
	{
		CDialog::OnCancel();
		return FALSE;
	}

	if( m_intlineTriggerCap)
	{
		int intLineTriggerFreq=0;

      if (!m_pAcq->GetParameter(CORACQ_PRM_INT_LINE_TRIGGER_ENABLE, &m_intLineTriggerEnable))
		{
			CDialog::OnCancel();
			return FALSE;
		}

		((CButton *)GetDlgItem(IDC_SCG_LS_INTERNAL_LINE_TRIGGER_ENABLE))->SetCheck(m_intLineTriggerEnable);
      if (!m_pAcq->GetParameter(CORACQ_PRM_INT_LINE_TRIGGER_FREQ, &intLineTriggerFreq))
		{
			CDialog::OnCancel();
			return FALSE;
		}

		m_IntLineTrigger = intLineTriggerFreq;

		GetDlgItem(IDC_SCG_LS_EDIT_INTERNAL_LINE_TRIGGER)->EnableWindow( m_intLineTriggerEnable);
	}
	else
	{
		GetDlgItem(IDC_SCG_LS_SCROLLBAR_INTERNAL_LINE_TRIGGER)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_LS_EDIT_INTERNAL_LINE_TRIGGER)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_LS_INTERNAL_LINE_TRIGGER_ENABLE)->EnableWindow(FALSE);
	}

	// Initialize the Linescan Direction feature
   if (!m_pAcq->GetCapability(CORACQ_CAP_LINESCAN_DIRECTION, &m_linescanDirectionCap))
	{
		CDialog::OnCancel();
		return FALSE;
	}


	if( m_linescanDirectionCap)
	{
		int linescanDirectionOutput=0;
		int linescanDirection=0;

      if (!m_pAcq->GetParameter(CORACQ_PRM_LINESCAN_DIRECTION, &linescanDirection))
		{
			CDialog::OnCancel();
			return FALSE;
		}

      if (!m_pAcq->GetParameter(CORACQ_PRM_LINESCAN_DIRECTION_OUTPUT, &linescanDirectionOutput))
		{
			CDialog::OnCancel();
			return FALSE;
		}

		if (!(m_linescanDirectionCap & linescanDirection & CORACQ_VAL_LINESCAN_DIRECTION_FORWARD))
		{
			((CButton *)GetDlgItem(IDC_SCG_LS_LINESCAN_DIRECTION_REVERSE))->SetCheck( TRUE);
			((CButton *)GetDlgItem(IDC_SCG_LS_LINESCAN_DIRECTION_FORWARD))->SetCheck( FALSE);
			GetDlgItem(IDC_SCG_LS_LINESCAN_DIRECTION_FORWARD)->EnableWindow( FALSE);
		}

		if (!(m_linescanDirectionCap & linescanDirection & CORACQ_VAL_LINESCAN_DIRECTION_REVERSE))
		{
			((CButton *)GetDlgItem(IDC_SCG_LS_LINESCAN_DIRECTION_REVERSE))->SetCheck( FALSE);
			((CButton *)GetDlgItem(IDC_SCG_LS_LINESCAN_DIRECTION_FORWARD))->SetCheck( TRUE);
			GetDlgItem(IDC_SCG_LS_LINESCAN_DIRECTION_REVERSE)->EnableWindow( FALSE);
		}


		if (linescanDirectionOutput & CORACQ_VAL_LINESCAN_DIRECTION_FORWARD)
		{
			((CButton *) GetDlgItem(IDC_SCG_LS_LINESCAN_DIRECTION_FORWARD))->SetCheck(TRUE);
		}
		else if (linescanDirectionOutput & CORACQ_VAL_LINESCAN_DIRECTION_REVERSE)
		{
			((CButton *) GetDlgItem(IDC_SCG_LS_LINESCAN_DIRECTION_REVERSE))->SetCheck(TRUE);
		}

		m_linescanDirection = linescanDirectionOutput; 

	}
	else
	{
		GetDlgItem(IDC_SCG_LS_LINESCAN_DIRECTION_FORWARD)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_LS_LINESCAN_DIRECTION_REVERSE)->EnableWindow( FALSE);
	}

	
	// Initialize the Shaft Encoder feature
   if (!m_pAcq->GetCapability(CORACQ_CAP_SHAFT_ENCODER_ENABLE, &m_shaftEncoderCap))
	{
		CDialog::OnCancel();
		return FALSE;
	}
		
	if(m_shaftEncoderCap)
	{
		int shaftEncoderEnable;

      if (!m_pAcq->GetParameter(CORACQ_PRM_SHAFT_ENCODER_ENABLE, &shaftEncoderEnable))
		{
			CDialog::OnCancel();
			return FALSE;
		}

		if( shaftEncoderEnable)
		{
			m_shaftEncoderEnable = TRUE;
		}
	}
	else
	{
		GetDlgItem(IDC_SCG_LS_SHAFT_ENCODER_ENABLE)->EnableWindow(FALSE);
	}

	UpdateData( FALSE);
	UpdateWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CLScanDlg::UpdateWindow() 
{
	// Enable/Disable the Line Integrate feature
	GetDlgItem(IDC_SCG_LS_EDIT_LINE_INTEGRATION)->EnableWindow( m_lineIntegrateEnable ? TRUE : FALSE);
	GetDlgItem(IDC_SCG_LS_SCROLLBAR_PIXELS)->EnableWindow( m_lineIntegrateEnable ? TRUE : FALSE);


	// Enable/Disable the External Frame Trigger feature
	if( m_extFrameTriggerEnable)
	{
		GetDlgItem(IDC_SCG_LS_EXT_FRAME_TRIGGER_RISING_EDGE )->EnableWindow( m_extFrameTriggerDetectionCap & CORACQ_VAL_RISING_EDGE ? TRUE:FALSE);
		GetDlgItem(IDC_SCG_LS_EXT_FRAME_TRIGGER_FALLING_EDGE)->EnableWindow( m_extFrameTriggerDetectionCap & CORACQ_VAL_FALLING_EDGE ? TRUE:FALSE);
	}
	else
	{
		GetDlgItem(IDC_SCG_LS_EXT_FRAME_TRIGGER_RISING_EDGE)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_LS_EXT_FRAME_TRIGGER_FALLING_EDGE)->EnableWindow( FALSE);
	}

	if ((m_extFrameTriggerLevelCap)&&(m_extFrameTriggerEnable))
	{
		GetDlgItem(IDC_SCG_LS_EXT_FRAME_TRIGGER_LEVEL_TTL   )->EnableWindow( m_extFrameTriggerLevelCap & CORACQ_VAL_LEVEL_TTL ? TRUE:FALSE);
		GetDlgItem(IDC_SCG_LS_EXT_FRAME_TRIGGER_LEVEL_RS422 )->EnableWindow( m_extFrameTriggerLevelCap & CORACQ_VAL_LEVEL_422 ? TRUE:FALSE);
	}
	else
	{
		GetDlgItem(IDC_SCG_LS_EXT_FRAME_TRIGGER_LEVEL_TTL   )->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_LS_EXT_FRAME_TRIGGER_LEVEL_RS422 )->EnableWindow( FALSE);
	}


	// enable the 3 line trigger if avalable
	GetDlgItem(IDC_SCG_LS_SHAFT_ENCODER_ENABLE)->EnableWindow(m_shaftEncoderCap);
	GetDlgItem(IDC_SCG_LS_INTERNAL_LINE_TRIGGER_ENABLE)->EnableWindow(m_intlineTriggerCap);
	GetDlgItem(IDC_SCG_LS_EXTERNAL_LINE_TRIGGER_ENABLE)->EnableWindow(m_extLineTriggerCap);

	// Enable/Disable the External Line Trigger feature
	if( m_extLineTriggerEnable)
	{
		GetDlgItem(IDC_SCG_LS_EXT_LINE_TRIGGER_RISING_EDGE)->EnableWindow( m_extLineTriggerDetectionCap & CORACQ_VAL_RISING_EDGE ? TRUE:FALSE);
		GetDlgItem(IDC_SCG_LS_EXT_LINE_TRIGGER_FALLING_EDGE)->EnableWindow( m_extLineTriggerDetectionCap & CORACQ_VAL_FALLING_EDGE ? TRUE:FALSE);

		GetDlgItem(IDC_SCG_LS_SHAFT_ENCODER_ENABLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SCG_LS_INTERNAL_LINE_TRIGGER_ENABLE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_SCG_LS_EXT_LINE_TRIGGER_RISING_EDGE)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_LS_EXT_LINE_TRIGGER_FALLING_EDGE)->EnableWindow( FALSE);
	}

	if ((m_extLineTriggerLevelCap)&&(m_extLineTriggerEnable))
	{
		GetDlgItem(IDC_SCG_LS_EXT_LINE_TRIGGER_LEVEL_TTL)->EnableWindow( m_extLineTriggerLevelCap & CORACQ_VAL_LEVEL_TTL ? TRUE:FALSE);
		GetDlgItem(IDC_SCG_LS_EXT_LINE_TRIGGER_LEVEL_RS422)->EnableWindow( m_extLineTriggerLevelCap & CORACQ_VAL_LEVEL_422 ? TRUE:FALSE);
	}
	else
	{
		GetDlgItem(IDC_SCG_LS_EXT_LINE_TRIGGER_LEVEL_TTL)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_LS_EXT_LINE_TRIGGER_LEVEL_RS422)->EnableWindow( FALSE);
	}

	// Enable/Disable the Int Line Trigger feature
	if( m_intLineTriggerEnable)
	{
		GetDlgItem(IDC_SCG_LS_SHAFT_ENCODER_ENABLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SCG_LS_EXTERNAL_LINE_TRIGGER_ENABLE)->EnableWindow(FALSE);
	}

	// Enable/Disable the Shaft Encoder feature
	if( m_shaftEncoderEnable)
	{
		GetDlgItem(IDC_SCG_LS_EXTERNAL_LINE_TRIGGER_ENABLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SCG_LS_INTERNAL_LINE_TRIGGER_ENABLE)->EnableWindow(FALSE);
	}
}


void CLScanDlg::OnKillfocusEditLineIntegration() 
{
	UpdateData(TRUE);
	VerifyLineIntegrateDuration();
	UpdateWindow();
}


void CLScanDlg::OnExternalFrameTriggerEnable() 
{
	BOOL bCheck;

	bCheck= ((CButton *)GetDlgItem(IDC_SCG_LS_EXTERNAL_FRAME_TRIGGER_ENABLE))->GetCheck();

	GetDlgItem(IDC_SCG_LS_EXT_FRAME_TRIGGER_RISING_EDGE)->EnableWindow( bCheck);
	GetDlgItem(IDC_SCG_LS_EXT_FRAME_TRIGGER_FALLING_EDGE)->EnableWindow( bCheck);
	GetDlgItem(IDC_SCG_LS_EXT_FRAME_TRIGGER_LEVEL_TTL)->EnableWindow( bCheck);
	GetDlgItem(IDC_SCG_LS_EXT_FRAME_TRIGGER_LEVEL_RS422)->EnableWindow( bCheck);

	UpdateData( TRUE);
	UpdateWindow();
}

void CLScanDlg::OnExternalLineTriggerEnable() 
{
	BOOL bCheck;

	bCheck= ((CButton *)GetDlgItem(IDC_SCG_LS_EXTERNAL_LINE_TRIGGER_ENABLE))->GetCheck();
	UpdateData( TRUE);

	((CButton *)GetDlgItem(IDC_SCG_LS_EXTERNAL_LINE_TRIGGER_ENABLE))->SetCheck(bCheck);
	GetDlgItem(IDC_SCG_LS_EXT_LINE_TRIGGER_RISING_EDGE)->EnableWindow( bCheck);
	GetDlgItem(IDC_SCG_LS_EXT_LINE_TRIGGER_FALLING_EDGE)->EnableWindow( bCheck);
	GetDlgItem(IDC_SCG_LS_EXT_LINE_TRIGGER_LEVEL_TTL)->EnableWindow( bCheck);
	GetDlgItem(IDC_SCG_LS_EXT_LINE_TRIGGER_LEVEL_RS422)->EnableWindow( bCheck);

	UpdateWindow();
}

void CLScanDlg::OnLineIntegrate() 
{
	UpdateWindow();
}


void CLScanDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);

	if( (CScrollBar *)pScrollBar == &m_ScrollBar_Pixels)
	{
		switch( nSBCode)
		{

			case SB_LINEDOWN:
				if( m_pixels >= 11)
				{
					m_pixels -= 10;
				}
				else 
				{
					m_pixels = 1;
				}
				break;
			case SB_LINEUP:
				m_pixels += 10;
				break;
			default:
				return;
		}

		VerifyLineIntegrateDuration();
	}

	if( (CScrollBar*)pScrollBar == &m_intLineTrigger_hz)
	{
		if (((CButton *)GetDlgItem(IDC_SCG_LS_INTERNAL_LINE_TRIGGER_ENABLE))->GetCheck())
		switch( nSBCode)
		{

			case SB_LINEDOWN:
				if( m_IntLineTrigger >= 11)
				{
					m_IntLineTrigger -= 10;
				}
				else 
				{
					m_IntLineTrigger = 1;
				}
				break;
			case SB_LINEUP:
				m_IntLineTrigger += 10;
				break;
			default:
				return;
		}
		UpdateData( FALSE);
	}
}


void CLScanDlg::VerifyLineIntegrateDuration()
{
   if (!m_lineIntegrateEnable)
   {
      return;
   }

	BOOL bAdjusted = FALSE;

	if( m_pixels >= m_lineIntegrateDurationMaxCap)
	{
		bAdjusted = TRUE;
		m_pixels = m_lineIntegrateDurationMaxCap;
	}

	if( m_pixels < m_lineIntegrateDurationMinCap)
	{
		bAdjusted = TRUE;
		m_pixels = m_lineIntegrateDurationMinCap;
	}

	UpdateData( FALSE);

	if( bAdjusted)
	{
		char msg[100];

		CorSnprintf( msg, sizeof(msg), "Line integration duration out of range. Value set to: %d\n", m_pixels);
		AfxMessageBox( msg);

		UpdateWindow();
	}
}


void CLScanDlg::OnInternalLineTriggerEnable() 
{
	BOOL bCheck;
	bCheck = ((CButton *)GetDlgItem(IDC_SCG_LS_INTERNAL_LINE_TRIGGER_ENABLE))->GetCheck();
	UpdateData( TRUE);

	m_intLineTriggerEnable = bCheck;
	((CButton *)GetDlgItem(IDC_SCG_LS_INTERNAL_LINE_TRIGGER_ENABLE))->SetCheck(bCheck);
	GetDlgItem(IDC_SCG_LS_EDIT_INTERNAL_LINE_TRIGGER)->EnableWindow( bCheck);

	UpdateWindow();
}

void CLScanDlg::OnLineIntegrateEnable() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck;
	bCheck = ((CButton *)GetDlgItem(IDC_SCG_LS_LINE_INTEGRATE_ENABLE))->GetCheck();

	GetDlgItem(IDC_SCG_LS_EDIT_LINE_INTEGRATION)->EnableWindow( bCheck);
	m_lineIntegrateEnable = bCheck;

	UpdateData( FALSE);
	UpdateWindow();
	
}

void CLScanDlg::OnShaftEncoderEnable() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck;
	bCheck = ((CButton *)GetDlgItem(IDC_SCG_LS_SHAFT_ENCODER_ENABLE))->GetCheck();

	((CButton *)GetDlgItem(IDC_SCG_LS_SHAFT_ENCODER_ENABLE))->SetCheck(bCheck);
	m_shaftEncoderEnable = bCheck;
	
	UpdateData( FALSE);
	UpdateWindow();
}
