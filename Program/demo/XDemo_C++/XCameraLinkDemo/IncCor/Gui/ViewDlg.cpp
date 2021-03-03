// ViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SapClassBasic.h"
#include "SapClassGui.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewDlg dialog


void CViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewDlg)
	DDX_Control(pDX, IDC_SCG_VIEW_SPIN_SCALE_WIDTH_FACTOR, m_scaleWidthFactorSpinCtrl);
	DDX_Control(pDX, IDC_SCG_VIEW_SPIN_SCALE_WIDTH, m_scaleWidthSpinCtrl);
	DDX_Control(pDX, IDC_SCG_VIEW_SPIN_SCALE_HEIGHT, m_scaleHeightSpinCtrl);
	DDX_Control(pDX, IDC_SCG_VIEW_SLIDER_RANGE, m_slider_range);
	DDX_Text(pDX, IDC_SCG_VIEW_EDIT_RANGE, m_Range);
	DDX_Check(pDX, IDC_SCG_VIEW_LOCK_ASPECT_RATIO, m_bLockAspectRatio);
	DDX_Text(pDX, IDC_SCG_VIEW_SCALE_HEIGHT, m_scaleHeight);
	DDX_Text(pDX, IDC_SCG_VIEW_SCALE_WIDTH, m_scaleWidth);
	DDX_Text(pDX, IDC_SCG_VIEW_SCALE_HEIGHT_FACTOR, m_scaleHeightFactor);
	DDX_Text(pDX, IDC_SCG_VIEW_SCALE_WIDTH_FACTOR, m_scaleWidthFactor);
	//}}AFX_DATA_MAP

	DDV_MinMaxUInt(pDX, m_Range, m_RangeMin, m_RangeMax);
	DDV_MinMaxDWord(pDX, m_scaleWidth, 1, UINT_MAX);
	DDV_MinMaxDWord(pDX, m_scaleHeight, 1, UINT_MAX);
	DDV_MinMaxFloat(pDX, m_scaleWidthFactor, 0.001f, 100000000.0f);
	DDV_MinMaxFloat(pDX, m_scaleHeightFactor, 0.001f, 100000000.0f);
}


BEGIN_MESSAGE_MAP(CViewDlg, CDialog)
	//{{AFX_MSG_MAP(CViewDlg)
	ON_EN_UPDATE(IDC_SCG_VIEW_EDIT_RANGE, OnUpdateEditViewRange)
	ON_WM_HSCROLL()
	ON_EN_UPDATE(IDC_SCG_VIEW_SCALE_HEIGHT, OnUpdateScaleHeight)
	ON_EN_UPDATE(IDC_SCG_VIEW_SCALE_WIDTH, OnUpdateScaleWidth)
	ON_EN_UPDATE(IDC_SCG_VIEW_SCALE_WIDTH_FACTOR, OnUpdateScaleWidthFactor)
	ON_EN_UPDATE(IDC_SCG_VIEW_SCALE_HEIGHT_FACTOR, OnUpdateScaleHeightFactor)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SCG_VIEW_SPIN_SCALE_HEIGHT, OnDeltaposSpinScaleHeight)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SCG_VIEW_SPIN_SCALE_HEIGHT_FACTOR, OnDeltaposSpinScaleHeightFactor)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SCG_VIEW_SPIN_SCALE_WIDTH, OnDeltaposSpinScaleWidth)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SCG_VIEW_SPIN_SCALE_WIDTH_FACTOR, OnDeltaposSpinScaleWidthFactor)
	ON_BN_CLICKED(IDC_SCG_VIEW_LOCK_ASPECT_RATIO, OnLockAspectRatio)
	ON_BN_CLICKED(IDC_SCG_VIEW_SCALING_NONE, OnScalingNone)
	ON_BN_CLICKED(IDC_SCG_VIEW_SCALING_FIT_TO_WINDOW, OnScalingFitToWindow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewDlg message handlers
CViewDlg::CViewDlg( CWnd *pParent, SapView *pView)
	: CDialog(CViewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewDlg)
	m_bLockAspectRatio = FALSE;
	m_scaleHeight = 0;
	m_scaleWidth = 0;
	m_scaleHeightFactor = 0.0f;
	m_scaleWidthFactor = 0.0f;
	//}}AFX_DATA_INIT

	m_App = AfxGetApp();
	
	m_pView = pView;
	m_pView->GetRangeMinMax(&m_RangeMin, &m_RangeMax);
	m_Range = m_pView->GetRange();

	const SapViewScaleParams scaleParams= m_pView->GetScaleParamsSrc();

	m_scaleWidthFactor = scaleParams.ZoomHorz() * 100.0f;
	m_scaleHeightFactor= scaleParams.ZoomVert() * 100.0f;

	m_scaleWidth = (DWORD)floor(m_pView->GetBuffer()->GetWidth() * m_scaleWidthFactor/100 + 0.5f);
	m_scaleHeight= (DWORD)floor(m_pView->GetBuffer()->GetHeight() * m_scaleHeightFactor/100 + 0.5f);
}

BOOL CViewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_slider_range.SetRange(m_RangeMin, m_RangeMax);
	m_slider_range.SetTicFreq(1);
	m_slider_range.SetPageSize(1);
	m_slider_range.SetPos(m_Range);
	m_RangeInit = m_Range;

	GetDlgItem( IDC_SCG_VIEW_EDIT_RANGE)->EnableWindow( m_pView->HasRange());
	GetDlgItem( IDC_SCG_VIEW_SLIDER_RANGE)->EnableWindow( m_pView->HasRange());

	LoadSettings();

	UpdateData( FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CViewDlg::LoadSettings() 
{
	// Read view settings
	m_bLockAspectRatio = m_App->GetProfileInt("SapView", "Aspect Ratio", 1);
}

void CViewDlg::SaveSettings()
{
	// Write view settings
	m_App->WriteProfileInt("SapView", "Aspect Ratio", m_bLockAspectRatio);
}

void CViewDlg::OnUpdateEditViewRange()
{
	int oldRange = m_Range;

	UpdateData(TRUE);

	if ((m_Range < m_RangeMin) || (m_Range > m_RangeMax))
		m_Range = oldRange;

	SetRange();
	m_slider_range.SetPos(m_Range);
}

void CViewDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);

	if ((CSliderCtrl*)pScrollBar == &m_slider_range)
	{
		switch( nSBCode)
		{
			case SB_THUMBPOSITION:
			case SB_THUMBTRACK:
				m_Range= nPos;
				break;
			case SB_LINELEFT:
			case SB_PAGELEFT:
				if (m_Range > m_RangeMin) m_Range -= 1;
				break;
			case SB_PAGERIGHT:
			case SB_LINERIGHT:
				if (m_Range < m_RangeMax) m_Range += 1;
				break;
			default:
				return;
		}

		SetRange();
	}
}

void CViewDlg::SetRange( void)
{
	m_pView->SetRange(m_Range);
	m_pView->Show();

	UpdateData( FALSE);	
}

void CViewDlg::OnUpdateScaleHeight() 
{
	UpdateData( TRUE);	

	m_scaleHeightFactor= 100.0f * m_scaleHeight/m_pView->GetBuffer()->GetHeight();
	if( m_bLockAspectRatio)
	{
		m_scaleWidthFactor= m_scaleHeightFactor;
		m_scaleWidth= (DWORD)floor(m_scaleWidthFactor * m_pView->GetBuffer()->GetWidth()/100 + 0.5f);
	}

	UpdateData( FALSE);
}

void CViewDlg::OnUpdateScaleHeightFactor() 
{
	UpdateData( TRUE);
	
	m_scaleHeight= (DWORD)floor(m_scaleHeightFactor * m_pView->GetBuffer()->GetHeight()/100 + 0.5f);

	if( m_bLockAspectRatio)
	{
		m_scaleWidthFactor= m_scaleHeightFactor;
		m_scaleWidth= (DWORD)floor(m_scaleWidthFactor * m_pView->GetBuffer()->GetWidth()/100 + 0.5f);
	}

	UpdateData( FALSE);
}

void CViewDlg::OnUpdateScaleWidth() 
{
	UpdateData( TRUE);	

	m_scaleWidthFactor= 100.0f * m_scaleWidth/ m_pView->GetBuffer()->GetWidth();
	if( m_bLockAspectRatio)
	{
		m_scaleHeightFactor= m_scaleWidthFactor;
		m_scaleHeight= (DWORD)floor(m_scaleHeightFactor * m_pView->GetBuffer()->GetHeight()/100 + 0.5f);
	}

	UpdateData( FALSE);
}

void CViewDlg::OnUpdateScaleWidthFactor() 
{
	UpdateData( TRUE);		

	m_scaleWidth= (DWORD)floor(m_scaleWidthFactor * m_pView->GetBuffer()->GetWidth()/100 + 0.5f);
	if( m_bLockAspectRatio)
	{
		m_scaleHeightFactor= m_scaleWidthFactor;
		m_scaleHeight= (DWORD)floor(m_scaleHeightFactor * m_pView->GetBuffer()->GetHeight()/100 + 0.5f);
	}

	UpdateData( FALSE);
}

void CViewDlg::OnDeltaposSpinScaleHeight(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	if(pNMUpDown->iDelta > 0)
	{
		m_scaleHeight = max(1, m_scaleHeight - 1);
	}
	else
	{
		m_scaleHeight = m_scaleHeight + 1;
	}

	m_scaleHeightFactor= 100.0f * m_scaleHeight/ m_pView->GetBuffer()->GetHeight();

	if( m_bLockAspectRatio)
	{
		m_scaleWidthFactor= m_scaleHeightFactor;
		m_scaleWidth= (DWORD)floor(m_scaleWidthFactor * m_pView->GetBuffer()->GetWidth()/100 + 0.5f);
	}

	UpdateData( FALSE);

	*pResult = 0;
}

void CViewDlg::OnDeltaposSpinScaleHeightFactor(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if(pNMUpDown->iDelta > 0)
	{
		m_scaleHeightFactor = max(1, m_scaleHeightFactor - 1);
	}
	else
	{
		m_scaleHeightFactor = m_scaleHeightFactor + 1;
	}

	m_scaleHeight= (DWORD)floor(m_scaleHeightFactor * m_pView->GetBuffer()->GetHeight()/100 + 0.5f);

	if( m_bLockAspectRatio)
	{
		m_scaleWidthFactor= m_scaleHeightFactor;
		m_scaleWidth= (DWORD)floor(m_scaleWidthFactor * m_pView->GetBuffer()->GetWidth()/100 + 0.5f);
	}

	UpdateData( FALSE);

	*pResult = 0;
}

void CViewDlg::OnDeltaposSpinScaleWidth(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if(pNMUpDown->iDelta > 0)
	{
		m_scaleWidth = max(1, m_scaleWidth - 1);
	}
	else
	{
		m_scaleWidth = m_scaleWidth + 1;
	}

	m_scaleWidthFactor= 100.0f * m_scaleWidth / m_pView->GetBuffer()->GetWidth();

	if( m_bLockAspectRatio)
	{
		m_scaleHeightFactor= m_scaleWidthFactor;
		m_scaleHeight= (DWORD)floor(m_scaleHeightFactor * m_pView->GetBuffer()->GetHeight()/100 + 0.5f);
	}

	UpdateData( FALSE);

	*pResult = 0;
}

void CViewDlg::OnDeltaposSpinScaleWidthFactor(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	if(pNMUpDown->iDelta > 0)
	{
		m_scaleWidthFactor = max(1, m_scaleWidthFactor - 1);
	}
	else
	{
		m_scaleWidthFactor = m_scaleWidthFactor + 1;
	}

	m_scaleWidth= (DWORD)floor(m_scaleWidthFactor * m_pView->GetBuffer()->GetWidth()/100 + 0.5f);

	if( m_bLockAspectRatio)
	{
		m_scaleHeightFactor= m_scaleWidthFactor;
		m_scaleHeight= (DWORD)floor(m_scaleHeightFactor * m_pView->GetBuffer()->GetHeight()/100 + 0.5f);
	}

	UpdateData( FALSE);

	*pResult = 0;
}

void CViewDlg::OnLockAspectRatio() 
{
	UpdateData( TRUE);
	
	if( m_bLockAspectRatio)
	{
		m_scaleHeightFactor= m_scaleWidthFactor;
		m_scaleHeight= (DWORD)floor(m_scaleHeightFactor * m_pView->GetBuffer()->GetHeight()/100 + 0.5f);

		UpdateData( FALSE);
	}
}

void CViewDlg::OnScalingNone() 
{
	m_scaleWidthFactor = 100.0f;
	m_scaleHeightFactor= 100.0f;

	m_scaleWidth = (DWORD)floor(m_pView->GetBuffer()->GetWidth()  * m_scaleWidthFactor/100 + 0.5f);
	m_scaleHeight= (DWORD)floor(m_pView->GetBuffer()->GetHeight() * m_scaleHeightFactor/100 + 0.5f);

	UpdateData( FALSE);
}

void CViewDlg::OnScalingFitToWindow() 
{
	int viewWidth;
	int viewHeight;

	m_pView->GetViewArea( &viewWidth, &viewHeight);

	m_scaleWidthFactor = 100.0f * viewWidth/m_pView->GetBuffer()->GetWidth();
	m_scaleHeightFactor= 100.0f * viewHeight/m_pView->GetBuffer()->GetHeight();

	if( m_bLockAspectRatio)
	{
		if( m_scaleWidthFactor < m_scaleHeightFactor)
		{
			m_scaleHeightFactor= m_scaleWidthFactor;
		}
		else
		{
			m_scaleWidthFactor= m_scaleHeightFactor;
		}
	}

	m_scaleWidth = (DWORD)floor(m_pView->GetBuffer()->GetWidth()  * m_scaleWidthFactor/100 + 0.5f);
	m_scaleHeight= (DWORD)floor(m_pView->GetBuffer()->GetHeight() * m_scaleHeightFactor/100 + 0.5f);

	m_scaleWidthFactor = 100.0f * m_scaleWidth / m_pView->GetBuffer()->GetWidth();
	m_scaleHeightFactor= 100.0f * m_scaleHeight / m_pView->GetBuffer()->GetHeight();

	UpdateData( FALSE);
}

void CViewDlg::OnOK() 
{
	m_pView->SetScalingMode( m_scaleWidthFactor/100.0f, m_scaleHeightFactor/100.0f);
	
	SaveSettings();
	
	CDialog::OnOK();
}

void CViewDlg::OnCancel() 
{
	m_Range = m_RangeInit;
	SetRange();
	
	CDialog::OnCancel();
}



