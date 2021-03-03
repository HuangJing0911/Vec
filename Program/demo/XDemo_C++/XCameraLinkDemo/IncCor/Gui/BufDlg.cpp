// BufDlg.cpp : implementation file
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
// Static variables and methods

SapFormatInfo CBufDlg::m_FormatInfo[] =
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

/////////////////////////////////////////////////////////////////////////////
// CBufDlg dialog

CBufDlg::CBufDlg(CWnd* pParent, SapBuffer *pBuffer, SapDisplay *pDisplay)
	: CDialog(CBufDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBufDlg)
	m_PixelDepth = 0;
	//}}AFX_DATA_INIT

	SapBuffer buf;
	if (!pBuffer) pBuffer = &buf;

	m_Count = pBuffer->GetCount();
   m_Width = pBuffer->GetWidth();
   m_Height = pBuffer->GetHeight();
   m_Format = pBuffer->GetFormat();
   m_Type = pBuffer->GetType();
   m_PixelDepth = pBuffer->GetPixelDepth();
	m_pSrcNode = pBuffer->GetSrcNode();
   m_Location = pBuffer->GetLocation();
   m_pDisplay = pDisplay;

   m_ScatterGatherType = SapBuffer::TypeScatterGather;

   // For acquisition hardware with DMA transfers using 32-bit addresses in 64-bit Windows (e.g., X64-CL iPro)
   if (m_pSrcNode)
   {
      if (!SapBuffer::IsBufferTypeSupported(m_pSrcNode->GetLocation(), SapBuffer::TypeScatterGather))
         m_ScatterGatherType = SapBuffer::TypeScatterGatherPhysical;
   }
}

BOOL CBufDlg::UpdateBuffer()
{
	// Set new parameters
	m_Buffer.SetCount(m_Count);
   m_Buffer.SetWidth(m_Width);
   m_Buffer.SetHeight(m_Height);
   m_Buffer.SetFormat(m_Format);
   m_Buffer.SetType(m_Type);
   m_Buffer.SetPixelDepth(m_PixelDepth);
	m_Buffer.SetSrcNode(m_pSrcNode);
	m_Buffer.SetLocation(m_Location);
	return TRUE;
}

void CBufDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBufDlg)
	DDX_Control(pDX, IDC_SCG_BUFFER_FORMAT, m_cbFormat);
	DDX_Text(pDX, IDC_SCG_BUFFER_WIDTH, m_Width);
	DDX_Text(pDX, IDC_SCG_BUFFER_HEIGHT, m_Height);
	DDX_Text(pDX, IDC_SCG_BUFFER_COUNT, m_Count);
	DDV_MinMaxUInt(pDX, m_Count, 1, UINT_MAX);
	DDX_Text(pDX, IDC_SCG_BUFFER_PIXEL_DEPTH, m_PixelDepth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBufDlg, CDialog)
	//{{AFX_MSG_MAP(CBufDlg)
	ON_CBN_SELCHANGE(IDC_SCG_BUFFER_FORMAT, OnSelchangeComboFormat)
	ON_BN_CLICKED(IDC_SCG_BUFFER_CONTIGUOUS, OnContiguous)
	ON_BN_CLICKED(IDC_SCG_BUFFER_SCATTER_GATHER, OnScatterGather)
	ON_BN_CLICKED(IDC_SCG_BUFFER_OFFSCREEN_VIDEO, OnOffscreenVideo)
	ON_BN_CLICKED(IDC_SCG_BUFFER_OVERLAY, OnOverlay)
	ON_BN_CLICKED(IDC_SCG_BUFFER_VIRTUAL, OnBufferVirtual)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBufDlg message handlers

BOOL CBufDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Initialize format combo
	for (int i= 0; i < sizeof(m_FormatInfo) / sizeof(m_FormatInfo[0]); i++)
	{
		m_cbFormat.AddString(m_FormatInfo[i].m_Name);
		m_cbFormat.SetItemData(i, m_FormatInfo[i].m_Value);
			
		if (m_FormatInfo[i].m_Value == m_Format)
			m_cbFormat.SetCurSel(i);
	}

	// Initialize type flags
   ((CButton *) GetDlgItem(IDC_SCG_BUFFER_CONTIGUOUS))->SetCheck(m_Type == SapBuffer::TypeContiguous);
	((CButton *) GetDlgItem(IDC_SCG_BUFFER_SCATTER_GATHER))->SetCheck(m_Type == m_ScatterGatherType);
	((CButton *) GetDlgItem(IDC_SCG_BUFFER_OFFSCREEN_VIDEO))->SetCheck(m_Type == SapBuffer::TypeOffscreenVideo);
	((CButton *) GetDlgItem(IDC_SCG_BUFFER_OVERLAY))->SetCheck(m_Type == SapBuffer::TypeOverlay);
	((CButton *) GetDlgItem(IDC_SCG_BUFFER_VIRTUAL))->SetCheck(m_Type == SapBuffer::TypeVirtual);

   EnableControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBufDlg::OnSelchangeComboFormat() 
{
   // Update format
	int index= m_cbFormat.GetCurSel();
	if (index != CB_ERR)
		m_Format = (SapFormat) m_cbFormat.GetItemData(index);

   // Reset pixel depth to max
   UpdateData(TRUE);
   m_PixelDepth = SapManager::GetPixelDepthMax(m_Format);
   UpdateData(FALSE);

   EnableControls();
}

void CBufDlg::OnContiguous()
{
	m_Type = SapBuffer::TypeContiguous;
}

void CBufDlg::OnScatterGather()
{
	m_Type = m_ScatterGatherType;
}

void CBufDlg::OnOffscreenVideo()
{
   m_Type = SapBuffer::TypeOffscreenVideo;
}

void CBufDlg::OnOverlay()
{
	m_Type = SapBuffer::TypeOverlay;
}

void CBufDlg::OnBufferVirtual() 
{
	m_Type = SapBuffer::TypeVirtual;	
}

void CBufDlg::OnOK() 
{
	UpdateData(TRUE);

	UpdateBuffer();
	
	CDialog::OnOK();
}

void CBufDlg::EnableControls()
{
	// Lock some parameters
	GetDlgItem(IDC_SCG_BUFFER_WIDTH)->EnableWindow(m_pSrcNode == NULL);
	GetDlgItem(IDC_SCG_BUFFER_HEIGHT)->EnableWindow(m_pSrcNode == NULL);
	GetDlgItem(IDC_SCG_BUFFER_FORMAT)->EnableWindow(m_pSrcNode == NULL);
	GetDlgItem(IDC_SCG_BUFFER_VIRTUAL)->EnableWindow(m_pSrcNode == NULL);

   // Current display determines availability of offscreen and overlay formats
   BOOL isOffscreenAvail = m_pDisplay ? m_pDisplay->IsOffscreenAvailable(m_Format) : TRUE;
   BOOL isOverlayAvail = m_pDisplay ? m_pDisplay->IsOverlayAvailable(m_Format) : TRUE;

	GetDlgItem(IDC_SCG_BUFFER_OFFSCREEN_VIDEO)->EnableWindow(isOffscreenAvail);
	GetDlgItem(IDC_SCG_BUFFER_OVERLAY)->EnableWindow(isOverlayAvail);

   // Is pixel depth adjustable?
   if (SapManager::GetPixelDepthMin(m_Format) != SapManager::GetPixelDepthMax(m_Format))
   	GetDlgItem(IDC_SCG_BUFFER_PIXEL_DEPTH)->EnableWindow(TRUE);
   else
   	GetDlgItem(IDC_SCG_BUFFER_PIXEL_DEPTH)->EnableWindow(FALSE);
}
