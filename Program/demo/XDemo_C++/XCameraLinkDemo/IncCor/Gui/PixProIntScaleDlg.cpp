// PixProIntScaleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SapClassBasic.h"
#include "SapClassGui.h"
#include "PixProIntScaleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPixProIntScaleDlg dialog


CPixProIntScaleDlg::CPixProIntScaleDlg(CWnd* pParent, PCORPPL_FCT_PROP properties, SapPixProParams *pParams)
	: CDialog(CPixProIntScaleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPixProIntScaleDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Properties = *properties;
	m_pParams = pParams;
	m_BankId = (m_Properties.bankId & CORPPL_FRAME_BUFFER_BANK0) ? 
		CORPPL_FRAME_BUFFER_BANK0 : CORPPL_FRAME_BUFFER_BANK1;
	m_Count = 0;
	m_Factor = 5;
}


void CPixProIntScaleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPixProIntScaleDlg)
	DDX_Control(pDX, IDC_SCG_PIXPRO_INTEGRATE_COUNT, m_cbCount);
	DDX_CBIndex(pDX, IDC_SCG_PIXPRO_SCALING_FACTOR, m_Factor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPixProIntScaleDlg, CDialog)
	//{{AFX_MSG_MAP(CPixProIntScaleDlg)
	ON_BN_CLICKED(IDC_SCG_PIXPRO_SDRAM_BANK0, OnPixproSdramBank0)
	ON_BN_CLICKED(IDC_SCG_PIXPRO_SDRAM_BANK1, OnPixproSdramBank1)
	ON_CBN_SELCHANGE(IDC_SCG_PIXPRO_INTEGRATE_COUNT, OnSelchangeCount)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPixProIntScaleDlg message handlers

BOOL CPixProIntScaleDlg::OnInitDialog() 
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

	// Initialize count combo
	for (int i=0; i < m_cbCount.GetCount(); i++)
	{
		m_cbCount.SetItemData(i, 4 << i);
	}
	m_cbCount.SetCurSel(3);
	OnSelchangeCount();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPixProIntScaleDlg::OnPixproSdramBank0() 
{
	m_BankId = CORPPL_FRAME_BUFFER_BANK0; 
}

void CPixProIntScaleDlg::OnPixproSdramBank1() 
{
	m_BankId = CORPPL_FRAME_BUFFER_BANK1; 
}

void CPixProIntScaleDlg::OnSelchangeCount() 
{
   m_Count = (int) m_cbCount.GetItemData(m_cbCount.GetCurSel());
}

void CPixProIntScaleDlg::OnOK()
{
	CDialog::OnOK();

	m_pParams->SetBankId(m_BankId);
	m_pParams->SetIntegrateCount(m_Count);
	m_pParams->SetIntegrateFactor(m_Factor);
}

