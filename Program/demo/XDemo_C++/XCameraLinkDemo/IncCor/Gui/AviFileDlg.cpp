// AviFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SapClassBasic.h"
#include "SapClassGui.h"
#include "AviFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AviFileDlg dialog


CAviFileDlg::CAviFileDlg(CWnd* pParent, BOOL bLoad, int firstFrame, int compressionLevel, int compressionType)
	: CDialog(CAviFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAviFileDlg)
	//}}AFX_DATA_INIT

	m_App = AfxGetApp();
	m_FirstFrame = m_App->GetProfileInt("SapFile", "Avi First Frame", firstFrame);
	m_CompressionLevel = m_App->GetProfileInt("SapFile", "Avi Compression Level", compressionLevel);
	m_CompressionType = m_App->GetProfileInt("SapFile", "Avi Compression Type", compressionType);

	m_bLoad = bLoad;
}


void CAviFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAviFileDlg)
	DDX_Text(pDX, IDC_SCG_AVI_EDIT_FIRST_FRAME, m_FirstFrame);
	DDX_Text(pDX, IDC_SCG_AVI_EDIT_COMPRESSION_LEVEL, m_CompressionLevel);
	DDV_MinMaxInt(pDX, m_CompressionLevel, 1, 99);
	DDX_CBIndex(pDX, IDC_SCG_AVI_COMPRESSION_TYPE, m_CompressionType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAviFileDlg, CDialog)
	//{{AFX_MSG_MAP(CAviFileDlg)
	ON_WM_VSCROLL()
	ON_EN_UPDATE(IDC_SCG_AVI_EDIT_FIRST_FRAME, OnUpdateAviEditFirstFrame)
	ON_EN_UPDATE(IDC_SCG_AVI_EDIT_COMPRESSION_LEVEL, OnUpdateAviEditCompressionLevel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAviFileDlg message handlers

void CAviFileDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);

	if (pScrollBar->GetDlgCtrlID() == IDC_SCG_AVI_SCROLLBAR_FIRST_FRAME)
	{
		switch(nSBCode)
		{
			case SB_LINEDOWN:
				if (m_FirstFrame > 0) m_FirstFrame--;
				break;
			case SB_LINEUP:
				m_FirstFrame++;
				break;
			default:
				break;
		}

   	UpdateData(FALSE);
	}

	if (pScrollBar->GetDlgCtrlID() == IDC_SCG_AVI_SCROLLBAR_COMPRESSION_LEVEL)
	{
		switch(nSBCode)
		{
			case SB_LINEDOWN:
				if (m_CompressionLevel > 1) m_CompressionLevel--;
				break;
			case SB_LINEUP:
				if (m_CompressionLevel < 99) m_CompressionLevel++;
				break;
			default:
				break;
		}

   	UpdateData(FALSE);
	}
}

void CAviFileDlg::OnUpdateAviEditFirstFrame() 
{
	UpdateData(TRUE);
}

void CAviFileDlg::OnUpdateAviEditCompressionLevel() 
{
	UpdateData(TRUE);
}

BOOL CAviFileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Enable/disable controls
	GetDlgItem(IDC_SCG_AVI_EDIT_FIRST_FRAME)->EnableWindow(m_bLoad);
	GetDlgItem(IDC_SCG_AVI_SCROLLBAR_FIRST_FRAME)->EnableWindow(m_bLoad);
	GetDlgItem(IDC_SCG_AVI_EDIT_COMPRESSION_LEVEL)->EnableWindow(!m_bLoad);
	GetDlgItem(IDC_SCG_AVI_SCROLLBAR_COMPRESSION_LEVEL)->EnableWindow(!m_bLoad);
	GetDlgItem(IDC_SCG_AVI_COMPRESSION_TYPE)->EnableWindow(!m_bLoad);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAviFileDlg::OnOK() 
{
	CDialog::OnOK();

	m_App->WriteProfileInt("SapFile", "Avi First Frame", m_FirstFrame);
	m_App->WriteProfileInt("SapFile", "Avi Compression Level", m_CompressionLevel);
	m_App->WriteProfileInt("SapFile", "Avi Compression Type", m_CompressionType);
}
