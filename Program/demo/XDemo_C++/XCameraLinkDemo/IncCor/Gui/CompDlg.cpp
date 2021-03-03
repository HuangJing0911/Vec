// CompDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SapClassBasic.h"
#include "SapClassGui.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PLANE_MONO	0
#define PLANE_RED		1
#define PLANE_GREEN	2
#define PLANE_BLUE	3

/////////////////////////////////////////////////////////////////////////////
// CCompDlg dialog


CCompDlg::CCompDlg(CWnd* pParent, SapAcquisition *pAcq, SapTransfer *pXfer)
	: CDialog(CCompDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCompDlg)
	m_brightness = 0;
	m_hue = 0;
	m_contrast = 0;
	m_saturation = 0;
	m_sharpness = 0;
	//}}AFX_DATA_INIT
	m_pAcq = pAcq;
	m_pXfer = pXfer;
}


void CCompDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCompDlg)
	DDX_Control(pDX, IDC_SCG_COMP_SLIDER_SHARPNESS, m_slider_sharpness);
	DDX_Control(pDX, IDC_SCG_COMP_SLIDER_SATURATION, m_slider_saturation);
	DDX_Control(pDX, IDC_SCG_COMP_SLIDER_HUE, m_slider_hue);
	DDX_Control(pDX, IDC_SCG_COMP_SLIDER_CONTRAST, m_slider_contrast);
	DDX_Control(pDX, IDC_SCG_COMP_SLIDER_BRIGHTNESS, m_slider_brightness);
	DDX_Text(pDX, IDC_SCG_COMP_EDIT_BRIGHTNESS, m_brightness);
	DDX_Text(pDX, IDC_SCG_COMP_EDIT_HUE, m_hue);
	DDX_Text(pDX, IDC_SCG_COMP_EDIT_CONTRAST, m_contrast);
	DDX_Text(pDX, IDC_SCG_COMP_EDIT_SATURATION, m_saturation);
	DDX_Text(pDX, IDC_SCG_COMP_EDIT_SHARPNESS, m_sharpness);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCompDlg, CDialog)
	//{{AFX_MSG_MAP(CCompDlg)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_SCG_COMP_RED, OnRed)
	ON_BN_CLICKED(IDC_SCG_COMP_GREEN, OnGreen)
	ON_BN_CLICKED(IDC_SCG_COMP_BLUE, OnBlue)
	ON_EN_KILLFOCUS(IDC_SCG_COMP_EDIT_CONTRAST, OnKillfocusEditContrast)
	ON_EN_KILLFOCUS(IDC_SCG_COMP_EDIT_BRIGHTNESS, OnKillfocusEditBrightness)
	ON_EN_KILLFOCUS(IDC_SCG_COMP_EDIT_SATURATION, OnKillfocusEditSaturation)
	ON_EN_KILLFOCUS(IDC_SCG_COMP_EDIT_HUE, OnKillfocusEditHue)
	ON_BN_CLICKED(IDC_SCG_COMP_DEFAULT, OnDefault)
	ON_EN_KILLFOCUS(IDC_SCG_COMP_EDIT_SHARPNESS, OnKillfocusEditSharpness)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCompDlg::Snap( void)
{
   if(m_pXfer != NULL)
   {
	   static BOOL bInProgress = FALSE;
	   if (!bInProgress)
	   {
		   bInProgress = TRUE;

		   m_pXfer->Snap();
		   if (CAbortDlg(this, m_pXfer).DoModal() != IDOK) 
			   m_pXfer->Abort();

		   bInProgress = FALSE;
	   }
   }
}

/////////////////////////////////////////////////////////////////////////////
// CCompDlg message handlers

void CCompDlg::OnOK() 
{
	CDialog::OnOK();
}

void CCompDlg::OnCancel() 
{
	// Put back initial values
	if( m_curPlane == PLANE_MONO)
	{
		if( m_contrastCap)
		{
			m_contrast = m_contrastInit[ PLANE_MONO];
			SetContrast();
		}

		if( m_brightnessCap)
		{
			m_brightness = m_brightnessInit[ PLANE_MONO];
			SetBrightness();
		}
	}
	else
	{
		for( m_curPlane = PLANE_RED; m_curPlane <= PLANE_BLUE; m_curPlane++)
		{
			if( m_contrastColorCap)
			{
				m_contrast = m_contrastInit[ m_curPlane];
				SetContrast();
			}

			if( m_brightnessColorCap)
			{
				m_brightness = m_brightnessInit[ m_curPlane];
				SetBrightness();
			}
		}
	}

	if( m_hueCap)
	{
		m_hue = m_hueInit;
		SetHue();
	}

	if( m_saturationCap)
	{
		m_saturation = m_saturationInit;
		SetSaturation();
	}

	if( m_sharpnessCap)
	{
		m_sharpness = m_sharpnessInit;
		SetSharpness();
	}

	Snap();
	
	CDialog::OnCancel();
}

BOOL CCompDlg::OnInitDialog() 
{
	int videoStd, videoType;
	int plane;

	CDialog::OnInitDialog();

	// Verifie acquisition object
	if (m_pAcq == NULL)
	{
		AfxMessageBox("Invalid acquisition object");
		CDialog::OnCancel();
		return FALSE;
	}

	m_contrastPrm[ PLANE_MONO] = CORACQ_PRM_CONTRAST;
	m_contrastPrm[ PLANE_RED] = CORACQ_PRM_CONTRAST_RED;
	m_contrastPrm[ PLANE_GREEN]= CORACQ_PRM_CONTRAST_GREEN;
	m_contrastPrm[ PLANE_BLUE] = CORACQ_PRM_CONTRAST_BLUE;

	m_brightnessPrm[ PLANE_MONO] = CORACQ_PRM_BRIGHTNESS;
	m_brightnessPrm[ PLANE_RED] = CORACQ_PRM_BRIGHTNESS_RED;
	m_brightnessPrm[ PLANE_GREEN] = CORACQ_PRM_BRIGHTNESS_GREEN;
	m_brightnessPrm[ PLANE_BLUE] = CORACQ_PRM_BRIGHTNESS_BLUE;
	
   m_huePrm = CORACQ_PRM_HUE;
   m_saturationPrm = CORACQ_PRM_SATURATION;
   m_sharpnessPrm = CORACQ_PRM_SHARPNESS;

	// Disable plane radio boxes
	m_curPlane = PLANE_MONO;

	// Contrast capabilities
   m_contrastCap = FALSE;
   m_contrastColorCap = FALSE;
	for( plane = PLANE_MONO; plane <= PLANE_BLUE; plane++)
	{
		m_contrastMinCap[ plane] = 100000;
		m_contrastMaxCap[ plane] = 100000;
		m_contrastValues[ plane] = 100000;
	}

   m_pAcq->GetCapability(CORACQ_CAP_CONTRAST, &m_contrastCap);
   m_pAcq->GetCapability(CORACQ_CAP_CONTRAST_RED, &m_contrastColorCap);

	if( m_contrastCap)
	{
      m_pAcq->GetCapability(CORACQ_CAP_CONTRAST_MIN, &m_contrastMinCap[ PLANE_MONO]);
      m_pAcq->GetCapability(CORACQ_CAP_CONTRAST_MAX, &m_contrastMaxCap[ PLANE_MONO]);

      if (!m_pAcq->GetParameter(CORACQ_PRM_CONTRAST, &m_contrastValues[ PLANE_MONO]))
		{
			CDialog::OnCancel();
			return FALSE;
		}
	}
	else if( m_contrastColorCap)
	{
		// Red, Green, Blue control of the contrast. We will assume in this case that the brightness
		// has also a Red, Green, Blue control.
		//
		m_curPlane = PLANE_RED;	// Default plane will be red
      m_pAcq->GetCapability(CORACQ_CAP_CONTRAST_RED_MIN, &m_contrastMinCap[ PLANE_RED]);
      m_pAcq->GetCapability(CORACQ_CAP_CONTRAST_RED_MAX, &m_contrastMaxCap[ PLANE_RED]);
      m_pAcq->GetCapability(CORACQ_CAP_CONTRAST_GREEN_MIN, &m_contrastMinCap[ PLANE_GREEN]);
      m_pAcq->GetCapability(CORACQ_CAP_CONTRAST_GREEN_MAX, &m_contrastMaxCap[ PLANE_GREEN]);
      m_pAcq->GetCapability(CORACQ_CAP_CONTRAST_BLUE_MIN, &m_contrastMinCap[ PLANE_BLUE]);
      m_pAcq->GetCapability(CORACQ_CAP_CONTRAST_BLUE_MAX, &m_contrastMaxCap[ PLANE_BLUE]);

		// Get current values
		for( plane = PLANE_RED; plane <= PLANE_BLUE; plane++)
		{
         if (!m_pAcq->GetParameter(m_contrastPrm[ plane], &m_contrastValues[ plane]))
			{
				CDialog::OnCancel();
				return FALSE;
			}
		}
	}
	else
	{
		// Disable the Contrast menu
		GetDlgItem(IDC_SCG_COMP_SLIDER_CONTRAST)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_COMP_EDIT_CONTRAST)->EnableWindow( FALSE);
	}


	// Brightness capabilities
   m_brightnessCap = FALSE;
   m_brightnessColorCap = FALSE;
	for( plane = PLANE_MONO; plane <= PLANE_BLUE; plane++)
	{
		m_brightnessMinCap[ plane] = 0;
		m_brightnessMaxCap[ plane] = 0;
		m_brightnessValues[ plane] = 0;
	}

   m_pAcq->GetCapability(CORACQ_CAP_BRIGHTNESS, &m_brightnessCap);
   m_pAcq->GetCapability(CORACQ_CAP_BRIGHTNESS_RED, &m_brightnessColorCap);

	if( m_brightnessCap)
	{
      m_pAcq->GetCapability(CORACQ_CAP_BRIGHTNESS_MIN, &m_brightnessMinCap[ PLANE_MONO]);
      m_pAcq->GetCapability(CORACQ_CAP_BRIGHTNESS_MAX, &m_brightnessMaxCap[ PLANE_MONO]);

      if (!m_pAcq->GetParameter(CORACQ_PRM_BRIGHTNESS, &m_brightnessValues[ PLANE_MONO]))
		{
			CDialog::OnCancel();
			return FALSE;
		}

	}
	else if( m_brightnessColorCap)
	{
      m_pAcq->GetCapability(CORACQ_CAP_BRIGHTNESS_RED_MIN, &m_brightnessMinCap[ PLANE_RED]);
      m_pAcq->GetCapability(CORACQ_CAP_BRIGHTNESS_RED_MAX, &m_brightnessMaxCap[ PLANE_RED]);
      m_pAcq->GetCapability(CORACQ_CAP_BRIGHTNESS_GREEN_MIN, &m_brightnessMinCap[ PLANE_GREEN]);
      m_pAcq->GetCapability(CORACQ_CAP_BRIGHTNESS_GREEN_MAX, &m_brightnessMaxCap[ PLANE_GREEN]);
      m_pAcq->GetCapability(CORACQ_CAP_BRIGHTNESS_BLUE_MIN, &m_brightnessMinCap[ PLANE_BLUE]);
      m_pAcq->GetCapability(CORACQ_CAP_BRIGHTNESS_BLUE_MAX, &m_brightnessMaxCap[ PLANE_BLUE]);

		for( plane = PLANE_RED; plane <= PLANE_BLUE; plane++)
		{
         if (!m_pAcq->GetParameter(m_brightnessPrm[ plane], &m_brightnessValues[ plane]))
			{
				CDialog::OnCancel();
				return FALSE;
			}
		}
	}
	else
	{
		//	Disable the Brightness Menu
		GetDlgItem(IDC_SCG_COMP_SLIDER_BRIGHTNESS)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_COMP_EDIT_BRIGHTNESS)->EnableWindow( FALSE);
	}


	// Hue capability
   m_hueCap = FALSE;
	m_hueMinCap = 0;
	m_hueMaxCap = 0;
	m_hue = 0;

   m_pAcq->GetCapability(CORACQ_CAP_HUE, &m_hueCap);

	// Hue is only available if grabbing with an NTSC composite color or Y/C camera
   if (!m_pAcq->GetParameter(CORACQ_PRM_VIDEO, &videoType))
	{
		CDialog::OnCancel();
		return FALSE;
	}

   if (!m_pAcq->GetParameter(CORACQ_PRM_VIDEO_STD, &videoStd))
	{
		CDialog::OnCancel();
		return FALSE;
	}

	if( m_hueCap && ((videoType == CORACQ_VAL_VIDEO_COLOR) || (videoType == CORACQ_VAL_VIDEO_YC)) && ( videoStd == CORACQ_VAL_VIDEO_STD_RS170_NTSC))
	{
      m_pAcq->GetCapability(CORACQ_CAP_HUE_MIN, &m_hueMinCap);
      m_pAcq->GetCapability(CORACQ_CAP_HUE_MAX, &m_hueMaxCap);

      if (!m_pAcq->GetParameter(CORACQ_PRM_HUE, &m_hue))
		{
			CDialog::OnCancel();
			return FALSE;
		}

	}
	else
	{
		// Disable the Hue Menu
		GetDlgItem(IDC_SCG_COMP_SLIDER_HUE)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_COMP_EDIT_HUE)->EnableWindow( FALSE);
	}


	// Saturation capability
   m_saturationCap = FALSE;
	m_saturationMinCap = 100000;
	m_saturationMaxCap = 100000;
	m_saturation = 100000;

   m_pAcq->GetCapability(CORACQ_CAP_SATURATION, &m_saturationCap);

	if( m_saturationCap && ((videoType == CORACQ_VAL_VIDEO_COLOR) || (videoType == CORACQ_VAL_VIDEO_YC)))
	{
      m_pAcq->GetCapability(CORACQ_CAP_SATURATION_MIN, &m_saturationMinCap);
      m_pAcq->GetCapability(CORACQ_CAP_SATURATION_MAX, &m_saturationMaxCap);

      if (!m_pAcq->GetParameter(CORACQ_PRM_SATURATION, &m_saturation))
		{
			CDialog::OnCancel();
			return FALSE;
		}
	}
	else
	{
		// Disable the Saturation Menu
		GetDlgItem(IDC_SCG_COMP_SLIDER_SATURATION)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_COMP_EDIT_SATURATION)->EnableWindow( FALSE);
	}


	// Sharpness capability
   m_sharpnessCap = FALSE;
   m_pAcq->GetCapability(CORACQ_CAP_SHARPNESS, &m_sharpnessCap);
	m_sharpnessMinCap = 0;
	m_sharpnessMaxCap = 0;
	m_sharpness = 0;

	if( m_sharpnessCap)
	{
      m_pAcq->GetCapability(CORACQ_CAP_SHARPNESS_MIN, &m_sharpnessMinCap);
      m_pAcq->GetCapability(CORACQ_CAP_SHARPNESS_MAX, &m_sharpnessMaxCap);

      if (!m_pAcq->GetParameter(CORACQ_PRM_SHARPNESS, &m_sharpness))
		{
			CDialog::OnCancel();
			return FALSE;
		}
	}
	else
	{
		// Disable the Hue Menu
		GetDlgItem(IDC_SCG_COMP_SLIDER_SHARPNESS)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_COMP_EDIT_SHARPNESS)->EnableWindow( FALSE);
	}

	// Scale the min/max values to integers. For minimum values, round up.
	for( plane = PLANE_MONO; plane <= PLANE_BLUE; plane++)
	{
		m_contrastMinCap[ plane] = (m_contrastMinCap[ plane] + 999)/1000;
		m_contrastMaxCap[ plane] /= 1000;
		m_contrastValues[ plane] /= 1000;
		m_contrastInit[ plane] = m_contrastValues[ plane];

		m_brightnessMinCap[ plane] = (m_brightnessMinCap[ plane] + 999)/1000;
		m_brightnessMaxCap[ plane] /= 1000;
		m_brightnessValues[ plane] /= 1000;
		m_brightnessInit[ plane] = m_brightnessValues[ plane];
	}

	m_contrast = m_contrastValues[ m_curPlane];
	m_brightness = m_brightnessValues[ m_curPlane];

	m_hueMinCap = (m_hueMinCap + 999)/1000;
	m_hueMaxCap /= 1000;
	m_hue /= 1000;
	m_hueInit = m_hue;

	m_saturationMinCap = (m_saturationMinCap + 999)/1000;
	m_saturationMaxCap /= 1000;
	m_saturation /= 1000;
	m_saturationInit = m_saturation;

   m_sharpnessInit = m_sharpness;


	// Initialize the slide bars of hue and saturation which are fix.
	m_slider_hue.SetRange( m_hueMinCap, m_hueMaxCap);
	m_slider_hue.SetTicFreq(  10);
	m_slider_hue.SetPageSize( 10);
	m_slider_hue.SetPos( m_hue);
	
	m_slider_saturation.SetRange( m_saturationMinCap, m_saturationMaxCap);
	m_slider_saturation.SetTicFreq(  10);
	m_slider_saturation.SetPageSize( 10);
	m_slider_saturation.SetPos( m_saturation);

	// Initialize the slide bars of sharpness.
	m_slider_sharpness.SetRange( m_sharpnessMinCap, m_sharpnessMaxCap);
	m_slider_sharpness.SetTicFreq(  1);
	m_slider_sharpness.SetPageSize( 1);
	m_slider_sharpness.SetPos( m_sharpness);

	// Disable the color planes if not needed
	if( m_curPlane == PLANE_MONO)
	{
		GetDlgItem(IDC_SCG_COMP_RED)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_COMP_GREEN)->EnableWindow( FALSE);
		GetDlgItem(IDC_SCG_COMP_BLUE)->EnableWindow( FALSE);
	}
	else
	{
		// Select the Red plane
		CheckRadioButton(IDC_SCG_COMP_RED, IDC_SCG_COMP_BLUE, IDC_SCG_COMP_RED);
	}


	UpdateWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CCompDlg::UpdateWindow()
{
	// Initialize the slide bars
	m_slider_contrast.SetRange( m_contrastMinCap[ m_curPlane], m_contrastMaxCap[ m_curPlane]);
	m_slider_contrast.SetTicFreq( 10);
	m_slider_contrast.SetPageSize( 10);

	m_slider_brightness.SetRange( m_brightnessMinCap[ m_curPlane], m_brightnessMaxCap[ m_curPlane]);
	m_slider_brightness.SetTicFreq( 10);
	m_slider_brightness.SetPageSize( 10);

	// Get Current contrast and brightness values of current selected color page (in RGB mode)
	if( m_curPlane != PLANE_MONO)
	{
		m_contrast = m_contrastValues[ m_curPlane];
		m_brightness = m_brightnessValues[ m_curPlane];
	}

	m_slider_contrast.SetPos( m_contrast);
	m_slider_brightness.SetPos( m_brightness);
	m_slider_hue.SetPos( m_hue);
	m_slider_saturation.SetPos( m_saturation);
	m_slider_sharpness.SetPos( m_sharpness);

	UpdateData( FALSE);
}


void CCompDlg::SetBrightness( void)
{
	if( m_brightness < m_brightnessMinCap[ m_curPlane])
	{
		m_brightness = m_brightnessMinCap[ m_curPlane];
	}
	else if( m_brightness > m_brightnessMaxCap[ m_curPlane])
	{
		m_brightness=  m_brightnessMaxCap[ m_curPlane];
	}

	UpdateData( FALSE);	

   m_pAcq->SetParameter(m_brightnessPrm[ m_curPlane], m_brightness * 1000);
}

void CCompDlg::SetContrast( void)
{
	if( m_contrast < m_contrastMinCap[ m_curPlane])
	{
		m_contrast= m_contrastMinCap[ m_curPlane];
	}
	else if( m_contrast > m_contrastMaxCap[ m_curPlane])
	{
		m_contrast= m_contrastMaxCap[ m_curPlane];
	}

	UpdateData( FALSE);	

   m_pAcq->SetParameter(m_contrastPrm[ m_curPlane], m_contrast * 1000);
}


void CCompDlg::SetHue( void)
{
	if( m_hue < m_hueMinCap)
	{
		m_hue= m_hueMinCap;
	}
	else if( m_hue > m_hueMaxCap)
	{
		m_hue =  m_hueMaxCap;
	}

	UpdateData( FALSE);	

   m_pAcq->SetParameter(m_huePrm, m_hue * 1000);
}

void CCompDlg::SetSaturation( void)
{
	if( m_saturation < m_saturationMinCap)
	{
		m_saturation = m_saturationMinCap;
	}
	else if( m_saturation >  m_saturationMaxCap)
	{
		m_saturation=  m_saturationMaxCap;
	}

	UpdateData( FALSE);	

   m_pAcq->SetParameter(m_saturationPrm, m_saturation * 1000);
}

void CCompDlg::SetSharpness( void)
{
	if( m_sharpness < m_sharpnessMinCap)
	{
		m_sharpness = m_sharpnessMinCap;
	}
	else if( m_sharpness > m_sharpnessMaxCap)
	{
		m_sharpness =  m_sharpnessMaxCap;
	}

	UpdateData( FALSE);	

   m_pAcq->SetParameter(m_sharpnessPrm, m_sharpness);
}


void CCompDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);

	if( (CSliderCtrl*)pScrollBar == &m_slider_contrast)
	{
		switch( nSBCode)
		{
			case SB_THUMBPOSITION:
			case SB_THUMBTRACK:
				m_contrast= nPos;
				break;
			case SB_LINELEFT:
				m_contrast -= 10;
				break;
			case SB_LINERIGHT:
				m_contrast += 10;
				break;
			case SB_PAGELEFT:
				m_contrast-= 10;
				break;
			case SB_PAGERIGHT:
				m_contrast+= 10;
				break;
			default:
				return;
		}
		SetContrast();
	}

	if( (CSliderCtrl*)pScrollBar == &m_slider_brightness)
	{
		switch( nSBCode)
		{
			case SB_THUMBPOSITION:
			case SB_THUMBTRACK:
				m_brightness= nPos;
				break;
			case SB_LINELEFT:
				m_brightness -= 10;
				break;
			case SB_LINERIGHT:
				m_brightness += 10;
				break;
			case SB_PAGELEFT:
				m_brightness-= 10;
				break;
			case SB_PAGERIGHT:
				m_brightness+= 10;
				break;
			default:
				return;
		}

		SetBrightness();
	}

	if( (CSliderCtrl*)pScrollBar == &m_slider_hue)
	{
		switch( nSBCode)
		{
			case SB_THUMBPOSITION:
			case SB_THUMBTRACK:
				m_hue = nPos;
				break;
			case SB_LINELEFT:
				m_hue -= 10;
				break;
			case SB_LINERIGHT:
				m_hue += 10;
				break;
			case SB_PAGELEFT:
				m_hue -= 10;
				break;
			case SB_PAGERIGHT:
				m_hue += 10;
				break;
			default:
				return;
		}
		SetHue();
	}

	if( (CSliderCtrl*)pScrollBar == &m_slider_saturation)
	{
		switch( nSBCode)
		{
			case SB_THUMBPOSITION:
			case SB_THUMBTRACK:
				m_saturation = nPos;
				break;
			case SB_LINELEFT:
				m_saturation -= 10;
				break;
			case SB_LINERIGHT:
				m_saturation += 10;
				break;
			case SB_PAGELEFT:
				m_saturation -= 10;
				break;
			case SB_PAGERIGHT:
				m_saturation += 10;
				break;
			default:
				return;
		}
		SetSaturation();
	}

	if( (CSliderCtrl*)pScrollBar == &m_slider_sharpness)
	{
		switch( nSBCode)
		{
			case SB_THUMBPOSITION:
			case SB_THUMBTRACK:
				m_sharpness = nPos;
				break;
			case SB_LINELEFT:
				m_sharpness--;
				break;
			case SB_LINERIGHT:
				m_sharpness++;
				break;
			case SB_PAGELEFT:
				m_sharpness--;
				break;
			case SB_PAGERIGHT:
				m_sharpness++;
				break;
			default:
				return;
		}
		SetSharpness();
	}

	Snap();

	UpdateData( FALSE);
}

void CCompDlg::OnRed() 
{
	// Save current values
	m_contrastValues[ m_curPlane] = m_contrast;
	m_brightnessValues[ m_curPlane] = m_brightness;

	m_curPlane = PLANE_RED;

	UpdateWindow();
}

void CCompDlg::OnGreen() 
{
	// Save current values
	m_contrastValues[ m_curPlane] = m_contrast;
	m_brightnessValues[ m_curPlane] = m_brightness;

	m_curPlane = PLANE_GREEN;

	UpdateWindow();
}

void CCompDlg::OnBlue() 
{
	// Save current values
	m_contrastValues[ m_curPlane] = m_contrast;
	m_brightnessValues[ m_curPlane] = m_brightness;

	m_curPlane = PLANE_BLUE;

	UpdateWindow();
}


void CCompDlg::OnKillfocusEditContrast() 
{
	UpdateData(TRUE);

	SetContrast();
	m_slider_contrast.SetPos( m_contrast);
	Snap();
}

void CCompDlg::OnKillfocusEditBrightness() 
{
	UpdateData(TRUE);
	SetBrightness();
	m_slider_brightness.SetPos( m_brightness);
	Snap();
}

void CCompDlg::OnKillfocusEditHue() 
{
	UpdateData(TRUE);
	SetHue();
	m_slider_hue.SetPos( m_hue);
	Snap();
}

void CCompDlg::OnKillfocusEditSaturation() 
{
	UpdateData(TRUE);
	SetSaturation();
	m_slider_saturation.SetPos( m_saturation);
	Snap();
}


void CCompDlg::OnKillfocusEditSharpness() 
{
	UpdateData(TRUE);
	SetSharpness();
	m_slider_sharpness.SetPos( m_sharpness);
	Snap();
}

void CCompDlg::OnDefault() 
{
	m_contrast = 100;
	m_brightness = 0;

	// Put back initial values
	if( m_curPlane == PLANE_MONO)
	{
		if( m_contrastCap)
		{
			SetContrast();
		}

		if( m_brightnessCap)
		{
			SetBrightness();
		}
	}
	else
	{
		int curPlane;

		curPlane = m_curPlane;

		for( m_curPlane = PLANE_RED; m_curPlane <= PLANE_BLUE; m_curPlane++)
		{
			if( m_contrastColorCap)
			{
				m_contrastValues[ m_curPlane] = 100;
				SetContrast();
			}

			if( m_brightnessColorCap)
			{
				m_brightnessValues[ m_curPlane] = 0;
				SetBrightness();
			}
		}

		m_curPlane = curPlane;
	}

	if( m_hueCap)
	{
		m_hue = 0;
		SetHue();
	}

	if( m_saturationCap)
	{
		m_saturation = 100;
		SetSaturation();
	}

	if( m_sharpnessCap)
	{
		m_sharpness = 0;
		SetSharpness();
	}

	UpdateWindow();
	Snap();
}

