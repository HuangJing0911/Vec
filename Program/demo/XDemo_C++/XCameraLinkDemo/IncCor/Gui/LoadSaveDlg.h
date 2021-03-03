#if !defined(AFX_LOADSAVEDLG_H__28D5F607_7438_11D4_BC94_00A0C9081566__INCLUDED_)
#define AFX_LOADSAVEDLG_H__28D5F607_7438_11D4_BC94_00A0C9081566__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoadSaveDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLoadSaveDlg dialog

typedef struct 
{
	SapFormat BufferFormat; // Buffer format
	int FileFormat[6]; // support for saving in bmp, tif, crc, raw, jpg and jpg200 in that order
} CONVERSION_TABLE, *PCONVERSION_TABLE;

class SAPCLASSGUI_CLASS CLoadSaveDlg : public CFileDialog
{
	DECLARE_DYNAMIC(CLoadSaveDlg)

public:
	CLoadSaveDlg(CWnd* pParent, SapBuffer *pBuffer, BOOL isFileOpen = TRUE, BOOL isSequence = FALSE);

	// Access to implementation
	CString GetPathName()	{ return m_PathName; }
	int GetType()				{ return m_Type; }
	CString GetOptions()		{ return m_Options; }
	int GetStartFrame()		{ return m_StartFrame; }
   int GetRawFileWidth()   { return m_RawFileWidth; }
   int GetRawFileHeight()  { return m_RawFileHeight; }
   int GetRawFileOffset()  { return m_RawFileOffset; }
   SapFormat GetRawFileFormat()  {return m_RawFileFormat; }


	// Operations
   // Only use INT_PTR starting with Visual Studio .NET 2003, since its presence depends on the
   // Platform SDK version with Visual Studio 6, and it is only needed for 64-bit compatibility anyway.
#if defined(_MSC_VER) && _MSC_VER >= 1300
	virtual INT_PTR DoModal();
#else
	virtual int DoModal();
#endif

protected:
	BOOL UpdateBuffer();
	void LoadSettings();
	void SaveSettings();
	BOOL SetOptions();
	BOOL AddTiffOptions();
	BOOL AddRawOptions();
	BOOL AddJpegOptions();
	BOOL AddAviOptions();
   void SetFilterIndex();
   virtual void OnTypeChange();

protected:
	//{{AFX_MSG(CLoadSaveDlg)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	SapBuffer *m_pBuffer;		// Pointer to a SapBuffer object
	CWinApp *m_App;				// Pointer to application object
	BOOL m_bSequence;				// TRUE if it is a sequential file

	char m_PathName[MAX_PATH];
	int m_Type;
	CString m_Options;
   int m_StartFrame;

   int m_RawFileWidth;
   int m_RawFileHeight;
   int m_RawFileOffset;
   SapFormat m_RawFileFormat;

   static CString m_TiffCompressionTypes[];
   static int m_StandardTypes[];
   static int m_SequenceTypes[];
#if !COR_WIN64
   static char m_StandardExtensions[6][5];
#else
   static char m_StandardExtensions[5][5];
#endif
   static CONVERSION_TABLE m_ConversionTable[];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOADSAVEDLG_H__28D5F607_7438_11D4_BC94_00A0C9081566__INCLUDED_)
