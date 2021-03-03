// BayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SapClassBasic.h"
#include "SapClassGui.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SapBayer::Align CBayerOptionsDlg::m_AlignValues[NumAlignValues] = 
{
   SapBayer::AlignGBRG, SapBayer::AlignBGGR, SapBayer::AlignRGGB, SapBayer::AlignGRBG
};

SapBayer::Method CBayerOptionsDlg::m_MethodValues[NumMethodValues] =
{
   SapBayer::Method1, SapBayer::Method2, SapBayer::Method3, SapBayer::Method4, SapBayer::Method5, SapBayer::Method6
};

/////////////////////////////////////////////////////////////////////////////
// CBayerOptionsDlg dialog

CBayerOptionsDlg::CBayerOptionsDlg(CWnd *pParent, SapBayer *pBayer, SapTransfer *pXfer, CImageWnd *pImageWnd, SapProcessing* pPro)
   : CDialog(CBayerOptionsDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CBayerOptionsDlg)
   m_BlueGain = 1.0f;
   m_Gamma = 1.0f;
   m_GreenGain = 1.0f;
   m_RedGain = 1.0f;
	m_GammaEnabled = FALSE;
	//}}AFX_DATA_INIT

   m_pBayer		= pBayer;
   m_pXfer		= pXfer;
	m_pPro		= pPro;
   m_pImageWnd = pImageWnd;
}

CBayerOptionsDlg::~CBayerOptionsDlg()
{
}

void CBayerOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CBayerOptionsDlg)
   DDX_Text(pDX, IDC_SCG_BAYER_BLUE_GAIN, m_BlueGain);
   DDX_Text(pDX, IDC_SCG_BAYER_GAMMA, m_Gamma);
   DDX_Text(pDX, IDC_SCG_BAYER_GREEN_GAIN, m_GreenGain);
   DDX_Text(pDX, IDC_SCG_BAYER_RED_GAIN, m_RedGain);
	DDX_Check(pDX, IDC_SCG_BAYER_GAMMA_ENABLE, m_GammaEnabled);
	//}}AFX_DATA_MAP
   DDX_Radio(pDX, IDC_SCG_BAYER_ALIGN_GB_RG, m_Align);
   DDX_Radio(pDX, IDC_SCG_BAYER_METHOD_1, m_Method);
   
}

BEGIN_MESSAGE_MAP(CBayerOptionsDlg, CDialog)
   //{{AFX_MSG_MAP(CBayerOptionsDlg)
   ON_EN_CHANGE(IDC_SCG_BAYER_BLUE_GAIN, OnChange)
   ON_BN_CLICKED(IDC_SCG_BAYER_AUTO_WHITE_BALANCE, OnAutoWhiteBalance)
   ON_BN_CLICKED(IDC_SCG_BAYER_APPLY, OnApply)
   ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDOK, OnClose)
   ON_EN_CHANGE(IDC_SCG_BAYER_GAMMA, OnChange)
   ON_EN_CHANGE(IDC_SCG_BAYER_GREEN_GAIN, OnChange)
   ON_EN_CHANGE(IDC_SCG_BAYER_RED_GAIN, OnChange)
	ON_BN_CLICKED(IDC_SCG_BAYER_GAMMA_ENABLE, OnGammaEnable)
	//}}AFX_MSG_MAP
   ON_CONTROL_RANGE(BN_CLICKED, IDC_SCG_BAYER_ALIGN_GB_RG, IDC_SCG_BAYER_ALIGN_GR_BG, OnChangeRange)
   ON_CONTROL_RANGE(BN_CLICKED, IDC_SCG_BAYER_METHOD_1, IDC_SCG_BAYER_METHOD_6, OnChangeRange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBayerOptionsDlg message handlers

BOOL CBayerOptionsDlg::OnInitDialog() 
{
   CDialog::OnInitDialog();

   if (m_pBayer == NULL)
   {
      AfxMessageBox("No Bayer object specified");
      CDialog::OnCancel();

      return FALSE;
   }

   UpdateInterface();

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CBayerOptionsDlg::UpdateInterface()
{
	SapBayer::Align  AlignCap = m_pBayer->GetAvailAlign();
	SapBayer::Method MethodCap= m_pBayer->GetAvailMethod();

   // Do not declare in 'for' statement, as we get an error with Visual Studio 2005 in the next loop
   int i;

   // Initialize settings for Bayer alignment
   for (i = 0; i < NumAlignValues; i++)
   {
      if (m_AlignValues[i] == m_pBayer->GetAlign())
      {
         m_Align = i;
         break;
      }
   }
   
	// Check which alignment is available
   GetDlgItem(IDC_SCG_BAYER_ALIGN_GB_RG)->EnableWindow( AlignCap & SapBayer::AlignGBRG);
   GetDlgItem(IDC_SCG_BAYER_ALIGN_BG_GR)->EnableWindow( AlignCap & SapBayer::AlignBGGR);
   GetDlgItem(IDC_SCG_BAYER_ALIGN_RG_GB)->EnableWindow( AlignCap & SapBayer::AlignRGGB);
   GetDlgItem(IDC_SCG_BAYER_ALIGN_GR_BG)->EnableWindow( AlignCap & SapBayer::AlignGRBG);

   // Initialize settings for Bayer method
   for (i = 0; i < NumMethodValues; i++)
   {
      if (m_MethodValues[i] == m_pBayer->GetMethod())
      {
         m_Method = i;
         break;
      }
   }

	// Check which interpolation method is available
   GetDlgItem(IDC_SCG_BAYER_METHOD_1)->EnableWindow( MethodCap & SapBayer::Method1);
   GetDlgItem(IDC_SCG_BAYER_METHOD_2)->EnableWindow( MethodCap & SapBayer::Method2);
   GetDlgItem(IDC_SCG_BAYER_METHOD_3)->EnableWindow( MethodCap & SapBayer::Method3);
   GetDlgItem(IDC_SCG_BAYER_METHOD_4)->EnableWindow( MethodCap & SapBayer::Method4);
   GetDlgItem(IDC_SCG_BAYER_METHOD_5)->EnableWindow( MethodCap & SapBayer::Method5);
   GetDlgItem(IDC_SCG_BAYER_METHOD_6)->EnableWindow( MethodCap & SapBayer::Method6);

   // Initialize gain values
   SapDataFRGB wbGain = m_pBayer->GetWBGain();

   m_RedGain   = wbGain.Red();
   m_GreenGain = wbGain.Green();
   m_BlueGain  = wbGain.Blue();

   GetDlgItem(IDC_SCG_BAYER_AUTO_WHITE_BALANCE)->EnableWindow( m_pImageWnd != NULL);

   // Initialize Gamma correction factor
   m_Gamma = m_pBayer->GetGamma();
	m_GammaEnabled= m_pBayer->IsLutEnabled();

	// Check if bayer decoder is enabled and if a lookup table is available
   GetDlgItem(IDC_SCG_BAYER_GAMMA)->EnableWindow( m_pBayer->IsEnabled() && m_GammaEnabled);
   GetDlgItem(IDC_SCG_BAYER_GAMMA_ENABLE)->EnableWindow( m_pBayer->IsEnabled());

	// Disable Apply button until something change
   GetDlgItem(IDC_SCG_BAYER_APPLY)->EnableWindow(FALSE);

   UpdateData(FALSE);
}

void CBayerOptionsDlg::OnChange() 
{
   UpdateData(TRUE);

	// Something change so enable the Apply button
   GetDlgItem(IDC_SCG_BAYER_APPLY)->EnableWindow(TRUE);
}

void CBayerOptionsDlg::OnGammaEnable() 
{
   UpdateData(TRUE);

   GetDlgItem(IDC_SCG_BAYER_GAMMA)->EnableWindow( m_pBayer->IsEnabled() && m_GammaEnabled);

	// Something change so enable the Apply button
   GetDlgItem(IDC_SCG_BAYER_APPLY)->EnableWindow(TRUE);		
}

void CBayerOptionsDlg::OnChangeRange(UINT id)
{
   OnChange();
}

void CBayerOptionsDlg::OnAutoWhiteBalance()
{
   if (m_pImageWnd == NULL)
      return;

   if (!m_pImageWnd->IsRoiTrackerActive())
   {
      AfxMessageBox("You must select a ROI containing white pixels");
      return;
   }

   if ( m_pBayer->IsEnabled() && !m_pBayer->IsSoftware())
	{
      AfxMessageBox("White balance is not available when hardware Bayer conversion is enabled");
      return;
	}

   CRect rect = m_pImageWnd->GetSelectedRoi();

   if (rect.Width() > 1 && rect.Height() > 1)
   {
		// Compute new white balance factors from region of interest
      if (m_pBayer->WhiteBalance(rect.left, rect.top, rect.Width(), rect.Height()))
      {
			// Update user interface
         UpdateInterface();

			// Redraw the image
         UpdateView();
      }
   }
}

void CBayerOptionsDlg::OnApply() 
{
	// Set alignment 
   m_pBayer->SetAlign(m_AlignValues[m_Align]);

	// Set interpolation method
	m_pBayer->SetMethod(m_MethodValues[m_Method]);

	// Set white balance's gain
   if( !m_pBayer->SetWBGain( SapDataFRGB(m_RedGain, m_GreenGain, m_BlueGain)))
	{
		SapDataFRGB wbGain= m_pBayer->GetWBGain();

		m_RedGain  = wbGain.Red();
		m_GreenGain= wbGain.Green();
		m_BlueGain = wbGain.Blue();
		
		UpdateData( FALSE);
	}

	if( m_GammaEnabled)
	{
		// Set new gamma factor
		m_pBayer->SetGamma(m_Gamma);
	}

	// Enable/Disable lookup table
   m_pBayer->EnableLut( m_GammaEnabled);


	// Redraw the image
   UpdateView();

	// Disable apply button
   GetDlgItem(IDC_SCG_BAYER_APPLY)->EnableWindow(FALSE);
}

void CBayerOptionsDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
   CDialog::OnShowWindow(bShow, nStatus);
   
   if (bShow)
	{
		if( m_pImageWnd != NULL)
		{
			// Redraw the image
			m_pImageWnd->Invalidate( FALSE);
			m_pImageWnd->OnPaint();
		}

		// Update user interface
      UpdateInterface();
	}
}

void CBayerOptionsDlg::UpdateView() 
{
   if (m_pBayer->IsEnabled())
	{
		// Check if we are operating on-line
		if( m_pXfer && *m_pXfer)
		{
			// Check if we are grabbing
			if( m_pXfer->IsGrabbing())
				// The view will be automatically updated on the next acquired frame
				return;

			// Check if we are using an hardware bayer decoder
			if( !m_pBayer->IsSoftware())
			{
				// Acquire one frame
				m_pXfer->Snap();

				return;
			}
		}

		// Else, apply bayer conversion to current buffer's content
		if( m_pPro != NULL)
		{
			m_pPro->Execute();
		}
		else
		{
			m_pBayer->Convert();
			if( m_pImageWnd != NULL)
			{
				// Redraw the bayer decoded image
				m_pImageWnd->Invalidate( FALSE);
				m_pImageWnd->OnPaint();
			}
		}
	}
}

void CBayerOptionsDlg::OnClose() 
{
	CDialog::OnOK();

	if( m_pImageWnd !=NULL)
	{
		// Hide ROI tracker
		m_pImageWnd->HideRoiTracker();

		// Redraw the image to remove tracker
		m_pImageWnd->Invalidate( FALSE);
		m_pImageWnd->OnPaint();
	}
}


