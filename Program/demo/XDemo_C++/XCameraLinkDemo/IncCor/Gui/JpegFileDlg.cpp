// JpegFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SapClassBasic.h"
#include "SapClassGui.h"
#include "JpegFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJpegFileDlg dialog


CJpegFileDlg::CJpegFileDlg(CWnd* pParent, int quality)
	: CDialog(CJpegFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJpegFileDlg)
	//}}AFX_DATA_INIT

	m_App = AfxGetApp();
	m_FileJpegQuality = m_App->GetProfileInt("SapFile", "Jpeg Quality", quality);
}


void CJpegFileDlg::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void CJpegFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJpegFileDlg)
	DDX_Text(pDX, IDC_SCG_JPG_EDIT_QUALITY, m_FileJpegQuality);
	DDV_MinMaxUInt(pDX, m_FileJpegQuality, 1, 100);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CJpegFileDlg, CDialog)
	//{{AFX_MSG_MAP(CJpegFileDlg)
	ON_WM_VSCROLL()
	ON_EN_UPDATE(IDC_SCG_JPG_EDIT_QUALITY, OnUpdateJpgEditQuality)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CJpegFileDlg, CDialog)
	//{{AFX_DISPATCH_MAP(CJpegFileDlg)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IJpegFileDlg to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {68CEC0E2-4C34-11D4-BDD9-00A0C932BC01}
static const IID IID_IJpegFileDlg =
{ 0x68cec0e2, 0x4c34, 0x11d4, { 0xbd, 0xd9, 0x0, 0xa0, 0xc9, 0x32, 0xbc, 0x1 } };

BEGIN_INTERFACE_MAP(CJpegFileDlg, CDialog)
	INTERFACE_PART(CJpegFileDlg, IID_IJpegFileDlg, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJpegFileDlg message handlers

void CJpegFileDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);

	if (pScrollBar->GetDlgCtrlID() == IDC_SCG_JPG_SCROLLBAR_QUALITY)
	{
		switch( nSBCode)
		{
			case SB_LINEDOWN:
				if (m_FileJpegQuality > 1) m_FileJpegQuality--;
				else m_FileJpegQuality = 1;
				break;
			case SB_LINEUP:
				if( m_FileJpegQuality < 100) m_FileJpegQuality++;
				else m_FileJpegQuality = 100;
				break;
			default:
				break;
		}

   	UpdateData(FALSE);
	}
}

void CJpegFileDlg::OnOK() 
{
	CDialog::OnOK();

	m_App->WriteProfileInt("SapFile", "Jpeg Quality", m_FileJpegQuality);
}

void CJpegFileDlg::OnUpdateJpgEditQuality() 
{
  	UpdateData(TRUE);
}
