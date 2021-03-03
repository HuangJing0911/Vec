// TiffFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SapClassBasic.h"
#include "SapClassGui.h"
#include "TiffFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTiffFileDlg dialog


CTiffFileDlg::CTiffFileDlg(CWnd* pParent, int compressionLevel, int compressionType)
	: CDialog(CTiffFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTiffFileDlg)
	//}}AFX_DATA_INIT

	m_App = AfxGetApp();
	m_CompressionLevel = m_App->GetProfileInt("SapFile", "Tiff Compression Level", compressionLevel);
	m_CompressionType = m_App->GetProfileInt("SapFile", "Tiff Compression Type", compressionType);
}


void CTiffFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTiffFileDlg)
	DDX_Text(pDX, IDC_SCG_TIFF_EDIT_COMPRESSION_LEVEL, m_CompressionLevel);
	DDV_MinMaxInt(pDX, m_CompressionLevel, 1, 100);
	DDX_CBIndex(pDX, IDC_SCG_TIFF_COMPRESSION_TYPE, m_CompressionType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTiffFileDlg, CDialog)
	//{{AFX_MSG_MAP(CTiffFileDlg)
	ON_WM_VSCROLL()
	ON_EN_UPDATE(IDC_SCG_TIFF_EDIT_COMPRESSION_LEVEL, OnUpdateTiffEditCompressionLevel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTiffFileDlg message handlers

void CTiffFileDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);

	if (pScrollBar->GetDlgCtrlID() == IDC_SCG_TIFF_SCROLLBAR_COMPRESSION_LEVEL)
	{
		switch(nSBCode)
		{
			case SB_LINEDOWN:
				if (m_CompressionLevel > 1) m_CompressionLevel--;
				break;
			case SB_LINEUP:
				if (m_CompressionLevel < 100) m_CompressionLevel++;
				break;
			default:
				break;
		}

   	UpdateData(FALSE);
	}
}

void CTiffFileDlg::OnUpdateTiffEditCompressionLevel() 
{
	UpdateData(TRUE);
}

void CTiffFileDlg::OnOK() 
{
	CDialog::OnOK();

	m_App->WriteProfileInt("SapFile", "Tiff Compression Level", m_CompressionLevel);
	m_App->WriteProfileInt("SapFile", "Tiff Compression Type", m_CompressionType);
}
