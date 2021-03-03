// AScanDlg.cpp : implementation file
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
// CAScanDlg dialog


CAScanDlg::CAScanDlg(CWnd* pParent, SapAcquisition *pAcq)
	: CDialog(CAScanDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAScanDlg)
	m_frame = 0;
	m_time = 0;
	//}}AFX_DATA_INIT
	m_pAcq = pAcq;
}


void CAScanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAScanDlg)
	DDX_Control(pDX, IDC_SCG_AS_SCROLLBAR_TIME, m_ScrollBar_Time);
	DDX_Control(pDX, IDC_SCG_AS_SCROLLBAR_FRAME, m_ScrollBar_Frame);
	DDX_Text(pDX, IDC_SCG_AS_EDIT_FRAME_INTEGRATE, m_frame);
	DDX_Text(pDX, IDC_SCG_AS_EDIT_TIME_INTEGRATE, m_time);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAScanDlg, CDialog)
	//{{AFX_MSG_MAP(CAScanDlg)
	ON_EN_KILLFOCUS(IDC_SCG_AS_EDIT_FRAME_INTEGRATE, OnKillfocusEditFrame)
	ON_EN_KILLFOCUS(IDC_SCG_AS_EDIT_TIME_INTEGRATE, OnKillfocusEditTime)
	ON_BN_CLICKED(IDC_SCG_AS_FRAME_INTEGRATE_ENABLE, OnFrameIntegrate)
	ON_BN_CLICKED(IDC_SCG_AS_TIME_INTEGRATE_ENABLE, OnTimeIntegrate)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_SCG_AS_CAM_TRIGGER_ENABLE, OnCamTriggerEnable)
	ON_BN_CLICKED(IDC_SCG_AS_CAM_RESET_ENABLE, OnCamResetEnable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAScanDlg message handlers
void CAScanDlg::OnOK() 
{
	BOOL camTriggerEnable;
	BOOL camResetEnable;
	BOOL timeIntegrateEnable;
	BOOL frameIntegrateEnable;
	int masterMode;

	UpdateData( TRUE);

	camTriggerEnable = ((CButton *)GetDlgItem(IDC_SCG_AS_CAM_TRIGGER_ENABLE))->GetCheck();
	camResetEnable = ((CButton *)GetDlgItem(IDC_SCG_AS_CAM_RESET_ENABLE))->GetCheck();
	timeIntegrateEnable = ((CButton *)GetDlgItem(IDC_SCG_AS_TIME_INTEGRATE_ENABLE))->GetCheck();
	frameIntegrateEnable = ((CButton *)GetDlgItem(IDC_SCG_AS_FRAME_INTEGRATE_ENABLE))->GetCheck();

	// Since the Camera Controls (trigger/reset/integrate) can be mutually exclusive, disable them all first
	if( m_camTriggerCap)
	{
      if (!m_pAcq->SetParameter(CORACQ_PRM_CAM_TRIGGER_ENABLE, (int) FALSE))
		{
			CDialog::OnCancel();
			return;
		}
	}

	if( m_camResetCap)
	{
      if (!m_pAcq->SetParameter(CORACQ_PRM_CAM_RESET_ENABLE, (int) FALSE))
		{
			CDialog::OnCancel();
			return;
		}
	}

	if( m_timeIntegrateCap)
	{
      if (!m_pAcq->SetParameter(CORACQ_PRM_TIME_INTEGRATE_ENABLE, (int) FALSE))
		{
			CDialog::OnCancel();
			return;
		}
	}

	if( m_frameIntegrateCap)
	{
      if (!m_pAcq->SetParameter(CORACQ_PRM_FRAME_INTEGRATE_ENABLE, (int) FALSE))
		{
			CDialog::OnCancel();
			return;
		}
	}

	// Enable the current selection
	if( camTriggerEnable)
	{
      if (!m_pAcq->SetParameter(CORACQ_PRM_CAM_TRIGGER_ENABLE, TRUE))
		{
			CDialog::OnCancel();
			return;
		}
	}

	if( camResetEnable)
	{
      if (!m_pAcq->SetParameter(CORACQ_PRM_CAM_RESET_ENABLE, TRUE))
		{
			CDialog::OnCancel();
			return;
		}
	}

	if( m_timeIntegrateCap)
	{
		if( timeIntegrateEnable)
		{
			VerifyTimeIntegrateDuration( TRUE);

			// Set time integration to a valid value before enabling the feature
         if (!m_pAcq->SetParameter(CORACQ_PRM_TIME_INTEGRATE_DURATION, m_time))
			{
				CDialog::OnCancel();
				return;
			}

         if (!m_pAcq->SetParameter(CORACQ_PRM_TIME_INTEGRATE_ENABLE, TRUE))
			{
				CDialog::OnCancel();
				return;
			}
		}
	}

	if( m_frameIntegrateCap)
	{
		if( frameIntegrateEnable)
		{
			VerifyFrameIntegrateCount();

			// Set frame integration count to a valid value before enabling the feature
         if (!m_pAcq->SetParameter(CORACQ_PRM_FRAME_INTEGRATE_COUNT, m_frame))
			{
				CDialog::OnCancel();
				return;
			}

         if (!m_pAcq->SetParameter(CORACQ_PRM_FRAME_INTEGRATE_ENABLE, TRUE))
			{
				CDialog::OnCancel();
				return;
			}
		}
 	}

	// Take care of the master mode selection only if it can be changed
	if( m_masterModeCap != CORACQ_VAL_MASTER_MODE_DISABLE)
	{
		if( ((CButton *)GetDlgItem(IDC_SCG_AS_MASTER_MODE_HS))->GetCheck() && ((CButton *)GetDlgItem(IDC_SCG_AS_MASTER_MODE_VS))->GetCheck())
		{
			masterMode = CORACQ_VAL_MASTER_MODE_HSYNC_VSYNC;
		}
		else if( ((CButton *)GetDlgItem(IDC_SCG_AS_MASTER_MODE_HS))->GetCheck())

		{
			masterMode = CORACQ_VAL_MASTER_MODE_HSYNC;
		}
		else if( ((CButton *)GetDlgItem(IDC_SCG_AS_MASTER_MODE_VS))->GetCheck())
		{
			masterMode = CORACQ_VAL_MASTER_MODE_VSYNC;
		}
		else
		{
			masterMode = CORACQ_VAL_MASTER_MODE_DISABLE;
		}

      if (!m_pAcq->SetParameter(CORACQ_PRM_MASTER_MODE, masterMode))
		{
			CDialog::OnCancel();
			return;
		}
	}

	CDialog::OnOK();
}


BOOL CAScanDlg::OnInitDialog() 
{
	SAPSTATUS status;
	int camTriggerMethodPrm;
	int camResetMethodPrm;
	int timeIntegrateMethodPrm;
	int frameIntegrateMethodPrm;
	BOOL controlEnable = FALSE;

	CDialog::OnInitDialog();

	// Verifie acquisition object
	if (m_pAcq == NULL)
	{
		AfxMessageBox("Invalid acquisition object");
		CDialog::OnCancel();
		return FALSE;
	}

	// Initialize Camera Trigger feature
   if (!m_pAcq->GetCapability(CORACQ_CAP_CAM_TRIGGER, &m_camTriggerCap))
	{
		CDialog::OnCancel();
		return FALSE;
	}

   if (!m_pAcq->GetParameter(CORACQ_PRM_CAM_TRIGGER_METHOD, &camTriggerMethodPrm))
	{
		CDialog::OnCancel();
		return FALSE;
	}

	if( m_camTriggerCap && camTriggerMethodPrm)
	{
		int camTriggerEnable;

      if (!m_pAcq->GetParameter(CORACQ_PRM_CAM_TRIGGER_ENABLE, &camTriggerEnable))
		{
			CDialog::OnCancel();
			return FALSE;
		}

		if( camTriggerEnable)
		{
			((CButton *) GetDlgItem(IDC_SCG_AS_CAM_TRIGGER_ENABLE))->SetCheck(TRUE);
			controlEnable = TRUE;
		}
	}
	else
	{
     	GetDlgItem(IDC_SCG_AS_CAM_TRIGGER_ENABLE)->EnableWindow(FALSE);
	}


	// Initialize Camera Reset feature
   if (!m_pAcq->GetCapability(CORACQ_CAP_CAM_RESET, &m_camResetCap))
	{
		CDialog::OnCancel();
		return FALSE;
	}

   if (!m_pAcq->GetParameter(CORACQ_PRM_CAM_RESET_METHOD, &camResetMethodPrm))
	{
		CDialog::OnCancel();
		return FALSE;
	}

	if( m_camResetCap && camResetMethodPrm)
	{
		int camResetEnable;

      if (!m_pAcq->GetParameter(CORACQ_PRM_CAM_RESET_ENABLE, &camResetEnable))
		{
			CDialog::OnCancel();
			return FALSE;
		}

		if( camResetEnable)
		{
			((CButton *) GetDlgItem(IDC_SCG_AS_CAM_RESET_ENABLE))->SetCheck(TRUE);
			controlEnable = TRUE;
		}
	}
	else
	{
     	GetDlgItem(IDC_SCG_AS_CAM_RESET_ENABLE)->EnableWindow(FALSE);
	}

	
	// Initialize Time Integrate feature
   if (!m_pAcq->GetCapability(CORACQ_CAP_TIME_INTEGRATE, &m_timeIntegrateCap))
	{
		CDialog::OnCancel();
		return FALSE;
	}

   if (!m_pAcq->GetParameter(CORACQ_PRM_TIME_INTEGRATE_METHOD, &timeIntegrateMethodPrm))
	{
		CDialog::OnCancel();
		return FALSE;
	}

	if( m_timeIntegrateCap && timeIntegrateMethodPrm)
	{
		int timeIntegrateEnable;

      if (!m_pAcq->GetParameter(CORACQ_PRM_TIME_INTEGRATE_ENABLE, &timeIntegrateEnable))
		{
			CDialog::OnCancel();
			return FALSE;
		}

		if( timeIntegrateEnable)
		{
			((CButton *) GetDlgItem(IDC_SCG_AS_TIME_INTEGRATE_ENABLE))->SetCheck(TRUE);
			controlEnable = TRUE;
		}

      if (!m_pAcq->GetCapability(CORACQ_CAP_TIME_INTEGRATE_DURATION_MIN, &m_timeIntegrateDurationMinCap))
		{
			CDialog::OnCancel();
			return FALSE;
		}

      if (!m_pAcq->GetCapability(CORACQ_CAP_TIME_INTEGRATE_DURATION_MAX, &m_timeIntegrateDurationMaxCap))
		{
			CDialog::OnCancel();
			return FALSE;
		}

      if (!m_pAcq->GetParameter(CORACQ_PRM_TIME_INTEGRATE_DURATION, &m_time))
		{
			CDialog::OnCancel();
			return FALSE;
		}

      if (!m_pAcq->GetParameter(CORACQ_PRM_CAM_TIME_INTEGRATE_DURATION_MIN, &m_camTimeIntegrateDurationMin))
		{
			CDialog::OnCancel();
			return FALSE;
		}

      if (!m_pAcq->GetParameter(CORACQ_PRM_CAM_TIME_INTEGRATE_DURATION_MAX, &m_camTimeIntegrateDurationMax))
		{
			CDialog::OnCancel();
			return FALSE;
		}

	   VerifyTimeIntegrateDuration( FALSE);
	}
	else
	{
		GetDlgItem(IDC_SCG_AS_TIME_INTEGRATE_ENABLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SCG_AS_EDIT_TIME_INTEGRATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SCG_AS_SCROLLBAR_TIME)->EnableWindow(FALSE);
	}


	// Initialize Frame Integrate feature
   if (!m_pAcq->GetCapability(CORACQ_CAP_FRAME_INTEGRATE, &m_frameIntegrateCap))
	{
		CDialog::OnCancel();
		return FALSE;
	}

   if (!m_pAcq->GetParameter(CORACQ_PRM_FRAME_INTEGRATE_METHOD, &frameIntegrateMethodPrm))
	{
		CDialog::OnCancel();
		return FALSE;
	}

	if( m_frameIntegrateCap && frameIntegrateMethodPrm)
	{
		int frameIntegrateEnable;

      if (!m_pAcq->GetParameter(CORACQ_PRM_FRAME_INTEGRATE_ENABLE, &frameIntegrateEnable))
		{
			CDialog::OnCancel();
			return FALSE;
		}

		if( frameIntegrateEnable)
		{
			((CButton *) GetDlgItem(IDC_SCG_AS_FRAME_INTEGRATE_ENABLE))->SetCheck(TRUE);
			controlEnable = TRUE;
		}

      if (!m_pAcq->GetCapability(CORACQ_CAP_FRAME_INTEGRATE_COUNT_MAX, &m_frameIntegrateCountMaxCap))
		{
			CDialog::OnCancel();
			return FALSE;
		}

      if (!m_pAcq->GetParameter(CORACQ_PRM_FRAME_INTEGRATE_COUNT, &m_frame))
		{
			CDialog::OnCancel();
			return FALSE;
		}
	}
	else
	{
		m_frame = 1;
		GetDlgItem(IDC_SCG_AS_FRAME_INTEGRATE_ENABLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SCG_AS_EDIT_FRAME_INTEGRATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SCG_AS_SCROLLBAR_FRAME)->EnableWindow(FALSE);
	}

	// If no camera control is selected, then check the 'none' button
	if( !controlEnable)
	{
		((CButton *) GetDlgItem(IDC_SCG_AS_RADIO_NONE))->SetCheck(TRUE);
	}


	// Initialize the master mode feature
	status = CorAcqGetCap(m_pAcq->GetHandle(), CORACQ_CAP_MASTER_MODE, &m_masterModeCap);
	if( status)
	{
		if( CORSTATUS_ID( status) != CORSTATUS_CAP_NOT_AVAILABLE)
		{
			DisplayStatus( "CorAcqGetCap", status);

			CDialog::OnCancel();
			return FALSE;
		}
		else
		{
			// Master mode not available
			m_masterModeCap = 0;
		}
	}
	else
	{
		int masterMode;

      if (!m_pAcq->GetParameter(CORACQ_PRM_MASTER_MODE, &masterMode))
		{
			CDialog::OnCancel();
			return FALSE;
		}

		if( masterMode == CORACQ_VAL_MASTER_MODE_HSYNC_VSYNC)
		{
			((CButton *) GetDlgItem(IDC_SCG_AS_MASTER_MODE_HS))->SetCheck(TRUE);
			((CButton *) GetDlgItem(IDC_SCG_AS_MASTER_MODE_VS))->SetCheck(TRUE);
		}
		else if( masterMode == CORACQ_VAL_MASTER_MODE_HSYNC)
		{
			((CButton *) GetDlgItem(IDC_SCG_AS_MASTER_MODE_HS))->SetCheck(TRUE);
		}
		else if( masterMode == CORACQ_VAL_MASTER_MODE_VSYNC)
		{
			((CButton *) GetDlgItem(IDC_SCG_AS_MASTER_MODE_VS))->SetCheck(TRUE);
		}
	}

	UpdateData( FALSE);
	UpdateWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CAScanDlg::UpdateWindow() 
{
	BOOL timeIntegrateEnable;
	BOOL frameIntegrateEnable;

	// Enable/Disable the Time Integrate feature
	timeIntegrateEnable = ((CButton *)GetDlgItem(IDC_SCG_AS_TIME_INTEGRATE_ENABLE))->GetCheck();

	GetDlgItem(IDC_SCG_AS_EDIT_TIME_INTEGRATE)->EnableWindow( timeIntegrateEnable);
	GetDlgItem(IDC_SCG_AS_SCROLLBAR_TIME)->EnableWindow( timeIntegrateEnable);

	// Enable/Disable the Frame Integrate feature
	frameIntegrateEnable = ((CButton *)GetDlgItem(IDC_SCG_AS_FRAME_INTEGRATE_ENABLE))->GetCheck();
	GetDlgItem(IDC_SCG_AS_EDIT_FRAME_INTEGRATE)->EnableWindow( frameIntegrateEnable);
	GetDlgItem(IDC_SCG_AS_SCROLLBAR_FRAME)->EnableWindow( frameIntegrateEnable);

	// Enable the master mode feature
   if ((m_masterModeCap & CORACQ_VAL_MASTER_MODE_HSYNC_VSYNC) != 0)
   {
   	GetDlgItem(IDC_SCG_AS_MASTER_MODE_HS)->EnableWindow(TRUE);
	   GetDlgItem(IDC_SCG_AS_MASTER_MODE_VS)->EnableWindow(TRUE);
   }
   else
   {
	   GetDlgItem(IDC_SCG_AS_MASTER_MODE_HS)->EnableWindow((m_masterModeCap & CORACQ_VAL_MASTER_MODE_HSYNC) != 0);
	   GetDlgItem(IDC_SCG_AS_MASTER_MODE_VS)->EnableWindow((m_masterModeCap & CORACQ_VAL_MASTER_MODE_VSYNC) != 0);

   }
}


void CAScanDlg::OnCamTriggerEnable() 
{
	UpdateWindow();
}

void CAScanDlg::OnCamResetEnable() 
{
	UpdateWindow();
}

void CAScanDlg::OnTimeIntegrate() 
{
	UpdateWindow();
}


void CAScanDlg::OnKillfocusEditTime() 
{
   UpdateData(TRUE);
   VerifyTimeIntegrateDuration( TRUE);
	UpdateWindow();
}


void CAScanDlg::OnFrameIntegrate() 
{
	UpdateWindow();
}


void CAScanDlg::OnKillfocusEditFrame() 
{
   UpdateData(TRUE);
   VerifyFrameIntegrateCount();
   UpdateWindow();	
}


void CAScanDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	BOOL timeIntegrateEnable;
	BOOL frameIntegrateEnable;

	timeIntegrateEnable = ((CButton *)GetDlgItem(IDC_SCG_AS_TIME_INTEGRATE_ENABLE))->GetCheck();
	frameIntegrateEnable = ((CButton *)GetDlgItem(IDC_SCG_AS_FRAME_INTEGRATE_ENABLE))->GetCheck();

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);

	if( ((CScrollBar *)pScrollBar == &m_ScrollBar_Time) && timeIntegrateEnable)
	{
		switch( nSBCode)
		{

			case SB_LINEDOWN:
				m_time -= 10;
				break;
			case SB_LINEUP:
				m_time += 10;
				break;
			default:
				return;
		}
		VerifyTimeIntegrateDuration( TRUE);
	}

	if( ((CScrollBar *)pScrollBar == &m_ScrollBar_Frame) && frameIntegrateEnable)
	{
		switch( nSBCode)
		{

			case SB_LINEDOWN:
				m_frame -= 1;
				break;
			case SB_LINEUP:
				m_frame += 1;
				break;
			default:
				return;
		}
		VerifyFrameIntegrateCount();
	}
}


void CAScanDlg::VerifyTimeIntegrateDuration( BOOL bNotify)
{
	BOOL bAdjusted = FALSE;

	if( m_time > m_timeIntegrateDurationMaxCap)
	{
		bAdjusted = TRUE;
		m_time = m_timeIntegrateDurationMaxCap;
	}

	if( m_time < m_timeIntegrateDurationMinCap)
	{
		bAdjusted = TRUE;
		m_time = m_timeIntegrateDurationMinCap;
	}

	
	if( m_time > m_camTimeIntegrateDurationMax)
	{
		bAdjusted = TRUE;
		m_time = m_camTimeIntegrateDurationMax;
	}

	if( m_time < m_camTimeIntegrateDurationMin)
	{
		bAdjusted = TRUE;
		m_time = m_camTimeIntegrateDurationMin;
	}

	UpdateData( FALSE);

	if( bAdjusted && bNotify)
	{
		char msg[100];

		CorSnprintf( msg, sizeof(msg), "Time integration duration out of range. Value set to: %d\n", m_time);
		AfxMessageBox( msg);

		UpdateWindow();
	}
}


void CAScanDlg::VerifyFrameIntegrateCount()
{
	BOOL bAdjusted = FALSE;

	if( m_frame > m_frameIntegrateCountMaxCap)
	{
		bAdjusted = TRUE;
		m_frame =  m_frameIntegrateCountMaxCap;
	}

	if( m_frame == 0)
	{
		bAdjusted = TRUE;
		m_frame = 1;
	}

	UpdateData( FALSE);

	if( bAdjusted)
	{
		char msg[100];

		CorSnprintf( msg, sizeof(msg), "Frame integration count out of range. Value set to: %d\n", m_frame);
		AfxMessageBox( msg);

		UpdateWindow();
	}
}

