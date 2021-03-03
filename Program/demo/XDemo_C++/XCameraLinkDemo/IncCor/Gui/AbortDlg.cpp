// Abort.cpp : implementation file
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
// CAbortDlg dialog


CAbortDlg::CAbortDlg(CWnd* pParent, SapTransfer *pXfer, int timeout)
	: CDialog(CAbortDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAbortDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pXfer = pXfer;
	m_timeout = timeout;
}


void CAbortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAbortDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAbortDlg, CDialog)
	//{{AFX_MSG_MAP(CAbortDlg)
	ON_BN_CLICKED(IDABORT, OnAbort)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAbortDlg message handlers
BOOL CAbortDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// If transfer is already finished, close immediately
	if (m_pXfer->Wait(0))
	{
		OnOK();
		return TRUE;
	}

	// Get default window placement
	GetWindowRect( &m_rect);

	// Hide window
	SetWindowPos( NULL, -100000, -100000, 0, 0, SWP_NOZORDER | SWP_SHOWWINDOW | SWP_NOSIZE);
	m_bShow = FALSE;

	// Set timer to a short time interval
	SetTimer( 0, 33, (TIMERPROC)NULL);

	// Get current time
	m_time0 = GetTickCount();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAbortDlg::OnAbort() 
{
	KillTimer(0);
	CAbortDlg::OnCancel();	
}

void CAbortDlg::OnTimer(UINT_PTR nIDEvent) 
{
	// If transfer is already finished, close immediately
	if (m_pXfer->Wait(0))
	{
		KillTimer(0);
		OnOK();
		return;
	}

	// Show window if time is out
	if (!m_bShow && (GetTickCount() - m_time0 > m_timeout))
	{
		SetWindowPos(NULL, m_rect.left, m_rect.top, 0, 0, SWP_NOZORDER | SWP_SHOWWINDOW | SWP_NOSIZE);
		m_bShow = TRUE;
	}

	CDialog::OnTimer(nIDEvent);
}
