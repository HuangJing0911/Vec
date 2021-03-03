// FlatFieldDlg.h : header file
//

#if !defined(AFX_FLATFIELDDLG_H__4B4FB021_6AFA_4451_8869_517250DC6EB3__INCLUDED_)
#define AFX_FLATFIELDDLG_H__4B4FB021_6AFA_4451_8869_517250DC6EB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFlatFieldDlg dialog

class SAPCLASSGUI_CLASS CFlatFieldDlg : public CDialog, public SapManager
{
public:
   // Various constants
   enum MiscValues
   {
      DefNumFramesAverage = 10,
	};

   // Log message types
   enum LogTypes
   {
      Error,
      Warning,
      Info
   };

// Construction
public:
   CFlatFieldDlg(CWnd *pParent, SapFlatField *pFlatField, SapTransfer *pXfer, SapBuffer *pBuffer);
   virtual ~CFlatFieldDlg();

// Acquire one image
   BOOL Snap();

// Dialog Data
   //{{AFX_DATA(CFlatFieldDlg)
	enum { IDD = IDD_SCG_FLAT_FIELD_CALIBRATION };
   CComboBox m_CorrectionTypeCtrl;
   CComboBox m_VideoTypeCtrl;
   CListBox m_LogMessageCtrl;
   int m_NumLinesAverage;
	int m_NumFramesAverage;
	int m_VerticalOffset;
   int m_DeviationMaxBlack;
   int m_DeviationMaxWhite;
   CComboBox m_CalibrationIndexCtrl;
   CButton m_SaveAndUpload;
   CStatic m_FlatFieldLabel;
   CString m_SaveLabel;
   CComboBox m_FlatFieldSelector;
	int m_ClippedCoefsDefects;
   float m_BlackPercentage;
   BOOL m_isGenie;
	//}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CFlatFieldDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CFlatFieldDlg)
   virtual BOOL OnInitDialog();
   afx_msg void OnDarkImage();
   afx_msg void OnBrightImage();
   afx_msg void OnDestroy();
   afx_msg void OnSaveAndUpload();
   virtual void OnOK();
   virtual void OnCancel();
   afx_msg void OnSelchangeCorrectionType();
   afx_msg void OnSelchangeVideoType();
   afx_msg void OnSelchangeCalibrationIndex();
	//}}AFX_MSG
   DECLARE_MESSAGE_MAP()

private:
   void DarkImage();
   void BrightImage();
   void LogMessage(int messageType, CString string);
	
	CString GetAverageStr( SapFlatFieldStats& stats) const;
	CString GetStdDeviationStr( SapFlatFieldStats& stats) const;
	CString GetLowStr( SapFlatFieldStats& stats) const;
	CString GetHighStr( SapFlatFieldStats& stats) const;
	CString GetPixelRatioStr( SapFlatFieldStats& stats) const;
	CString GetNumPixelsStr( SapFlatFieldStats& stats) const;
	CString GetPeakPositionStr( SapFlatFieldStats& stats) const;

private:
   SapFlatField *m_pFlatField;
   SapTransfer *m_pXfer;
   SapBuffer *m_pBuffer;
   SapBuffer *m_pLocalBuffer;

	SapFlatField::CorrectionType m_CorrectionType;
	SapAcquisition::VideoType m_VideoType;
	int m_RecommendedDark;
	int m_RecommendedBright;
   int m_GainDivisor;
   int m_CalibrationIndex;
	BOOL m_FlatFieldDataValid;
   int m_UserFlatFieldCount;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLATFIELDDLG_H__4B4FB021_6AFA_4451_8869_517250DC6EB3__INCLUDED_)
