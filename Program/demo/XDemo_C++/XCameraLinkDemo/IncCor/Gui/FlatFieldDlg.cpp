// FlatFieldDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SapClassBasic.h"
#include "SapClassGui.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FlatFieldDlg dialog

CFlatFieldDlg::CFlatFieldDlg(CWnd *pParent, SapFlatField *pFlatField, SapTransfer *pXfer, SapBuffer *pBuffer)
   : CDialog(CFlatFieldDlg::IDD, pParent)
{
   m_pBuffer    = pBuffer;
   m_pFlatField = pFlatField;
   m_pXfer      = pXfer;
   m_UserFlatFieldCount = 0;
   m_GainDivisor  = 0;
	m_RecommendedDark = 0;
	m_RecommendedBright = 0;
   m_isGenie = FALSE;
	m_FlatFieldDataValid = TRUE;

	//{{AFX_DATA_INIT(CFlatFieldDlg)
   m_NumLinesAverage = SapFlatField::DefNumLinesAverage;
	m_NumFramesAverage = DefNumFramesAverage;
	m_VerticalOffset = 0;
	m_DeviationMaxBlack   = 0;
   m_DeviationMaxWhite   = 0;
	m_ClippedCoefsDefects = BST_CHECKED;
	//}}AFX_DATA_INIT
}

CFlatFieldDlg::~CFlatFieldDlg()
{
}

void CFlatFieldDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CFlatFieldDlg)
   DDX_Control(pDX, IDC_SCG_SAVE_AND_UPLOAD, m_SaveAndUpload);
   DDX_Control(pDX, IDC_SCG_FLATFIELD_SELECT, m_FlatFieldSelector);
   DDX_Control(pDX,IDC_STATIC_FLATFIELD_LABEL,m_FlatFieldLabel);
   DDX_Control(pDX, IDC_SCG_FF_CORRECTION_TYPE, m_CorrectionTypeCtrl);
   DDX_Control(pDX, IDC_SCG_FF_VIDEO_TYPE, m_VideoTypeCtrl);
   DDX_Control(pDX, IDC_SCG_FF_LOG_MESSAGE, m_LogMessageCtrl);
   DDX_Text(pDX, IDC_SCG_FF_LINES_AVG, m_NumLinesAverage);
	DDV_MinMaxDWord(pDX, m_NumLinesAverage, 1, 65536);
	DDX_Text(pDX, IDC_SCG_FF_FRAMES_AVG, m_NumFramesAverage);
	DDV_MinMaxDWord(pDX, m_NumFramesAverage, 1, 4096);
	DDX_Text(pDX, IDC_SCG_FF_VERTICAL_OFFSET, m_VerticalOffset);
   DDX_Text(pDX, IDC_SAVE_LABEL, m_SaveLabel);
	DDX_Text(pDX, IDC_SCG_FF_BLACK_DEVIATION_MAX, m_DeviationMaxBlack);
  	DDX_Text(pDX, IDC_SCG_FF_WHITE_DEVIATION_MAX, m_DeviationMaxWhite);
   DDX_Text(pDX, IDC_SCG_FF_BLACK_PERCENTAGE, m_BlackPercentage);
	DDV_MinMaxDWord(pDX, m_VerticalOffset, 0, UINT32_MAX);
	DDV_MinMaxDWord(pDX, m_DeviationMaxBlack, 0, UINT32_MAX);
   DDV_MinMaxDWord(pDX, m_DeviationMaxWhite, 0, UINT32_MAX);
   DDX_Control(pDX, IDC_SCG_FF_CALIBRATION_INDEX, m_CalibrationIndexCtrl);
	DDX_Check(pDX, IDC_SCG_FF_CLIPPED_AS_DEFECTS, m_ClippedCoefsDefects);
   
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFlatFieldDlg, CDialog)
   //{{AFX_MSG_MAP(CFlatFieldDlg)
   ON_BN_CLICKED(IDC_SCG_FF_DARK_IMAGE, OnDarkImage)
   ON_BN_CLICKED(IDC_SCG_FF_BRIGHT_IMAGE, OnBrightImage)
   ON_BN_CLICKED(IDC_SCG_SAVE_AND_UPLOAD, OnSaveAndUpload)
   ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_SCG_FF_CORRECTION_TYPE, OnSelchangeCorrectionType)
	ON_CBN_SELCHANGE(IDC_SCG_FF_VIDEO_TYPE, OnSelchangeVideoType)
	ON_CBN_SELCHANGE(IDC_SCG_FF_CALIBRATION_INDEX, OnSelchangeCalibrationIndex)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlatFieldDlg message handlers

BOOL CFlatFieldDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if( m_pBuffer == NULL || !*m_pBuffer)
	{
		AfxMessageBox("Invalid buffer object");
		CDialog::OnCancel();
		return FALSE;
	}

	if( m_pFlatField == NULL)
	{
		AfxMessageBox("Invalid flat-field object");
		CDialog::OnCancel();
		return FALSE;
	}

	CString str;
   BOOL    isOffLine = (m_pXfer == NULL);

   str.Format("Step 1: %s a dark image", isOffLine ? "Load": "Acquire");
   GetDlgItem(IDC_SCG_FF_DARK_IMAGE_MSG_1)->SetWindowText(str);

   str.Format("%s &Dark Image", isOffLine ? "Load": "Acquire");
   GetDlgItem(IDC_SCG_FF_DARK_IMAGE)->SetWindowText(str);

   str.Format("Step 2: %s a non-saturated bright image", isOffLine ? "Load": "Acquire");
   GetDlgItem(IDC_SCG_FF_BRIGHT_IMAGE_MSG_1)->SetWindowText(str);

   str.Format("%s &Bright Image", isOffLine ? "Load": "Acquire");
   GetDlgItem(IDC_SCG_FF_BRIGHT_IMAGE)->SetWindowText(str);

   // Calculate recommended values for dark and bright images
	int maxPixelValue = (int) pow(2.0, m_pBuffer->GetPixelDepth());
	m_RecommendedDark = (int)(maxPixelValue * SapDefFlatFieldAvgFactorBlack);
	m_RecommendedBright = (int)(maxPixelValue * SapDefFlatFieldAvgFactorWhite);

   // Adjust the recommended dark value according to the hardware limit for offset coefficients
   int offsetMin = 0;
   int offsetMax = 255;
   m_pFlatField->GetOffsetMinMax(&offsetMin, &offsetMax);

   if (m_RecommendedDark > offsetMax)
      m_RecommendedDark = offsetMax;

   str.Format("(recommended average pixel\nvalue below %d)\n", m_RecommendedDark);
   GetDlgItem(IDC_SCG_FF_DARK_IMAGE_MSG_2)->SetWindowText(str);

   str.Format("(recommended average pixel\nvalue around %d)\n", m_RecommendedBright);
   GetDlgItem(IDC_SCG_FF_BRIGHT_IMAGE_MSG_2)->SetWindowText(str);

   // Query correction type, video type, number of lines to average (line scan),
   // vertical offset, gain divisor, maximum deviation, and calibration index
	// for the dark image from flat field object
   m_CorrectionType   = m_pFlatField->GetCorrectionType();
	m_VideoType        = m_pFlatField->GetVideoType();
   m_NumLinesAverage  = m_pFlatField->GetNumLinesAverage();
   m_NumFramesAverage = m_pFlatField->GetNumFramesAverage();
	m_VerticalOffset   = m_pFlatField->GetVerticalOffset();
   m_GainDivisor      = m_pFlatField->GetGainDivisor();
	m_DeviationMaxBlack = m_pFlatField->GetDeviationMaxBlack();
   m_DeviationMaxWhite = m_pFlatField->GetDeviationMaxWhite();
   m_CalibrationIndex = m_pFlatField->GetIndex();
	m_ClippedCoefsDefects = m_pFlatField->IsClippedGainOffsetDefects() ? BST_CHECKED : BST_UNCHECKED;
   m_BlackPercentage  = m_pFlatField->GetBlackPixelPercentage();

   // Adjust maximum black deviation from the adjusted recommended dark value
   if (m_DeviationMaxBlack > m_RecommendedDark)
   {
      m_DeviationMaxBlack = m_RecommendedDark;
      m_pFlatField->SetDeviationMaxBlack(m_DeviationMaxBlack);
   }

   // Set correction type
   m_CorrectionTypeCtrl.AddString("Flat Field");
   m_CorrectionTypeCtrl.AddString("Flat Line");
   m_CorrectionTypeCtrl.SetCurSel(m_CorrectionType == SapFlatField::CorrectionTypeField ? 0 : 1);

	// Set video type
	m_VideoTypeCtrl.AddString( "Monochrome");
	m_VideoTypeCtrl.AddString( "Bayer");
	m_VideoTypeCtrl.SetCurSel( m_VideoType != SapAcquisition::VideoBayer ? 0: 1);

   // Set calibration index
   CString indexStr;

   for (int index = 0; index < m_pFlatField->GetNumFlatField(); index++)
   {
      indexStr.Format("%d", index);
      m_CalibrationIndexCtrl.AddString( indexStr);
   }

   m_CalibrationIndexCtrl.SetCurSel( m_CalibrationIndex);

   // Enable/disable controls according to current properties
   // Changing the correction type (flat-field vs flat-line) is only relevant when operating
   // offline, that is, when this information is not available from the acquisition hardware.
   BOOL isOnline = (m_pXfer && *m_pXfer);
   m_CorrectionTypeCtrl.EnableWindow(!isOnline && m_VideoType != SapAcquisition::VideoBayer);
	m_VideoTypeCtrl.EnableWindow( m_pXfer == NULL);

   GetDlgItem(IDC_SCG_FF_FRAMES_AVG)->EnableWindow( m_pXfer != NULL);
   GetDlgItem(IDC_SCG_FF_LINES_AVG)->EnableWindow(m_CorrectionType == SapFlatField::CorrectionTypeLine);
   GetDlgItem(IDC_SCG_FF_VERTICAL_OFFSET)->EnableWindow(m_CorrectionType == SapFlatField::CorrectionTypeLine);
   GetDlgItem(IDC_SCG_FF_BLACK_DEVIATION_MAX)->EnableWindow(TRUE);
   GetDlgItem(IDC_SCG_FF_WHITE_DEVIATION_MAX)->EnableWindow(TRUE);
   GetDlgItem(IDC_SCG_FF_BLACK_PERCENTAGE)->EnableWindow(TRUE);

   m_CalibrationIndexCtrl.EnableWindow( m_pFlatField->GetNumFlatField() > 1);

   GetDlgItem(IDC_SCG_SAVE_AND_UPLOAD)->EnableWindow(FALSE);
   GetDlgItem(IDC_SCG_FLATFIELD_SELECT)->EnableWindow(FALSE);
   GetDlgItem(IDC_SCG_FF_DARK_IMAGE)->EnableWindow(TRUE);
   GetDlgItem(IDC_SCG_FF_BRIGHT_IMAGE)->EnableWindow(FALSE);
   GetDlgItem(IDOK)->EnableWindow(FALSE);
   GetDlgItem(IDCANCEL)->EnableWindow(TRUE);

   if (m_pFlatField->GetAcqDevice())
   {
      SapFeature feature(m_pFlatField->GetAcqDevice()->GetLocation());
      feature.Create();
      int fileCount = 0;

      BOOL status = false;
      char deviceModel[64];

      //for all Genies except Genie TS, do not try to do a file access
      m_pFlatField->GetAcqDevice()->IsFeatureAvailable("DeviceModelName",&status);
      if(status)
      {
         m_pFlatField->GetAcqDevice()->GetFeatureValue("DeviceModelName",deviceModel,sizeof(deviceModel));
         if (strstr(deviceModel,"Genie") != 0 && strstr(deviceModel,"TS") == 0)
         {
            //device is a Genie but not Genie TS
            m_FlatFieldLabel.ShowWindow(FALSE);
            m_FlatFieldSelector.ShowWindow(FALSE);
            m_SaveAndUpload.SetWindowTextA("Save");
            m_isGenie = TRUE;
            UpdateData(FALSE);
            return FALSE;
         }
      }
      if (m_pFlatField->GetAcqDevice()->GetFileCount(&fileCount) == FALSE)
      {
         m_FlatFieldLabel.ShowWindow(FALSE);
         m_FlatFieldSelector.ShowWindow(FALSE);
         m_SaveAndUpload.SetWindowTextA("Save");
         UpdateData(FALSE);
         return FALSE;
      }

      for (int i = 0; i < fileCount; i++)
      {
         char currentFileName[512];
         CString fileName;
         memset(currentFileName, 0, sizeof(currentFileName));
         m_pFlatField->GetAcqDevice()->GetFileNameByIndex(i, &currentFileName[0], sizeof(currentFileName));
         if (strstr(currentFileName,"FlatField")!= 0)
         {
            CString featureName;
            featureName.Format("%s%s", "##FileSelector.",currentFileName);
            fileName.Format("%s",currentFileName);
            if (fileName.Find("0",0) == -1)
            {
               if(m_pFlatField->GetAcqDevice()->GetFeatureInfo(featureName,&feature))
               {
                  char displayName[512];
                  feature.GetDisplayName(displayName,sizeof(displayName));
                  m_FlatFieldSelector.AddString(displayName);               
               }
               else
               {
                  m_FlatFieldSelector.AddString(currentFileName); 
               }
               m_FlatFieldSelector.SetItemData(m_UserFlatFieldCount++, i);
            }
         }
      }
   
      m_FlatFieldSelector.SetCurSel(0);
      m_SaveLabel = "Step 3:  Save Calibration offset and gain files";
      feature.Destroy();
   }
   else
   {
      m_FlatFieldLabel.ShowWindow(FALSE);
      m_FlatFieldSelector.ShowWindow(FALSE);
      m_SaveAndUpload.SetWindowTextA("Save");
   }

   

   UpdateData(FALSE);

   return TRUE;  // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CFlatFieldDlg::OnDestroy() 
{
   CDialog::OnDestroy();
}

//
// Step 1: Snap a dark image to determine the offset coefficients
//
void CFlatFieldDlg::OnDarkImage() 
{
	UpdateData();

	int nbImagesUsed = (m_pFlatField->GetCorrectionType() == SapFlatField::CorrectionTypeField) ? m_NumFramesAverage : 1;

   // Set correction type
   m_pFlatField->SetCorrectionType( m_CorrectionType);

	// Set video type
	m_pFlatField->SetVideoType( m_VideoType);

	// Set maximum deviation from average pixel value for dark image
	m_pFlatField->SetDeviationMaxBlack( m_DeviationMaxBlack);

	// Set number of lines to average and vertical offset
   m_pFlatField->SetNumLinesAverage( m_NumLinesAverage);
	m_pFlatField->SetVerticalOffset( m_VerticalOffset);

	m_pFlatField->SetNumFramesAverage( nbImagesUsed);

	// Set wether to declare pixels with clipped coefficient as defective
	m_pFlatField->EnableClippedGainOffsetDefects(m_ClippedCoefsDefects == BST_CHECKED);

   // Set allowed percentage of black pixels on corrected image
   m_pFlatField->SetBlackPixelPercentage(m_BlackPercentage);

   // Set calibration index
   m_pFlatField->SetIndex(m_CalibrationIndex);
   
   m_LogMessageCtrl.ResetContent();

   if (m_pXfer && *m_pXfer)
   {
		m_pLocalBuffer = new SapBuffer(nbImagesUsed, m_pBuffer);
		m_pLocalBuffer->Create();

      // Acquire an image
      if (!Snap())
      {
         LogMessage(Error, "Unable to acquire an image");
         goto Cleanup;
      }
   }
   else
   {
      // Load an image

		m_pLocalBuffer = new SapBuffer(1, m_pBuffer);
		m_pLocalBuffer->Create();

      CLoadSaveDlg dlg(GetParent(), NULL, TRUE);
      if (dlg.DoModal() != IDOK)
         goto Cleanup;

      CString path = dlg.GetPathName();

      // Create a temporary buffer in order to know the selected file's native format and pixel depth

      SapBuffer loadBuffer(path);
      loadBuffer.Create();

      if(loadBuffer.GetFormat() != m_pBuffer->GetFormat() || loadBuffer.GetPixelDepth() != m_pBuffer->GetPixelDepth())
      {
         LogMessage(Warning, "Image file has a different format than expected.  Pixel values may get shifted.");
      }

      if( loadBuffer.GetWidth() != m_pBuffer->GetWidth() || loadBuffer.GetHeight() != m_pBuffer->GetHeight() )
      {
         LogMessage(Error, "Image file selected doesn't have same dimensions as buffer.");
         goto Cleanup;
      }

      loadBuffer.Load(path);
      m_pLocalBuffer->Copy(&loadBuffer);

		CString str;
      str.Format( "Loaded dark image: '%s'", (LPCTSTR)dlg.GetPathName());
		LogMessage( Info, str);
   }

   DarkImage();

Cleanup:
	if (m_pLocalBuffer != NULL)
	{
		m_pLocalBuffer->Destroy();
		delete m_pLocalBuffer;
		m_pLocalBuffer = NULL;
	}
}

void CFlatFieldDlg::DarkImage()
{
   CString str;
	SapFlatFieldStats stats;

	str.Format( "Correction type: '%s'", m_CorrectionType == SapFlatField::CorrectionTypeField? "Field": "Line");
   LogMessage(Info, str);

	str.Format( "Video type: '%s'", m_VideoType == SapAcquisition::VideoMono ? "Monochrome": "Bayer");
   LogMessage(Info, str);

	if( m_pXfer != NULL)
	{
		str.Format( "Number of frames to average: %d", m_NumFramesAverage);
	   LogMessage(Info, str);
	}

	if( m_CorrectionType == SapFlatField::CorrectionTypeLine)
	{
		str.Format( "Number of lines to average: %d", m_NumLinesAverage);
	   LogMessage(Info, str);

		str.Format( "Vertical offset from top: %d", m_VerticalOffset);
	   LogMessage(Info, str);	
	}

   LogMessage(Info, "Dark image calibration");

   if (!m_pFlatField->GetStats(m_pLocalBuffer, &stats))
   {
      LogMessage(Error, "   Unable to get image statistics");
      return;
   }

   // Uncomment this to consider the standard deviation in the 'tooManyBadPixels' decision
//#define USE_STANDARD_DEVIATION

   BOOL tooManyBadPixels = FALSE;
   int numComponents = stats.GetNumComponents();

   for (int i = 0; i < numComponents; i++)
   {
#ifdef USE_STANDARD_DEVIATION
      if (stats.GetAverage(i) + stats.GetStdDeviation(i) > m_RecommendedDark)
#else
      if (stats.GetAverage(i) > m_RecommendedDark)
#endif
      {
         tooManyBadPixels = TRUE;
         break;
      }
   }

   if (tooManyBadPixels && m_pFlatField->IsClippedGainOffsetDefects())
   {
      str.Format("The following statistics have been computed on the dark image: \n"
                 "    The average pixel value is %s\n"
#ifdef USE_STANDARD_DEVIATION
                 "    The standard deviation is %s\n"
#endif
                 "\nThis yields too many bad pixels above the hardware limit of %lu\n"
                 "\nTo disable bad pixels, uncheck the \'Consider as defective ...\'\n"
                 "checkbox, then acquire the dark image again",
                 (LPCTSTR)GetAverageStr(stats),
#ifdef USE_STANDARD_DEVIATION
                 (LPCTSTR)GetStdDeviationStr(stats),
#endif
                 m_RecommendedDark);

      AfxMessageBox(str, MB_OK);
      return;
   }
   else
   {
      str.Format("The following statistics have been computed on the dark image: \n"
                 "    The average pixel value is %s\n"
#ifdef USE_STANDARD_DEVIATION
                 "    The standard deviation is %s\n"
#endif
                 "\nWe recommend an average pixel value of less than %lu\n"
                 "\nDo you want to use this image?",
                 (LPCTSTR)GetAverageStr(stats),
#ifdef USE_STANDARD_DEVIATION
                 (LPCTSTR)GetStdDeviationStr(stats),
#endif
                 m_RecommendedDark);

      if (AfxMessageBox(str, MB_YESNO) != IDYES)
         return;
   }

	// Log pixel statistics
   str.Format("    Average pixel value: %s", (LPCTSTR)GetAverageStr( stats));
	LogMessage(Info, str);

#ifdef USE_STANDARD_DEVIATION
   str.Format("    Standard deviation: %s", (LPCTSTR)GetStdDeviationStr( stats));
	LogMessage(Info, str);
#endif

	str.Format("    Maximum deviation allowed from average pixel value: %lu", m_DeviationMaxBlack);
   LogMessage(Info, str);

   // Compute offset coefficients using last acquired image
   if (m_pFlatField->ComputeOffset(m_pLocalBuffer))
   {		
		// From this point on, the flatfield data is tainted until calibration is complete
		m_FlatFieldDataValid = FALSE; 

      GetDlgItem(IDOK)->EnableWindow(FALSE);
      GetDlgItem(IDC_SCG_FF_DARK_IMAGE)->EnableWindow(FALSE);
      GetDlgItem(IDC_SCG_FF_BRIGHT_IMAGE)->EnableWindow(TRUE);
   
	   m_CorrectionTypeCtrl.EnableWindow( FALSE);
		m_VideoTypeCtrl.EnableWindow( FALSE);
		m_CalibrationIndexCtrl.EnableWindow( FALSE);

		LogMessage(Info, "Calibration with a dark image has been done successfully");

		UpdateData(FALSE);
   }
   else
   {
      str = SapManager::GetLastStatus();
      LogMessage(Error, str);
   }
}

//
// Step 2: Snap a bright image to calculate the gain coefficients
//
void CFlatFieldDlg::OnBrightImage() 
{
	UpdateData();

	int nbImagesUsed = (m_pFlatField->GetCorrectionType() == SapFlatField::CorrectionTypeField) ? m_NumFramesAverage : 1;

	// Set maximum deviation from average pixel value for bright image
	m_pFlatField->SetDeviationMaxWhite( m_DeviationMaxWhite);

	// Set number of lines to average and vertical offset
	m_pFlatField->SetNumLinesAverage( m_NumLinesAverage);
	m_pFlatField->SetVerticalOffset( m_VerticalOffset);

	m_pFlatField->SetNumFramesAverage( nbImagesUsed);

	// Set wether to declare pixels with clipped coefficient as defective
	m_pFlatField->EnableClippedGainOffsetDefects(m_ClippedCoefsDefects == BST_CHECKED);

	if (m_pXfer && *m_pXfer)
   {
		m_pLocalBuffer = new SapBuffer(nbImagesUsed, m_pBuffer);
		m_pLocalBuffer->Create();

      // Acquire an image
      if (!Snap())
      {
         LogMessage(Error, "Unable to acquire an image");
         goto Cleanup;
      }
   }
   else
   {
      // Load an image
		m_pLocalBuffer = new SapBuffer(1, m_pBuffer);
		m_pLocalBuffer->Create();


      CLoadSaveDlg dlg(GetParent(), NULL, TRUE);
      if (dlg.DoModal() != IDOK)
         goto Cleanup;

      CString path = dlg.GetPathName();

      // Create a temporary buffer in order to know the selected file's native format and pixel depth

      SapBuffer loadBuffer(path);
      loadBuffer.Create();

      if(loadBuffer.GetFormat() != m_pBuffer->GetFormat() || loadBuffer.GetPixelDepth() != m_pBuffer->GetPixelDepth())
      {
         LogMessage(Warning, "Image file has a different format than expected.  Pixel values may get shifted.");
      }

      if( loadBuffer.GetWidth() != m_pBuffer->GetWidth() || loadBuffer.GetHeight() != m_pBuffer->GetHeight() )
      {
         LogMessage(Error, "Image file selected doesn't have same dimensions as buffer.");
         goto Cleanup;
      }

      loadBuffer.Load(path);
      m_pLocalBuffer->Copy(&loadBuffer);

		CString str;
      str.Format( "Loaded bright image: '%s'", (LPCTSTR)dlg.GetPathName());
		LogMessage( Info, str);
	}

   BrightImage();

Cleanup:
	if (m_pLocalBuffer != NULL)
	{
		m_pLocalBuffer->Destroy();
		delete m_pLocalBuffer;
		m_pLocalBuffer = NULL;
	}
}

void CFlatFieldDlg::BrightImage()
{
   CString str;
   SapFlatFieldStats stats;

	if( m_pXfer != NULL)
	{
		str.Format( "Number of frames to average: %d", m_NumFramesAverage);
	   LogMessage(Info, str);
	}

	if( m_CorrectionType == SapFlatField::CorrectionTypeLine)
	{
		str.Format( "Number of lines to average: %d", m_NumLinesAverage);
	   LogMessage(Info, str);

		str.Format( "Vertical offset from top: %d", m_VerticalOffset);
	   LogMessage(Info, str);	
	}

	LogMessage(Info, "Bright image calibration");

   // Get statistics on the (bright - dark) image
   if (!m_pFlatField->GetStats(m_pLocalBuffer, &stats))
   {
      LogMessage(Error, "Unable to get image statistic");
      return;
   }

   str.Format("The following statistics have been computed on the bright image\n"
              "after the substraction of the dark image:\n\n"
              "    The average pixel value is %s.\n"
				  "    The highest peak has been detected at %s.\n"
              "    %s pixels (%s) have a luminance value between %s to %s\n"
              "\nWe recommend at least %d for the highest peak value\n"
              "with %.2f%% pixels lying between the lower and the higher bound.\n"
              "\nDo you want to use this image?",
              (LPCTSTR)GetAverageStr( stats), 
				  (LPCTSTR)GetPeakPositionStr( stats), 
				  (LPCTSTR)GetNumPixelsStr( stats), 
				  (LPCTSTR)GetPixelRatioStr( stats), 
				  (LPCTSTR)GetLowStr( stats), 
				  (LPCTSTR)GetHighStr( stats),
              m_RecommendedBright, SapDefFlatFieldPixelRatio);

   if (AfxMessageBox(str, MB_YESNO) != IDYES)
      return;

   // Log average pixel value, lower and higher bounds and pixel ratio
   str.Format("    Average pixel value: %s", (LPCTSTR)GetAverageStr( stats));
   LogMessage(Info, str);
	str.Format("    Maximum deviation allowed from average pixel value: %lu", m_DeviationMaxWhite);
   LogMessage(Info, str);
   str.Format("    Highest peak position: %s", (LPCTSTR)GetPeakPositionStr( stats));
   LogMessage(Info, str);
   str.Format("    Lower bound: %s", (LPCTSTR)GetLowStr( stats));
   LogMessage(Info, str);
   str.Format("    Upper bound: %s", (LPCTSTR)GetHighStr( stats));
   LogMessage(Info, str);
   str.Format("    Number of pixels inside bounds: %s (%s)", (LPCTSTR)GetNumPixelsStr( stats), (LPCTSTR)GetPixelRatioStr( stats));
   LogMessage(Info, str);

   SapFlatFieldDefects defects;

   // Compute gain coefficient using last acquired image

   if (m_pFlatField->ComputeGain(m_pLocalBuffer, &defects, TRUE))
   {
      // Check for the presence of cluster (adjacent defective pixels)
      if (defects.GetNumClusters() != 0)
      {
         str.Format("%lu pixels (%.2f%% ) have been identified as being defective\n"
                     "with %lu clusters.\n"
                     "\nWe recommend less than %.2f%% of defective pixels with no cluster.\n"
                     "\nDo you still want to use this image?\n",
                     defects.GetNumDefects(), defects.GetDefectRatio(), defects.GetNumClusters(),
                     SapDefFlatFieldDefectRatio);

         if( AfxMessageBox(str, MB_YESNO) != IDYES)
            return;
      }

      // Log number of defective pixels detected
      str.Format("    Number of defective pixels detected: %lu (%.2f%%)", defects.GetNumDefects(), defects.GetDefectRatio());
      LogMessage(Info, str);

      // Log number of cluster detected
      str.Format("    Number of clusters detected: %lu", defects.GetNumClusters());
      LogMessage(Info, str);

      GetDlgItem(IDC_SCG_FF_DARK_IMAGE)->EnableWindow(TRUE);
      GetDlgItem(IDC_SCG_FF_BRIGHT_IMAGE)->EnableWindow(FALSE);
      GetDlgItem(IDOK)->EnableWindow(TRUE);

      BOOL isOnline = (m_pXfer && *m_pXfer);
      m_CorrectionTypeCtrl.EnableWindow(!isOnline && m_VideoType != SapAcquisition::VideoBayer);
		m_VideoTypeCtrl.EnableWindow( m_pXfer == NULL);
	   GetDlgItem(IDC_SCG_FF_FRAMES_AVG)->EnableWindow(m_pXfer != NULL);
		GetDlgItem(IDC_SCG_FF_LINES_AVG)->EnableWindow(m_CorrectionType == SapFlatField::CorrectionTypeLine);
		GetDlgItem(IDC_SCG_FF_VERTICAL_OFFSET)->EnableWindow(m_CorrectionType == SapFlatField::CorrectionTypeLine);
      m_CalibrationIndexCtrl.EnableWindow( m_pFlatField->GetNumFlatField() > 1);

		LogMessage(Info, "Calibration with a bright image has been done successfully");

		// Calibration is complete, turn this flag on
		m_FlatFieldDataValid = TRUE;

      GetDlgItem(IDC_SCG_SAVE_AND_UPLOAD)->EnableWindow(TRUE);
      GetDlgItem(IDC_SCG_FLATFIELD_SELECT)->EnableWindow(TRUE);


		UpdateData( FALSE);
   }
}

void CFlatFieldDlg::OnOK() 
{   
   CDialog::OnOK();

   if (m_pBuffer && *m_pBuffer)
      m_pBuffer->Clear();

	if (m_FlatFieldDataValid == FALSE)
		m_pFlatField->Clear();
}

void CFlatFieldDlg::OnCancel() 
{
   if (m_pBuffer && *m_pBuffer)
      m_pBuffer->Clear();

	// If calibration was only partially done, clear the flat field data.
	if (m_FlatFieldDataValid == FALSE)
		m_pFlatField->Clear();

	// If OK is enabled, it means we performed a calibration but want to clear the data.
	if (GetDlgItem(IDOK)->IsWindowEnabled())
		m_pFlatField->Clear();

   CDialog::OnCancel();
}

void CFlatFieldDlg::OnSelchangeCorrectionType() 
{
	UpdateData();
	
   m_CorrectionType = m_CorrectionTypeCtrl.GetCurSel() == 0 ? SapFlatField::CorrectionTypeField : SapFlatField::CorrectionTypeLine;

	GetDlgItem(IDC_SCG_FF_LINES_AVG)->EnableWindow(m_CorrectionType == SapFlatField::CorrectionTypeLine);
	GetDlgItem(IDC_SCG_FF_VERTICAL_OFFSET)->EnableWindow(m_CorrectionType == SapFlatField::CorrectionTypeLine);

	m_VideoTypeCtrl.EnableWindow( m_CorrectionType == SapFlatField::CorrectionTypeField);
}

void CFlatFieldDlg::OnSelchangeVideoType() 
{
	UpdateData();
	
	m_VideoType = m_VideoTypeCtrl.GetCurSel() == 0 ? SapAcquisition::VideoMono : SapAcquisition::VideoBayer;

	m_CorrectionTypeCtrl.EnableWindow( m_VideoType == SapAcquisition::VideoMono);
}

void CFlatFieldDlg::OnSelchangeCalibrationIndex() 
{
	UpdateData();

   int curSel = m_CalibrationIndexCtrl.GetCurSel();
   if (curSel != CB_ERR)
      m_CalibrationIndex = curSel;
}

// Snap
//    Acquire image(s)
//
BOOL CFlatFieldDlg::Snap()
{
	// Check if the transfer object is available
   if( m_pXfer == NULL || !*m_pXfer)
      return FALSE;

	for( int iFrame= 0; iFrame < m_pLocalBuffer->GetCount(); iFrame++)
	{
		// Acquire one image
		m_pXfer->Snap();

		// Wait until the acquired image has been transferred into system memory
		if (CAbortDlg(NULL, m_pXfer).DoModal() != IDOK)
		{
			m_pXfer->Abort();
			return FALSE;
		}
		
      //Add a short delay to ensure the transfer callback has time to arrive
		Sleep(100);
		if( m_pLocalBuffer != NULL)
		{
			m_pLocalBuffer->SetIndex(iFrame);
			m_pLocalBuffer->Copy(m_pBuffer);
		}
	}

   return TRUE;
}

void CFlatFieldDlg::LogMessage(int messageType, CString string)
{
   CString message;

   // Message header
   switch (messageType)
   {
      case Error:
         message = "[Err] ";
         break;
      case Warning:
         message = "[Wrn] ";
         break;
      case Info:
         message = "[Msg] ";
         break;
   }

   message += string;

   // Set text extent (horizontal scrollbar)
   CDC *pDC= m_LogMessageCtrl.GetDC();
   CSize size = pDC->GetTextExtent(message);
   int width = m_LogMessageCtrl.GetHorizontalExtent();

   if (size.cx > width)
   {
      width = size.cx;
      m_LogMessageCtrl.SetHorizontalExtent(size.cx);
   }

   m_LogMessageCtrl.ReleaseDC(pDC);

   // Add log message to control
   m_LogMessageCtrl.AddString(message);
   m_LogMessageCtrl.SetCurSel(m_LogMessageCtrl.GetCount() - 1);
}

CString CFlatFieldDlg::GetAverageStr( SapFlatFieldStats& stats) const
{
	CString str;

	if( stats.GetNumComponents() > 1)
	{
		str+= "[ ";
		for( int iComponent= 0; iComponent < stats.GetNumComponents(); iComponent++)
		{
			CString szComponent;;
			szComponent.Format( "%d", stats.GetAverage( iComponent));
			str+= szComponent;

			if( iComponent != stats.GetNumComponents()-1)
				str+= ", ";
		}
		str+= " ]";
	}
	else
	{
		str.Format( "%d", stats.GetAverage());
	}

	return str;
}

CString CFlatFieldDlg::GetStdDeviationStr( SapFlatFieldStats& stats) const
{
	CString str;

	if( stats.GetNumComponents() > 1)
	{
		str+= "[ ";
		for( int iComponent= 0; iComponent < stats.GetNumComponents(); iComponent++)
		{
			CString szComponent;;
			szComponent.Format( "%d", stats.GetStdDeviation( iComponent));
			str+= szComponent;

			if( iComponent != stats.GetNumComponents()-1)
				str+= ", ";
		}
		str+= " ]";
	}
	else
	{
		str.Format( "%d", stats.GetStdDeviation());
	}

	return str;
}

CString CFlatFieldDlg::GetLowStr( SapFlatFieldStats& stats) const
{
	CString str;

	if( stats.GetNumComponents() > 1)
	{
		str+= "[ ";
		for( int iComponent= 0; iComponent < stats.GetNumComponents(); iComponent++)
		{
			CString szComponent;;
			szComponent.Format( "%d", stats.GetLow( iComponent));
			str+= szComponent;

			if( iComponent != stats.GetNumComponents()-1)
				str+= ", ";
		}
		str+= " ]";
	}
	else
	{
		str.Format( "%d", stats.GetLow());
	}

	return str;
}

CString CFlatFieldDlg::GetHighStr( SapFlatFieldStats& stats) const
{
	CString str;

	if( stats.GetNumComponents() > 1)
	{
		str+= "[ ";
		for( int iComponent= 0; iComponent < stats.GetNumComponents(); iComponent++)
		{
			CString szComponent;;
			szComponent.Format( "%d", stats.GetHigh( iComponent));
			str+= szComponent;

			if( iComponent != stats.GetNumComponents()-1)
				str+= ", ";
		}
		str+= " ]";
	}
	else
	{
		str.Format( "%d", stats.GetHigh());
	}

	return str;
}

CString CFlatFieldDlg::GetPeakPositionStr( SapFlatFieldStats& stats) const
{
	CString str;

	if( stats.GetNumComponents() > 1)
	{
		str+= "[ ";
		for( int iComponent= 0; iComponent < stats.GetNumComponents(); iComponent++)
		{
			CString szComponent;;
			szComponent.Format( "%d", stats.GetPeakPosition( iComponent));
			str+= szComponent;

			if( iComponent != stats.GetNumComponents()-1)
				str+= ", ";
		}
		str+= " ]";
	}
	else
	{
		str.Format( "%d", stats.GetPeakPosition());
	}

	return str;
}

CString CFlatFieldDlg::GetPixelRatioStr( SapFlatFieldStats& stats) const
{
	CString str;

	if( stats.GetNumComponents() > 1)
	{
		str+= "[ ";
		for( int iComponent= 0; iComponent < stats.GetNumComponents(); iComponent++)
		{
			CString szComponent;;
			szComponent.Format( "%.2f%%", stats.GetPixelRatio( iComponent));
			str+= szComponent;

			if( iComponent != stats.GetNumComponents()-1)
				str+= ", ";
		}
		str+= " ]";
	}
	else
	{
		str.Format( "%.2f%%", stats.GetPixelRatio());
	}

	return str;
}

CString CFlatFieldDlg::GetNumPixelsStr( SapFlatFieldStats& stats) const
{
	CString str;

	if( stats.GetNumComponents() > 1)
	{
		str+= "[ ";
		for( int iComponent= 0; iComponent < stats.GetNumComponents(); iComponent++)
		{
			CString szComponent;;
			szComponent.Format( "%d", stats.GetNumPixels( iComponent));
			str+= szComponent;

			if( iComponent != stats.GetNumComponents()-1)
				str+= ", ";
		}
		str+= " ]";
	}
	else
	{
		str.Format( "%d", stats.GetNumPixels());
	}

	return str;
}



void CFlatFieldDlg::OnSaveAndUpload()
{
   CFileDialog dlg( FALSE, "", "FFC.tif", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "TIFF Files (*.tif)|*.tif|", this);
	
	if( dlg.DoModal() == IDOK)
	{
		// Save flat field offset correction file
		m_pFlatField->Save(dlg.GetPathName());
      LogMessage(0x02, "File saved successfully.");
      if (m_UserFlatFieldCount > 1 && !m_isGenie)
      {
         CWaitCursor waitCursor;
         LogMessage(0x02, "Uploading file to device... Please wait.");
         int index = (DWORD)m_FlatFieldSelector.GetItemData(m_FlatFieldSelector.GetCurSel());
         int timeout = SapManager::GetCommandTimeout();
         SapManager::SetCommandTimeout(5*60*1000); //5 mintues timeout
         if (m_pFlatField->GetAcqDevice()->WriteFile(dlg.GetPathName(),index))
            LogMessage(0x02, "File upload completed successfully.");
         else
            LogMessage(0x02, "File upload failed.");
         //restore initial timeout
         SapManager::SetCommandTimeout(timeout);
      }
      
	}
}
