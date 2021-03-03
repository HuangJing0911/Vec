#ifndef _SAPFLATFIELD_H_
#define _SAPFLATFIELD_H_

// SapFlatField.h : header file
//

#include "SapClassBasicDef.h"

// Default ratios
const float SapDefFlatFieldPixelRatio				=99.00f;    // 99 percent of all pixels
const float SapDefFlatFieldDefectRatio				= 1.00f;    // Up to 1 percent of defective pixels
const float SapDefFlatFieldAvgFactorBlack       = 0.25f;		// 0.25 * maximum pixel value
const float SapDefFlatFieldAvgFactorWhite       = 0.80f;		// 0.80 * maximum pixel value
const float SapDefFlatFieldDeviationMax         = 0.25f;		// 0.25 * maximum pixel value

// Forward declarations
class SAPCLASSBASIC_CLASS SapFlatFieldStats;
class SAPCLASSBASIC_CLASS SapFlatFieldDefects;
class SapFlatFieldDevice;

//
// SapFlatField class declaration
//
class SAPCLASSBASIC_CLASS SapFlatField : public SapManager
{
public:
   // Various constants
   enum MiscValues
   {
      DefNumLinesAverage = 128,
		DefNumFramesAverage = 10,

		DefGainMin    = 0,
		DefGainMax    = 255,
      DefGainDivisor= 128,
      DefGainBase   = 0,

		DefOffsetMin= 0,
		DefOffsetMax= 255,
		DefOffsetSoftwareMax= 4095,
      DefOffsetFactor= 1,

      DefDeviationFactorMax = 4,				

      InvalidFlatFieldNumber= -1
   };

	enum CorrectionType
	{
		CorrectionTypeField,
		CorrectionTypeLine,
		CorrectionTypeInvalid= -1
	};

   enum CalibrationAlgorithm
   {
      CalibrationMethod1 = 0,
      CalibrationMethodUnknown = -1
   };

public:
	// Constructor/Destructor
   SapFlatField();
   SapFlatField(SapAcquisition *pAcq, int numFlatField = 1);
   SapFlatField(SapAcqDevice *pAcqDevice);
   SapFlatField(SapBuffer *pBuffer, int numFlatField = 1);
   SapFlatField(const SapFlatField &flatField);
   virtual ~SapFlatField();

	SapFlatField &operator= (const SapFlatField &flatField);

	// Creation/destruction
   virtual BOOL Create();
   virtual BOOL Destroy();

public:
	// Access to implementation
   SapAcquisition *GetAcquisition() const { return m_pAcq; }
   SapAcqDevice *GetAcqDevice()     const { return m_pAcqDevice; }
   SapBuffer *GetBuffer()           const { return m_pBuffer; }
   SapBuffer *GetBufferOffset()     const { return m_pBufferOffset; }
   SapBuffer *GetBufferGain()       const { return m_pBufferGain; }

   BOOL GetBufferOffset(SapBuffer *pBuffer, int bufIndex = 0, int offsetIndex = 0);
   BOOL GetBufferGain(SapBuffer *pBuffer, int bufIndex = 0, int gainIndex = 0);

   BOOL IsEnabled()          const { return m_IsEnabled; }
	BOOL IsPixelReplacement() const { return m_bPixelReplacement;}
	BOOL IsClippedGainOffsetDefects() const { return m_ClippedGainOffsetDefects; }
   BOOL IsSoftware()         const { return !m_IsHardware || !m_IsHardwareEnabled; }

   CorrectionType GetCorrectionType()   const { return m_CorrectionType; }
	int GetDeviationMaxBlack()           const { return m_DeviationMaxBlack;}
	float GetBlackPixelPercentage()      const { return m_wantedBlackPixelPercentage;}
	int GetDeviationMaxWhite()           const { return m_DeviationMaxWhite;}
   int GetGainDivisor()                 const { return m_GainDivisor; }
   int GetGainBase()                    const { return m_GainBase; }
   void GetGainMinMax(int *pGainMin, int *pGainMax);
   void GetOffsetMinMax(int *pOffsetMin, int *pOffsetMax);
   double GetOffsetFactor()             const { return m_OffsetFactor; }
   int GetNumLinesAverage()             const { return m_NumLinesAverage; }
	int GetNumFramesAverage()            const { return m_NumFramesAverage; }
	int GetVerticalOffset()              const { return m_VerticalOffset; }
	SapAcquisition::VideoType GetVideoType()	const { return m_VideoType; }
   int GetNumFlatField()                const { return m_NumFlatField; }
   int GetIndex()                       const { return m_Index; }

	virtual BOOL SetAcquisition(SapAcquisition *pAcq);
	virtual BOOL SetAcqDevice(SapAcqDevice *pAcqDevice);
	virtual BOOL SetBlackPixelPercentage(float blackPixelPercent);
   virtual BOOL SetBuffer(SapBuffer *pBuffer);
   virtual BOOL SetCorrectionType( CorrectionType correctionType);
	virtual BOOL SetDeviationMaxBlack( int deviationMax);
	virtual BOOL SetDeviationMaxWhite( int deviationMax);
   virtual BOOL SetGainDivisor(int gainDivisor);
   virtual BOOL SetGainMinMax(int gainMin, int gainMax);
   virtual BOOL SetGainBase(int gainBase);
   virtual BOOL SetOffsetMinMax(int offsetMin, int offsetMax);
   virtual BOOL SetOffsetFactor(double offsetFactor);
   virtual BOOL SetNumLinesAverage(int numLinesAverage);
	virtual BOOL SetNumFramesAverage(int numFramesAverage);
	virtual BOOL SetVerticalOffset( int verticalOffset);
   virtual BOOL SetVideoType( SapAcquisition::VideoType videoType, SapBayer::Align alignment = SapBayer::AlignAll);
   virtual BOOL SetNumFlatField( int numFlatField);
	virtual BOOL SetRegionOfInterest( int leftOffset, int topOffset, int width, int height);

   virtual BOOL SetIndex( int index);
   virtual void Next() { m_Index++; if (m_Index == m_NumFlatField) m_Index = 0; }

   // Load/save gain and offset buffers from/to disk files
	virtual BOOL Load( const char *fileName);
	virtual BOOL Save( const char *fileName);

   // Load gain and offset buffers from buffer objects
	virtual BOOL Load( SapBuffer* pBufferGain, SapBuffer* pBufferOffset);

   // Clear values for gain and offset buffers
   virtual BOOL Clear();

	// Read offset and gain coefficients from the acquisition device.
	virtual BOOL ReadGainOffsetFromDevice();

	// Compute values for gain and offset buffers
	// NOTE: the numImages parameter is obsolete.  Use SetNumFramesAverage() instead.
   virtual BOOL ComputeOffset(SapBuffer *pBuffer, int numImages = 0);
   virtual BOOL ComputeGain(SapBuffer *pBuffer, SapFlatFieldDefects *pDefects, BOOL bUseImageMaxValue = TRUE, int numImages = 0);
   virtual BOOL ComputeGain(SapBuffer *pBuffer, SapFlatFieldDefects *pDefects, SapData target);

   // Execution control
	virtual BOOL EnableClippedGainOffsetDefects( BOOL enable= TRUE);
	virtual BOOL EnablePixelReplacement( BOOL enable= TRUE);
   virtual BOOL Enable(BOOL enable = TRUE, BOOL useHardware= TRUE);
   virtual BOOL Execute(SapBuffer *pBuffer);
	virtual BOOL Execute(SapBuffer *pBuffer, int bufIndex);

   // Buffer statictics
   virtual BOOL GetAverage(SapBuffer *pBuffer, SapFlatFieldStats *pStats);
   virtual BOOL GetStats(SapBuffer *pBuffer, SapFlatFieldStats *pStats);
   
   // Obsolete methods
   BOOL IsHardware() const { return m_IsHardware; }
   virtual BOOL Load( const char *gainFile, const char *offsetFile);
   virtual BOOL Save( const char *gainFile, const char *offsetFile);
   virtual BOOL ComputeGain(SapBuffer *pBuffer, int deviationMax, SapFlatFieldDefects *pDefects, BOOL bUseImageMaxValue = TRUE, int numImages = 0);
   virtual BOOL ComputeGain(SapBuffer *pBuffer, int blackDeviationMax, int whiteDeviationMax, SapFlatFieldDefects *pDefects, BOOL bUseImageMaxValue = TRUE, int numImages = 0, SapData *pTarget = NULL);
   virtual BOOL GetAverage(SapBuffer *pBuffer, int *pAverage, int *pStdDeviation = NULL, int iComponent= 0);

protected:
	// Utility methods
	virtual BOOL CreateRemaining();
   virtual void Construct(SapAcquisition *pAcq, SapAcqDevice *pAcqDevice, SapBuffer *pBuffer, int numFlatField);

   virtual BOOL IsFileCompatible(const char *filename, SapBuffer *pBuffer) const;
	virtual BOOL IsBufferCompatible( SapBuffer *pBufferSrc, SapBuffer *pBufferDst) const;
	virtual BOOL IsFormatCompatible(SapFormat format) const;
	virtual BOOL GetPixelDepth( int* pixelDepth) const;

   template<class DATATYPE>
   BOOL FFCAverageFrames(SapBuffer *pBuffer, int numImages, SapBuffer *pBufferAverage, BOOL bBlackImage = FALSE);

   template<class DATATYPE>
   BOOL FFCAverageLines(void *pBuffer, int verticalOffset, int width, int height, SapFormat format, void *pBufferAverage, BOOL bBlackImage = FALSE);

   template<class BUFTYPE, class OFFSETTYPE>
   BOOL FFCComputeOffset(SapBuffer *pAvgBuffer);

   template<class BUFTYPE, class FFCTYPE>
   BOOL FFCComputeGain(SapBuffer *pBuffer, SapBuffer *pBufferOffset, float pixelRatio, int offsetDeviationMax, int whiteDeviationMax, SapBuffer *pBufferGain, SapFlatFieldDefects *pDefects, int iLineOffset= 0, int iLineStep= 1, int iColumnOffset= 0, int iColumnStep= 1, int bitDepth = -1, int target = -1);

   template<class DATATYPE>
   BOOL FFCGetAverage(SapBuffer *pBuffer, int *pAverage, int *pStdDeviation, int iLineOffset= 0, int iLineStep= 1, int iColumnOffset= 0, int iColumnStep= 1);

   template<class BUFTYPE, class OFFSETTYPE>
   BOOL FFCGetStats(SapBuffer *pBuffer, SapBuffer *pBufferOffset, float pixelRatio, SapFlatFieldStats *pStats, int iLineOffset= 0, int iLineStep= 1, int iColumnOffset= 0, int iColumnStep= 1, int iComponent= 0);

   template<class BUFTYPE, class OFFSETTYPE>
   BOOL FFCGetHistogram(SapBuffer *pBuffer, SapBuffer* pBufferOffset, int **ppHistogram, int *pNumBins, int iLineOffset= 0, int iLineStep= 1, int iColumnOffset= 0, int iColumnStep= 1, double offsetFactor = DefOffsetFactor);

   virtual BOOL FFCHistPeaks( int* pHistogram, int numBins, int vicinity, SapDataPoint** ppPeaks, int *pnPeaks);

   template<class BUFTYPE, class FFCTYPE>
   BOOL FFCExecute(BUFTYPE *pBuffer, int width, int height, SapFormat format, FFCTYPE *pBufferOffset, FFCTYPE *pBufferGain, int isLineScan);

   template<class BUFTYPE, class FFCTYPE>
   BOOL FFCExecuteAoI(BUFTYPE *pBuffer, int width, int height, int leftOffset, int topOffset, SapFormat format, FFCTYPE *pBufferOffset, FFCTYPE *pBufferGain, int coefWidth, int coefHeight, int isLineScan);

   BOOL FFCGetComponentTarget(int iComponent, SapDataRGB *pRgbTarget, SapBayer::Align bayerAlignment, int *pComponentTarget);
protected:
   SapAcquisition *m_pAcq;
   SapAcqDevice   *m_pAcqDevice;
   SapBuffer      *m_pBuffer;
   SapBuffer      *m_pBufferOffset;
   SapBuffer      *m_pBufferGain;
   SapFlatFieldDevice *m_pDevice;

   BOOL m_IsEnabled;
   BOOL m_FlatFieldEnable;       // Current value for 'enable' parameter
   BOOL m_IsHardware;
   BOOL m_IsHardwareEnabled;
   int  m_GainDivisor;
   int  m_GainMin;
   int  m_GainMax;
   int  m_GainBase;
   int  m_OffsetMin;
   int  m_OffsetMax;
   double  m_OffsetFactor;
   int  m_NumLinesAverage;
   int  m_NumFramesAverage;
   int  m_VerticalOffset;
   int  m_DeviationMaxBlack;
   int  m_DeviationMaxWhite;
   BOOL m_bPixelReplacement;
   int  m_NumFlatField;
   int  m_Index;
   BOOL m_ClippedGainOffsetDefects;
   float m_wantedBlackPixelPercentage;
   SapFlatField::CalibrationAlgorithm m_CalibrationAlgorithm;

   // For Software FFC when ROI is used
   int  m_LeftOffsetROI;
   int  m_TopOffsetROI;
   int  m_WidthROI;
   int  m_HeightROI;

   SapAcquisition::VideoType m_VideoType;
   CorrectionType m_CorrectionType;
   SapXferParams m_XferParams;
   SapBayer::Align m_BayerAlignment;

   BOOL m_NeedCoefficientsReload;
};

//
// SapFlatFieldDefects class declaration
//
class SAPCLASSBASIC_CLASS SapFlatFieldDefects
{
public:
   SapFlatFieldDefects()
   {
      m_NumDefects = 0;
      m_NumClusters = 0;
      m_DefectRatio = 0.0f;
   }
   ~SapFlatFieldDefects() {}

   int GetNumDefects()    const { return m_NumDefects; }
   int GetNumClusters()   const { return m_NumClusters; }
   float GetDefectRatio() const { return m_DefectRatio; }

   void SetNumDefects(int numDefects)     { m_NumDefects = numDefects; }
   void SetNumClusters(int numClusters)   { m_NumClusters = numClusters; }
   void SetDefectRatio(float defectRatio) { m_DefectRatio = defectRatio; }

   void IncrementNumDefects()  { m_NumDefects++; }
   void IncrementNumClusters() { m_NumClusters++; }

protected:
   int   m_NumDefects;
   int   m_NumClusters;
   float m_DefectRatio;
};

//
// SapFlatFieldStats class declaration
//
class SAPCLASSBASIC_CLASS SapFlatFieldStats
{
public:
   enum MiscValues
   {
		MaxComponent= 4
   };

	SapFlatFieldStats()
   {
		m_NumComponents= 1;

      memset( m_Average, 0, sizeof( m_Average));
      memset( m_StdDeviation, 0, sizeof( m_StdDeviation));
      memset( m_PeakPosition, 0, sizeof( m_PeakPosition));
      memset( m_Low, 0, sizeof( m_Low));
      memset( m_High, 0, sizeof( m_High));
      memset( m_NumPixels, 0, sizeof( m_NumPixels));
		memset( m_PixelRatio, 0, sizeof( m_PixelRatio));
   }
   ~SapFlatFieldStats() {}

	int GetAverage( int iComponent= 0)			const { return m_Average[ iComponent]; }
	int GetStdDeviation( int iComponent= 0)	const { return m_StdDeviation[ iComponent]; }
	int GetPeakPosition( int iComponent= 0)	const { return m_PeakPosition[ iComponent]; }
	int GetLow( int iComponent= 0)				const { return m_Low[ iComponent]; }
	int GetHigh( int iComponent= 0)				const { return m_High[ iComponent]; }
	int GetNumPixels( int iComponent= 0)		const { return m_NumPixels[ iComponent]; }
	float GetPixelRatio( int iComponent= 0)	const { return m_PixelRatio[ iComponent]; }
	int GetNumComponents()							const { return m_NumComponents;}

   void SetAverage(int iComponent, int average)           { m_Average[iComponent] = average; }
   void SetStdDeviation(int iComponent, int stdDeviation) { m_StdDeviation[iComponent] = stdDeviation; }
   void SetPeakPosition(int iComponent, int peakPosition) { m_PeakPosition[iComponent] = peakPosition; }
   void SetLow(int iComponent, int low)                   { m_Low[iComponent] = low; }
   void SetHigh(int iComponent, int high)                 { m_High[iComponent] = high; }
   void SetNumPixels(int iComponent, int numPixels)       { m_NumPixels[iComponent] = numPixels; }
   void SetPixelRatio(int iComponent, float pixelRatio)   { m_PixelRatio[iComponent] = pixelRatio; }
   void SetNumComponents(int numComponents)               { m_NumComponents = numComponents; }

   void DecrementLow(int iComponent) { m_Low[iComponent]--; }
   void IncrementHigh(int iComponent) { m_High[iComponent]++; }

protected:
	int m_NumComponents;
	int m_Average[ MaxComponent];
	int m_StdDeviation[ MaxComponent];
	int m_PeakPosition[ MaxComponent];
	int m_Low[ MaxComponent];
	int m_High[ MaxComponent];
	int m_NumPixels[ MaxComponent];
	float m_PixelRatio[ MaxComponent];
};

class SapFlatFieldDevice : public SapManager
{
public:
   virtual BOOL Create() = 0;
   virtual BOOL Destroy() = 0;

   virtual BOOL IsEnabled() = 0;
   virtual BOOL IsHwFlatFieldSupported() = 0;
   virtual BOOL GetCorrectionType(SapFlatField::CorrectionType *pCorrectionType) = 0;
   virtual BOOL GetVideoType(SapAcquisition::VideoType *pVideoType) = 0;
   virtual BOOL GetBayerAlignment(SapBayer::Align *pAlignment) = 0;
   virtual BOOL GetFlatFieldBufferParams(int *pWidth, int *pHeight, SapFormat *pFormat) = 0;
   virtual BOOL GetAlgorithmParams(SapFlatField::CalibrationAlgorithm *pMethod, int *pGainMin, int *pGainMax, int *pGainDivisor, int *pGainBase, int *pOffsetMin, int *pOffsetMax, double *pOffsetFactor) = 0;
   virtual BOOL GetAOI(int *pLeft, int *pWidth, int *pTop, int *pHeight) = 0;
   virtual SapXferParams GetXferParams() = 0;

   virtual BOOL Enable(BOOL enable) = 0;
   virtual BOOL SetNumFlatField(int numFlatField) {m_NumFlatField = numFlatField; return TRUE;};
   virtual BOOL ReadGainOffset(SapBuffer *pGainBuffer, SapBuffer *pOffsetBuffer) = 0;
   virtual BOOL WriteGainOffset(SapBuffer *pGainBuffer, SapBuffer *pOffsetBuffer) = 0;
   virtual BOOL PostComputeGain(SapBuffer *pGainBuffer, SapBuffer *pOffsetBuffer) = 0;

protected:
   BOOL m_bInitOK;
   int m_NumFlatField;
};

class SapFlatFieldAcq : public SapFlatFieldDevice
{
public:
   SapFlatFieldAcq(SapAcquisition* pAcq, int numFlatField);
   BOOL Create();
   BOOL Destroy();

   BOOL IsEnabled();
   BOOL IsHwFlatFieldSupported();
   BOOL GetCorrectionType(SapFlatField::CorrectionType *pCorrectionType);
   BOOL GetVideoType(SapAcquisition::VideoType *pVideoType);
   BOOL GetBayerAlignment(SapBayer::Align *pAlignment);
   BOOL GetFlatFieldBufferParams(int *pWidth, int *pHeight, SapFormat *pFormat);
   BOOL GetAlgorithmParams(SapFlatField::CalibrationAlgorithm *pMethod, int *pGainMin, int *pGainMax, int *pGainDivisor, int *pGainBase, int *pOffsetMin, int *pOffsetMax, double *pOffsetFactor);
   BOOL GetAOI(int *pLeft, int *pWidth, int *pTop, int *pHeight);
   SapXferParams GetXferParams() {return m_pAcq->GetXferParams();}

   BOOL Enable(BOOL enable);
   BOOL ReadGainOffset(SapBuffer *pGainBuffer, SapBuffer *pOffsetBuffer);
   BOOL WriteGainOffset(SapBuffer *pGainBuffer, SapBuffer *pOffsetBuffer);
   BOOL PostComputeGain(SapBuffer *pGainBuffer, SapBuffer *pOffsetBuffer) {return TRUE;}
   
protected:
   enum MiscValues
   {
      InvalidFlatFieldNumber = -1,
   };
   SapAcquisition *m_pAcq;

   int *m_FlatFieldNumbers;     // Flat-field numbers returned from acquisition device

};

class SapFlatFieldGenie : public SapFlatFieldDevice
{
public:
   SapFlatFieldGenie(SapAcqDevice*);
   BOOL Create();
   BOOL Destroy();

   BOOL IsEnabled();
   BOOL IsHwFlatFieldSupported();
   BOOL GetCorrectionType(SapFlatField::CorrectionType *pCorrectionType);
   BOOL GetVideoType(SapAcquisition::VideoType *pVideoType);
   BOOL GetBayerAlignment(SapBayer::Align *pAlignment);
   BOOL GetFlatFieldBufferParams(int *pWidth, int *pHeight, SapFormat *pFormat);
   BOOL GetAlgorithmParams(SapFlatField::CalibrationAlgorithm *pMethod, int *pGainMin, int *pGainMax, int *pGainDivisor, int *pGainBase, int *pOffsetMin, int *pOffsetMax, double *pOffsetFactor);
   BOOL GetAOI(int *pLeft, int *pWidth, int *pTop, int *pHeight);
   SapXferParams GetXferParams() {return m_pAcqDevice->GetXferParams();}

   BOOL Enable(BOOL enable);
   BOOL ReadGainOffset(SapBuffer *pGainBuffer, SapBuffer *pOffsetBuffer);
   BOOL WriteGainOffset(SapBuffer *pGainBuffer, SapBuffer *pOffsetBuffer);
   BOOL PostComputeGain(SapBuffer *pGainBuffer, SapBuffer *pOffsetBuffer) {return TRUE;}

protected:
   BOOL FFCSetReplacementInOffset(SapBuffer *pBufferOffset, SapBuffer* pBufferGain);
   BOOL ConvertBuffer(SapBuffer* pSrcBuffer, SapBuffer *pDestBuffer);

protected:
   SapAcqDevice *m_pAcqDevice;
   SapFormat m_AcqDeviceFlatFieldFormat;
   BOOL m_IsFlatFieldEnableFeatureWritable;

   static char *m_ScanTypeFeatureName;
   static char *m_ColorTypeFeatureName;
   static char *m_FlatFieldEnableFeatureName;
   static char *m_FlatFieldWidthFeatureName;
   static char *m_FlatFieldHeightFeatureName;
   static char *m_FlatFieldFormatFeatureName;
   static char *m_FlatFieldGainMinFeatureName;
   static char *m_FlatFieldGainMaxFeatureName;
   static char *m_FlatFieldOffsetMinFeatureName;
   static char *m_FlatFieldOffsetMaxFeatureName;
   static char *m_FlatFieldGainBufferFeatureName;
   static char *m_FlatFieldOffsetBufferFeatureName;
};

class SapFlatFieldDFNC : public SapFlatFieldDevice
{
public:
   SapFlatFieldDFNC(SapAcqDevice*);
   BOOL Create();
   BOOL Destroy();

   BOOL IsEnabled();
   BOOL IsHwFlatFieldSupported();
   BOOL GetCorrectionType(SapFlatField::CorrectionType *pCorrectionType);
   BOOL GetVideoType(SapAcquisition::VideoType *pVideoType);
   BOOL GetBayerAlignment(SapBayer::Align *pAlignment);
   BOOL GetFlatFieldBufferParams(int *pWidth, int *pHeight, SapFormat *pFormat);
   BOOL GetAlgorithmParams(SapFlatField::CalibrationAlgorithm *pMethod, int *pGainMin, int *pGainMax, int *pGainDivisor, int *pGainBase, int *pOffsetMin, int *pOffsetMax, double *pOffsetFactor);
   BOOL GetAOI(int *pLeft, int *pWidth, int *pTop, int *pHeight);
   SapXferParams GetXferParams() {return m_pAcqDevice->GetXferParams();}

   BOOL Enable(BOOL enable);
   BOOL ReadGainOffset(SapBuffer *pGainBuffer, SapBuffer *pOffsetBuffer);
   BOOL WriteGainOffset(SapBuffer *pGainBuffer, SapBuffer *pOffsetBuffer);
   BOOL PostComputeGain(SapBuffer *pGainBuffer, SapBuffer *pOffsetBuffer);
protected:
   BOOL FFCSetReplacementInOffset(SapBuffer *pBufferOffset, SapBuffer *pBufferGain);
protected:
   SapAcqDevice *m_pAcqDevice;

   static char *m_ScanTypeFeatureName;
   static char *m_ColorTypeFeatureName;
   static char *m_PixelColorFilterFeatureName;
   static char *m_FlatFieldModeFeatureName;
   static char *m_FlatFieldCalMethodFeatureName;
   static char *m_FlatFieldWidthFeatureName;
   static char *m_FlatFieldHeightFeatureName;
   static char *m_FlatFieldFormatFeatureName;
   static char *m_FlatFieldGainMinFeatureName;
   static char *m_FlatFieldGainMaxFeatureName;
   static char *m_FlatFieldGainDivisorFeatureName;
   static char *m_FlatFieldGainBaseFeatureName;
   static char *m_FlatFieldOffsetMinFeatureName;
   static char *m_FlatFieldOffsetMaxFeatureName;
   static char *m_FlatFieldOffsetFactorFeatureName;
   static char *m_FlatFieldGainBufferFeatureName;
   static char *m_FlatFieldOffsetBufferFeatureName;
};

#endif
