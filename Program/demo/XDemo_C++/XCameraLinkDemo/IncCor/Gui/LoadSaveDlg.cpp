// LoadSaveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SapClassBasic.h"
#include "SapClassGui.h"
#include "RawFileDlg.h"
#include "JpegFileDlg.h"
#include "AviFileDlg.h"
#include "TiffFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// General definitions
#if COR_WIN64
#define STANDARD_FILTER		"BMP Files (*.bmp)|*.bmp|TIF Files (*.tif)|*.tif|Teledyne DALSA Files (*.crc)|*.crc|RAW data Files (*.raw)|*.raw|JPEG Files (*.jpg)|*.jpg||"
#else
#define STANDARD_FILTER		"BMP Files (*.bmp)|*.bmp|TIF Files (*.tif)|*.tif|Teledyne DALSA Files (*.crc)|*.crc|RAW data Files (*.raw)|*.raw|JPEG Files (*.jpg)|*.jpg|JPEG 2000 Files (*.jp2)|*.jp2||"
#endif
#define SEQUENCE_FILTER		"AVI Files (*.avi)|*.avi||"

// RAW file definitions
#define RAW_DEFAULT_WIDTH     SapDefWidth
#define RAW_DEFAULT_HEIGHT    SapDefHeight
#define RAW_DEFAULT_OFFSET    0

// JPEG file definitions
#define JPEG_DEFAULT_QUALITY  90

// AVI file definitions
#define AVI_DEFAULT_START_FRAME  0
#define AVI_DEFAULT_COMPRESSION_LEVEL  90
#define AVI_DEFAULT_COMPRESSION_TYPE   0

// TIFF file definitions
#define TIFF_DEFAULT_COMPRESSION_LEVEL  90
#define TIFF_DEFAULT_COMPRESSION_TYPE   0

// TIFF compression types
CString CLoadSaveDlg::m_TiffCompressionTypes[] =
{
   "corfile_val_compression_none",
   "corfile_val_compression_rle",
   "corfile_val_compression_lzw",
   "corfile_val_compression_jpeg",
};

// Type table definition
int CLoadSaveDlg::m_StandardTypes[] = 
{
   SapBuffer::FileFormatBMP,
   SapBuffer::FileFormatTIFF,
   SapBuffer::FileFormatCRC,
   SapBuffer::FileFormatRAW,
   SapBuffer::FileFormatJPEG,
#if !COR_WIN64
   SapBuffer::FileFormatJPEG2000,
#endif
};

#if !COR_WIN64
char CLoadSaveDlg::m_StandardExtensions[6][5] = 
#else
char CLoadSaveDlg::m_StandardExtensions[5][5] = 
#endif
{
   ".bmp",
   ".tif",
   ".crc",
   ".raw",
   ".jpg",
#if !COR_WIN64
   ".jp2",
#endif
};

int CLoadSaveDlg::m_SequenceTypes[] = 
{
   SapBuffer::FileFormatAVI,
};

#define UNSUPPORTED    -1    // not supported
#define NO_CONVERSION   0    // supported without conversion
#define CONV_TO_MONO8   1    // supported but converted to MONO8
#define CONV_TO_RGB8    2    // supported but converted to RGB888
#define CONV_TO_RGB10   3    // supported but converted to RGB101010
#define CONV_TO_RGB16   4    // supported but converted to RGB161616

// The following formats exist but aren't supported in this table:
// SapFormatMono24, SapFormatInt24, SapFormatUint24, SapFormatAYU2, SapFormatIYU1, SapFormatIYU2,
// SapFormatUint1, SapFormatBinary, SapFormatLAB, SapFormatLAB16161616, SapFormatLAB101010,
// SapFormatYUVP8, SapFormatYUVP16, SapFormatHSVP8, SapFormatHSVP16, SapFormatHSIP8, SapFormatHSIP16, 
// SapFormatLABP8, SapFormatLABP16

CONVERSION_TABLE CLoadSaveDlg::m_ConversionTable[]=
{//{FORMAT,               {BMP,           TIFF,          CRC,           RAW,           JPEG,          JPEG2000     }}
	{SapFormatMono8,       {NO_CONVERSION, NO_CONVERSION, NO_CONVERSION, NO_CONVERSION, NO_CONVERSION, NO_CONVERSION}},
	{SapFormatInt8,        {CONV_TO_MONO8, NO_CONVERSION, NO_CONVERSION, NO_CONVERSION, UNSUPPORTED,   NO_CONVERSION}},
	{SapFormatUint8,       {NO_CONVERSION, NO_CONVERSION, NO_CONVERSION, NO_CONVERSION, NO_CONVERSION, NO_CONVERSION}},
	{SapFormatMono16,      {CONV_TO_MONO8, NO_CONVERSION, NO_CONVERSION, NO_CONVERSION, CONV_TO_MONO8, NO_CONVERSION}},
	{SapFormatInt16,       {CONV_TO_MONO8, NO_CONVERSION, NO_CONVERSION, NO_CONVERSION, UNSUPPORTED,   NO_CONVERSION}},
	{SapFormatUint16,      {CONV_TO_MONO8, NO_CONVERSION, NO_CONVERSION, NO_CONVERSION, CONV_TO_MONO8, NO_CONVERSION}},
	{SapFormatMono32,      {UNSUPPORTED,   UNSUPPORTED,   NO_CONVERSION, NO_CONVERSION, UNSUPPORTED,   UNSUPPORTED}},
	{SapFormatInt32,       {UNSUPPORTED,   UNSUPPORTED,   NO_CONVERSION, NO_CONVERSION, UNSUPPORTED,   UNSUPPORTED}},
	{SapFormatUint32,      {UNSUPPORTED,   UNSUPPORTED,   NO_CONVERSION, NO_CONVERSION, UNSUPPORTED,   UNSUPPORTED}},
	{SapFormatMono64,      {UNSUPPORTED,   UNSUPPORTED,   NO_CONVERSION, NO_CONVERSION, UNSUPPORTED,   UNSUPPORTED}},
	{SapFormatInt64,       {UNSUPPORTED,   UNSUPPORTED,   NO_CONVERSION, NO_CONVERSION, UNSUPPORTED,   UNSUPPORTED}},
	{SapFormatUint64,      {UNSUPPORTED,   UNSUPPORTED,   NO_CONVERSION, NO_CONVERSION, UNSUPPORTED,   UNSUPPORTED}},
	{SapFormatRGB5551,     {NO_CONVERSION, CONV_TO_RGB8,  NO_CONVERSION, NO_CONVERSION, UNSUPPORTED,   UNSUPPORTED}},
	{SapFormatRGB565,      {NO_CONVERSION, CONV_TO_RGB8,  NO_CONVERSION, NO_CONVERSION, UNSUPPORTED,   NO_CONVERSION}},
	{SapFormatRGB888,      {NO_CONVERSION, NO_CONVERSION, NO_CONVERSION, NO_CONVERSION, NO_CONVERSION, NO_CONVERSION}},
	{SapFormatRGBR888,     {CONV_TO_RGB8,  CONV_TO_RGB8,  NO_CONVERSION, NO_CONVERSION, CONV_TO_RGB8,  CONV_TO_RGB8}},
	{SapFormatRGB8888,     {NO_CONVERSION, CONV_TO_RGB8,  NO_CONVERSION, NO_CONVERSION, NO_CONVERSION, NO_CONVERSION}},
	{SapFormatRGB101010,   {NO_CONVERSION, CONV_TO_RGB16, NO_CONVERSION, NO_CONVERSION, UNSUPPORTED,   NO_CONVERSION}},
	{SapFormatRGB161616,   {CONV_TO_RGB10, NO_CONVERSION, NO_CONVERSION, NO_CONVERSION, UNSUPPORTED,   NO_CONVERSION}},
	{SapFormatRGB16161616, {CONV_TO_RGB10, CONV_TO_RGB16, NO_CONVERSION, NO_CONVERSION, UNSUPPORTED,   UNSUPPORTED}},
	{SapFormatHSV,         {UNSUPPORTED,   UNSUPPORTED,   NO_CONVERSION, NO_CONVERSION, UNSUPPORTED,   UNSUPPORTED}},
	{SapFormatUYVY,        {CONV_TO_RGB8,  CONV_TO_RGB8,  NO_CONVERSION, NO_CONVERSION, CONV_TO_RGB8,  CONV_TO_RGB8}},
	{SapFormatYUY2,        {CONV_TO_RGB8,  CONV_TO_RGB8,  NO_CONVERSION, NO_CONVERSION, CONV_TO_RGB8,  CONV_TO_RGB8}},
	{SapFormatYVYU,        {CONV_TO_RGB8,  CONV_TO_RGB8,  NO_CONVERSION, NO_CONVERSION, CONV_TO_RGB8,  CONV_TO_RGB8}},
	{SapFormatYUYV,        {CONV_TO_RGB8,  CONV_TO_RGB8,  NO_CONVERSION, NO_CONVERSION, CONV_TO_RGB8,  CONV_TO_RGB8}},
	{SapFormatY411,        {UNSUPPORTED,   UNSUPPORTED,   NO_CONVERSION, NO_CONVERSION, UNSUPPORTED,   UNSUPPORTED}},
	{SapFormatY211,        {UNSUPPORTED,   UNSUPPORTED,   NO_CONVERSION, NO_CONVERSION, UNSUPPORTED,   UNSUPPORTED}},
	{SapFormatYUV,         {UNSUPPORTED,   UNSUPPORTED,   NO_CONVERSION, NO_CONVERSION, UNSUPPORTED,   UNSUPPORTED}},
	{SapFormatFloat,       {UNSUPPORTED,   UNSUPPORTED,   NO_CONVERSION, NO_CONVERSION, UNSUPPORTED,   UNSUPPORTED}},
	{SapFormatComplex,     {UNSUPPORTED,   UNSUPPORTED,   NO_CONVERSION, NO_CONVERSION, UNSUPPORTED,   UNSUPPORTED}},
	{SapFormatPoint,       {UNSUPPORTED,   UNSUPPORTED,   NO_CONVERSION, NO_CONVERSION, UNSUPPORTED,   UNSUPPORTED}},
	{SapFormatFPoint,      {UNSUPPORTED,   UNSUPPORTED,   NO_CONVERSION, NO_CONVERSION, UNSUPPORTED,   UNSUPPORTED}},
	{SapFormatMono1,       {NO_CONVERSION, NO_CONVERSION, NO_CONVERSION, NO_CONVERSION, UNSUPPORTED,   UNSUPPORTED}},
	{SapFormatHSI,         {UNSUPPORTED,   UNSUPPORTED,   NO_CONVERSION, NO_CONVERSION, UNSUPPORTED,   UNSUPPORTED}},
	{SapFormatRGBP8,       {CONV_TO_RGB8,  CONV_TO_RGB8,  NO_CONVERSION, NO_CONVERSION, UNSUPPORTED,   NO_CONVERSION}},
	{SapFormatRGBP16,      {CONV_TO_RGB10, CONV_TO_RGB16, NO_CONVERSION, NO_CONVERSION, UNSUPPORTED,   UNSUPPORTED}}

};
/////////////////////////////////////////////////////////////////////////////
// CLoadSaveDlg
IMPLEMENT_DYNAMIC(CLoadSaveDlg, CFileDialog)

CLoadSaveDlg::CLoadSaveDlg(CWnd* pParent, SapBuffer *pBuffer, BOOL isFileOpen, BOOL isSequence)
: CFileDialog(isFileOpen, "", NULL, OFN_HIDEREADONLY | (isFileOpen ? OFN_FILEMUSTEXIST : OFN_OVERWRITEPROMPT), isSequence ? SEQUENCE_FILTER : STANDARD_FILTER, pParent)
{
	m_pBuffer = pBuffer;
	m_App = AfxGetApp();
	m_bSequence = isSequence;
   m_StartFrame = (isSequence || pBuffer == NULL) ? 0 : pBuffer->GetIndex();

	// Load parameters from registry
	LoadSettings();
}

BOOL CLoadSaveDlg::UpdateBuffer()
{
	// Set load/save options
   if (!SetOptions())
      return FALSE;

	if (m_pBuffer != NULL)
	{
		// Load/save file
		CWaitCursor wait;
		if (m_bOpenFileDialog)
		{
			return m_pBuffer->Load(m_PathName, m_StartFrame, 0, 0, m_Options);
		}
		else
		{
			int conversionType = 0;
			SapFormat format = m_pBuffer->GetFormat();

			for (int i = 0; i< (sizeof(m_ConversionTable) / sizeof(CONVERSION_TABLE)); i++)
			{
				if (m_ConversionTable[i].BufferFormat == format)
				{
					switch (m_Type)
					{
					case SapBuffer::FileFormatBMP:
						conversionType = m_ConversionTable[i].FileFormat[0];
						break;
					case SapBuffer::FileFormatTIFF:
						conversionType = m_ConversionTable[i].FileFormat[1];
						break;
					case SapBuffer::FileFormatCRC:
						conversionType = m_ConversionTable[i].FileFormat[2];
						break;
					case SapBuffer::FileFormatRAW:
						conversionType = m_ConversionTable[i].FileFormat[3];
						break;
					case SapBuffer::FileFormatJPEG:
						conversionType = m_ConversionTable[i].FileFormat[4];
						break;
#if !COR_WIN64
					case SapBuffer::FileFormatJPEG2000:
						conversionType = m_ConversionTable[i].FileFormat[5];
						break;
#endif
					default:
						break;
					}
					break;
				}
			}

			if (conversionType > 0)
			{
            CString message;
            switch(conversionType)
            {
            case CONV_TO_MONO8:
               message = "Warning: data conversion to MONO8 has taken place to save the image in this file format.";
               break;
            case CONV_TO_RGB8:
               message = "Warning: data conversion to RGB888 has taken place to save the image in this file format.";
               break;
            case CONV_TO_RGB10:
               message = "Warning: data conversion to RGB101010 has taken place to save the image in this file format.";
               break;
            case CONV_TO_RGB16:
               message = "Warning: data conversion to RGB161616 has taken place to save the image in this file format.";
               break;
            }
				MessageBox((LPCSTR)message, 0, MB_ICONEXCLAMATION);
			}

			return m_pBuffer->Save(m_PathName, m_Options, m_StartFrame);
		}
	}

	return TRUE;
}

// Only use INT_PTR starting with Visual Studio .NET 2003, since its presence depends on the
// Platform SDK version with Visual Studio 6, and it is only needed for 64-bit compatibility anyway.
#if defined(_MSC_VER) && _MSC_VER >= 1300
INT_PTR CLoadSaveDlg::DoModal()
#else
int CLoadSaveDlg::DoModal()
#endif
{
	// Set name and type
   m_ofn.lpstrFile = m_PathName;
	SetFilterIndex();

	// Call base class method
   int ret = (int) CFileDialog::DoModal();
	if (ret != IDOK) return ret;

	// Update type from filter index
   if (m_bSequence)
      m_Type = m_SequenceTypes[m_ofn.nFilterIndex - 1];
   else
      m_Type = m_StandardTypes[m_ofn.nFilterIndex - 1];

	// Update file object
	if (!UpdateBuffer()) return IDABORT;
 
   // Save parameters to registry
   SaveSettings();

	return IDOK;
}

void CLoadSaveDlg::LoadSettings()
{
	// Read file name
	CString str = m_App->GetProfileString("SapFile", "Name", "");
   CorStrncpy(m_PathName, str, sizeof(m_PathName));

	// Read file type
	m_Type = m_App->GetProfileInt("SapFile", "Type", 0);
}

void CLoadSaveDlg::SaveSettings()
{
	// Write file name
	m_App->WriteProfileString("SapFile", "Name", m_PathName);

	// Write file type
	m_App->WriteProfileInt("SapFile", "Type", m_Type);
}

BOOL CLoadSaveDlg::SetOptions()
{
   BOOL success = TRUE;

	switch (m_Type)
	{
		case SapBuffer::FileFormatBMP: 
			m_Options = "-format bmp"; 
			break;

		case SapBuffer::FileFormatTIFF: 
			m_Options = "-format tif"; 
         if (!m_bOpenFileDialog)
            success = AddTiffOptions();
			break;

		case SapBuffer::FileFormatCRC: 
			m_Options = "-format crc"; 
			break;

		case SapBuffer::FileFormatRAW: 
			m_Options = "-format raw"; 
         if (m_bOpenFileDialog)
            success = AddRawOptions();
			break;

		case SapBuffer::FileFormatJPEG: 
			m_Options = "-format jpg"; 
         if (!m_bOpenFileDialog)
            success = AddJpegOptions();
			break;

		case SapBuffer::FileFormatJPEG2000: 
			m_Options = "-format jp2"; 
         if (!m_bOpenFileDialog)
            success = AddJpegOptions();
			break;

		case SapBuffer::FileFormatAVI: 
			m_Options = "-format avi"; 
         if (m_bOpenFileDialog)
            success = AddAviOptions();
			break;
	}

   return success;
}

BOOL CLoadSaveDlg::AddTiffOptions()
{
	CTiffFileDlg dlg(m_pParentWnd, TIFF_DEFAULT_COMPRESSION_LEVEL, TIFF_DEFAULT_COMPRESSION_TYPE);

   if (dlg.DoModal() != IDOK)
      return FALSE;

	CString strType;
   strType.Format(" -compression %s", (LPCTSTR)m_TiffCompressionTypes[dlg.GetCompressionType()]);
	m_Options += strType;

	CString strLevel;
	strLevel.Format(" -quality %d", dlg.GetCompressionLevel());
	m_Options += strLevel;

   return TRUE;
}

BOOL CLoadSaveDlg::AddRawOptions()
{
   int width = m_pBuffer ? m_pBuffer->GetWidth() : RAW_DEFAULT_WIDTH;
   int height = m_pBuffer ? m_pBuffer->GetHeight() : RAW_DEFAULT_HEIGHT;
   int format = m_pBuffer ? m_pBuffer->GetFormat() : 0;

	CRawFileDlg dlg(m_pParentWnd, width, height, RAW_DEFAULT_OFFSET, format);

   if (dlg.DoModal() != IDOK)
      return FALSE;

   m_RawFileWidth = dlg.GetRawFileWidth();
   m_RawFileHeight = dlg.GetRawFileHeight();
   m_RawFileOffset = dlg.GetRawFileOffset();
   m_RawFileFormat = dlg.GetRawFileFormat();

	CString str;
	str.Format(" -width %d -height %d -offset %d", m_RawFileWidth, m_RawFileHeight, m_RawFileOffset);
	m_Options += str;

   return TRUE;
}

BOOL CLoadSaveDlg::AddJpegOptions()
{
	CJpegFileDlg dlg(m_pParentWnd, JPEG_DEFAULT_QUALITY);

   if (dlg.DoModal() != IDOK)
      return FALSE;

	CString str;
	str.Format(" -q %d", dlg.GetJpegFileQuality());
	m_Options += str;

   return TRUE;
}

BOOL CLoadSaveDlg::AddAviOptions()
{
	CAviFileDlg dlg(m_pParentWnd, TRUE, AVI_DEFAULT_START_FRAME, AVI_DEFAULT_COMPRESSION_LEVEL, AVI_DEFAULT_COMPRESSION_TYPE);

   if (dlg.DoModal() != IDOK)
      return FALSE;

	m_StartFrame = dlg.GetFirstFrame();

   return TRUE;
}

void CLoadSaveDlg::SetFilterIndex()
{
   m_ofn.nFilterIndex = 1;

   if (m_bSequence)
   {
      for (int i=0; i < sizeof(m_SequenceTypes) / sizeof(m_SequenceTypes[0]); i++)
         if (m_Type == m_SequenceTypes[i]) 
         {
            m_ofn.nFilterIndex = i + 1;
            return;
         }
   }
   else
   {
      for (int i=0; i < sizeof(m_StandardTypes) / sizeof(m_StandardTypes[0]); i++)
         if (m_Type == m_StandardTypes[i])
         {
            m_ofn.nFilterIndex = i + 1;
            return;
         }
   }
}

void CLoadSaveDlg::OnTypeChange()
{
   CWnd* pDialogWnd = GetForegroundWindow();
   CString filename = GetFileTitle();
   filename += m_StandardExtensions[m_ofn.nFilterIndex - 1];
   ASSERT_VALID(pDialogWnd);

   pDialogWnd->SetDlgItemText(cmb13,(LPCTSTR)filename); 
  
}

BEGIN_MESSAGE_MAP(CLoadSaveDlg, CFileDialog)
	//{{AFX_MSG_MAP(CLoadSaveDlg)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

