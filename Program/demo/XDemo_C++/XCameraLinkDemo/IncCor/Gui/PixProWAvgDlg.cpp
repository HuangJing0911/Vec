// PixProWAvgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SapClassBasic.h"
#include "SapClassGui.h"
#include "PixProWAvgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPixProWAvgDlg dialog


CPixProWAvgDlg::CPixProWAvgDlg(CWnd* pParent, PCORPPL_FCT_PROP properties, SapPixProParams *pParams)
	: CDialog(CPixProWAvgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPixProWAvgDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Properties = *properties;
	m_pParams = pParams;
	m_BankId = (m_Properties.bankId & CORPPL_FRAME_BUFFER_BANK0) ? 
		CORPPL_FRAME_BUFFER_BANK0 : CORPPL_FRAME_BUFFER_BANK1;
	m_bRefImage = FALSE;
	m_Factor = 0;
}


void CPixProWAvgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPixProWAvgDlg)
	DDX_Control(pDX, IDC_SCG_PIXPRO_WEIGHTING_FACTOR, m_cbFactor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPixProWAvgDlg, CDialog)
	//{{AFX_MSG_MAP(CPixProWAvgDlg)
	ON_BN_CLICKED(IDC_SCG_PIXPRO_SDRAM_BANK0, OnPixproSdramBank0)
	ON_BN_CLICKED(IDC_SCG_PIXPRO_SDRAM_BANK1, OnPixproSdramBank1)
	ON_BN_CLICKED(IDC_SCG_PIXPRO_WEIGHTED_AVG, OnPixproWeightedAvg)
	ON_BN_CLICKED(IDC_SCG_PIXPRO_REF_IMAGE, OnPixproRefImage)
	ON_CBN_SELCHANGE(IDC_SCG_PIXPRO_WEIGHTING_FACTOR, OnSelchangeFactor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPixProWAvgDlg message handlers

BOOL CPixProWAvgDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Enable valid bank selection
	GetDlgItem(IDC_SCG_PIXPRO_SDRAM_BANK0)->EnableWindow(m_Properties.bankId & CORPPL_FRAME_BUFFER_BANK0);
	GetDlgItem(IDC_SCG_PIXPRO_SDRAM_BANK1)->EnableWindow(m_Properties.bankId & CORPPL_FRAME_BUFFER_BANK1);

	// Check selected bank
	if (m_BankId & CORPPL_FRAME_BUFFER_BANK0)
	{
		((CButton *)GetDlgItem(IDC_SCG_PIXPRO_SDRAM_BANK0))->SetCheck(TRUE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_SCG_PIXPRO_SDRAM_BANK1))->SetCheck(TRUE);
	}

	// Select default operation
	((CButton *)GetDlgItem(IDC_SCG_PIXPRO_WEIGHTED_AVG))->SetCheck(TRUE);

	// Set factors in combo
	for (int i=0; i < m_cbFactor.GetCount(); i++)
	{
		int factor = 1000 / (1 << (i + 1));
		m_cbFactor.SetItemData(i, factor);
	}
	m_cbFactor.SetCurSel(2);
	OnSelchangeFactor();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPixProWAvgDlg::OnPixproSdramBank0() 
{
	m_BankId = CORPPL_FRAME_BUFFER_BANK0; 
}

void CPixProWAvgDlg::OnPixproSdramBank1() 
{
	m_BankId = CORPPL_FRAME_BUFFER_BANK1; 
}

void CPixProWAvgDlg::OnPixproWeightedAvg() 
{
	m_bRefImage = FALSE;
}

void CPixProWAvgDlg::OnPixproRefImage() 
{
	m_bRefImage = TRUE;
}

void CPixProWAvgDlg::OnSelchangeFactor() 
{
   m_Factor = (int) m_cbFactor.GetItemData(m_cbFactor.GetCurSel());
}

void CPixProWAvgDlg::OnOK()
{
	CDialog::OnOK();

	m_pParams->SetBankId(m_BankId);
	m_pParams->SetRefImage(m_bRefImage);
	m_pParams->SetWeightingFactor(m_Factor);
}


