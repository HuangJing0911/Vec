// PixProAbsSubDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SapClassBasic.h"
#include "SapClassGui.h"
#include "PixProAbsSubDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPixProAbsSubDlg dialog


CPixProAbsSubDlg::CPixProAbsSubDlg(CWnd* pParent, PCORPPL_FCT_PROP properties, SapPixProParams *pParams)
	: CDialog(CPixProAbsSubDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPixProAbsSubDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Properties = *properties;
	m_pParams = pParams;
	m_BankId = (m_Properties.bankId & CORPPL_FRAME_BUFFER_BANK0) ? 
		CORPPL_FRAME_BUFFER_BANK0 : CORPPL_FRAME_BUFFER_BANK1;
	m_bRefImage = FALSE;
}


void CPixProAbsSubDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPixProAbsSubDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPixProAbsSubDlg, CDialog)
	//{{AFX_MSG_MAP(CPixProAbsSubDlg)
	ON_BN_CLICKED(IDC_SCG_PIXPRO_SDRAM_BANK0, OnPixproSdramBank0)
	ON_BN_CLICKED(IDC_SCG_PIXPRO_SDRAM_BANK1, OnPixproSdramBank1)
	ON_BN_CLICKED(IDC_SCG_PIXPRO_ABS_SUB, OnPixproAbsSub)
	ON_BN_CLICKED(IDC_SCG_PIXPRO_REF_IMAGE, OnPixproRefImage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPixProAbsSubDlg message handlers

BOOL CPixProAbsSubDlg::OnInitDialog() 
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
	((CButton *)GetDlgItem(IDC_SCG_PIXPRO_ABS_SUB))->SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPixProAbsSubDlg::OnPixproSdramBank0() 
{
	m_BankId = CORPPL_FRAME_BUFFER_BANK0; 
}

void CPixProAbsSubDlg::OnPixproSdramBank1() 
{
	m_BankId = CORPPL_FRAME_BUFFER_BANK1; 
}

void CPixProAbsSubDlg::OnPixproAbsSub() 
{
	m_bRefImage = FALSE;
}

void CPixProAbsSubDlg::OnPixproRefImage() 
{
	m_bRefImage = TRUE;
}

void CPixProAbsSubDlg::OnOK()
{
	CDialog::OnOK();

	m_pParams->SetBankId(m_BankId);
	m_pParams->SetRefImage(m_bRefImage);
}

