// RawFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SapClassBasic.h"
#include "SapClassGui.h"
#include "RawFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRawFileDlg dialog
SapFormatInfo CRawFileDlg::m_FormatInfo[] =
{
	{SapFormatMono1,    "Monochrome 1-bit"},
	{SapFormatMono8,    "Monochrome 8-bit"},
	{SapFormatMono16,   "Monochrome 16-bit"},
	{SapFormatRGB5551,  "RGB 5551"},
	{SapFormatRGB565,   "RGB 565"},
	{SapFormatRGB888,   "RGB 888 (blue first)"},
	{SapFormatRGBR888,  "RGBR 888 (red first)"},
	{SapFormatRGB8888,  "RGB 8888"},
	{SapFormatRGB101010,"RGB 101010"},
	{SapFormatRGB161616,"RGB 161616"},
   {SapFormatRGB16161616,"RGB 16161616"},
	{SapFormatRGBP8,	  "RGB Planar 8-bit"},
	{SapFormatRGBP16,   "RGB Planar 16-bit"},
	{SapFormatHSI,	     "HSI"},
	{SapFormatHSIP8,    "HSI Planar 8-bit"},
	{SapFormatHSV,	     "HSV"},
	{SapFormatUYVY,	  "UYVY"},
	{SapFormatYUY2,	  "YUY2"},
	{SapFormatYUYV,	  "YUYV"},
	{SapFormatLAB,	     "LAB"},
	{SapFormatLABP8,    "LAB Planar 8-bit"},
	{SapFormatLAB101010,"LAB 101010"},
	{SapFormatLABP16,   "LAB Planar 16-bit"},
};

CRawFileDlg::CRawFileDlg(CWnd* pParent, int width, int height, int offset, SapFormat format)
	: CDialog(CRawFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRawFileDlg)
	//}}AFX_DATA_INIT

	m_App = AfxGetApp();
	m_FileRawWidth = m_App->GetProfileInt("SapFile", "Raw Width", width);
	m_FileRawHeight = m_App->GetProfileInt("SapFile", "Raw Height", height);
	m_FileRawOffset = m_App->GetProfileInt("SapFile", "Raw Offset", offset);
   m_FileRawFormat = format;
}


void CRawFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRawFileDlg)
	DDX_Text(pDX, IDC_SCG_RAW_EDIT_HEIGHT, m_FileRawHeight);
	DDX_Text(pDX, IDC_SCG_RAW_EDIT_WIDTH, m_FileRawWidth);
	DDX_Text(pDX, IDC_SCG_RAW_EDIT_OFFSET, m_FileRawOffset);
   DDX_Control(pDX, IDC_SCG_RAW_COMBO_FORMAT, m_cbFormat);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRawFileDlg, CDialog)
	//{{AFX_MSG_MAP(CRawFileDlg)
   ON_CBN_SELCHANGE(IDC_SCG_RAW_COMBO_FORMAT, OnSelchangeComboFormat)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRawFileDlg message handlers

void CRawFileDlg::OnOK() 
{
	CDialog::OnOK();

   m_FileRawFormat = (SapFormat) m_cbFormat.GetItemData(m_cbFormat.GetCurSel());

	m_App->WriteProfileInt("SapFile", "Raw Width", m_FileRawWidth);
	m_App->WriteProfileInt("SapFile", "Raw Height", m_FileRawHeight);
	m_App->WriteProfileInt("SapFile", "Raw Offset", m_FileRawOffset);
   m_App->WriteProfileInt("SapFile", "Raw Format", m_FileRawFormat);
}

BOOL CRawFileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Initialize format combo
	for (int i= 0; i < sizeof(m_FormatInfo) / sizeof(m_FormatInfo[0]); i++)
	{
		int index = m_cbFormat.AddString(m_FormatInfo[i].m_Name);
		m_cbFormat.SetItemData(index, m_FormatInfo[i].m_Value);
			
		if ((m_FormatInfo[i].m_Value == m_FileRawFormat) || ((m_FileRawFormat == 0) && (m_FormatInfo[i].m_Value == SapFormatMono8)))
			m_cbFormat.SetCurSel(index);
	}

   if (m_FileRawFormat != 0)
      m_cbFormat.EnableWindow(FALSE);

   return TRUE;
}

void CRawFileDlg::OnSelchangeComboFormat() 
{
   // Update format
	int index= m_cbFormat.GetCurSel();
	if (index != CB_ERR)
		m_FileRawFormat = (SapFormat) m_cbFormat.GetItemData(index);
}