#ifndef _SAPACQUISITION_H_
#define _SAPACQUISITION_H_

//	SapAcquisition.h : header file
//

#include "SapClassBasicDef.h"

#define ACQCFG_DEFAULT_EXTENSION		"ccf"
#define ACQCFG_DEFAULT_FOLDER			"\\CamFiles\\User"

#define ACQCFG_SECTION_BOARD			"Board"
#define ACQCFG_KEY_SERVER_NAME		"Server Name"
#define ACQCFG_KEY_DEVICE_NAME		"Device Name"
#define ACQCFG_KEY_DEVICE_INDEX		"Device Index"

#define ACQCFG_SECTION_CUSTOM_PARAM "Custom Parameters"
#define ACQCFG_CUSTOM_PARAM_ENTRY   "Parameter_%d"
#define ACQCFG_CUSTOM_PARAM_DEFAULT "0,0,0,No Parameter"
#if COR_LINUX
#define ACQCFG_CUSTOM_PARAM_FORMAT  "%i,%i,%i"
#else
#define ACQCFG_CUSTOM_PARAM_FORMAT  "%li,%li,%li"
#endif

//
// SapAcquisition class declaration
//

// Forward declarations
class SAPCLASSBASIC_CLASS SapAcqCallbackInfo;
typedef void (*SapAcqCallback)(SapAcqCallbackInfo *);

class SAPCLASSBASIC_CLASS SapAcquisition : public SapXferNode
{
public:
   // Acquisition event types
   typedef int EventType;     // For compatibility with old Sapera++ application code
   enum _EventType
   {
      EventNone            = 0,
      EventShaftEncoderReverseCountOverflow = CORACQ_VAL_EVENT_TYPE_SHAFT_ENCODER_REVERSE_COUNT_OVERFLOW,
      EventLineTriggerTooFast = CORACQ_VAL_EVENT_TYPE_LINE_TRIGGER_TOO_FAST,
      EventLinkError       = CORACQ_VAL_EVENT_TYPE_LINK_ERROR,
      EventVerticalTimeout = CORACQ_VAL_EVENT_TYPE_VERTICAL_TIMEOUT,
      EventExternalTrigger2 = CORACQ_VAL_EVENT_TYPE_EXTERNAL_TRIGGER2,
      EventVirtualFrame    = CORACQ_VAL_EVENT_TYPE_VIRTUAL_FRAME,
      EventUserDefine      = CORACQ_VAL_EVENT_TYPE_USER_DEFINE,
      EventExternalTriggerTooSlow = CORACQ_VAL_EVENT_TYPE_EXT_LINE_TRIGGER_TOO_SLOW,
      EventExtLineTriggerTooSlow = CORACQ_VAL_EVENT_TYPE_EXT_LINE_TRIGGER_TOO_SLOW,
      EventHsyncLock       = CORACQ_VAL_EVENT_TYPE_HSYNC_LOCK,
      EventHsyncUnlock     = CORACQ_VAL_EVENT_TYPE_HSYNC_UNLOCK,
      EventExternalTriggerIgnored = CORACQ_VAL_EVENT_TYPE_EXTERNAL_TRIGGER_IGNORED,
      EventDataOverflow    = CORACQ_VAL_EVENT_TYPE_DATA_OVERFLOW,
      EventFrameLost       = CORACQ_VAL_EVENT_TYPE_FRAME_LOST,
      EventStartOfField    = CORACQ_VAL_EVENT_TYPE_START_OF_FIELD,
      EventStartOfOdd      = CORACQ_VAL_EVENT_TYPE_START_OF_ODD,
      EventStartOfEven     = CORACQ_VAL_EVENT_TYPE_START_OF_EVEN,
      EventStartOfFrame    = CORACQ_VAL_EVENT_TYPE_START_OF_FRAME,
      EventEndOfField      = CORACQ_VAL_EVENT_TYPE_END_OF_FIELD,
      EventEndOfOdd        = CORACQ_VAL_EVENT_TYPE_END_OF_ODD,
      EventEndOfEven       = CORACQ_VAL_EVENT_TYPE_END_OF_EVEN,
      EventEndOfFrame      = CORACQ_VAL_EVENT_TYPE_END_OF_FRAME,
      EventExternalTrigger = CORACQ_VAL_EVENT_TYPE_EXTERNAL_TRIGGER,
      EventVerticalSync    = CORACQ_VAL_EVENT_TYPE_VERTICAL_SYNC,
      EventEndOfLine       = CORACQ_VAL_EVENT_TYPE_END_OF_LINE,
      EventEndOfNLines     = CORACQ_VAL_EVENT_TYPE_END_OF_NLINES,
      EventNoHSync         = CORACQ_VAL_EVENT_TYPE_NO_HSYNC,
      EventNoVSync         = CORACQ_VAL_EVENT_TYPE_NO_VSYNC,
      EventNoPixelClk      = CORACQ_VAL_EVENT_TYPE_NO_PIXEL_CLK,
      EventPixelClk        = CORACQ_VAL_EVENT_TYPE_PIXEL_CLK,
      EventMask            = CORACQ_VAL_EVENT_TYPE_MASK
   };

   // Software trigger types
   typedef int SoftwareTriggerType;
   enum _SoftwareTriggerType
   {
      SoftwareTriggerExt      = CORACQ_VAL_SOFTWARE_TRIGGER_EXT,
      SoftwareTriggerExtFrame = CORACQ_VAL_SOFTWARE_TRIGGER_EXT_FRAME,
      SoftwareTriggerExtLine  = CORACQ_VAL_SOFTWARE_TRIGGER_EXT_LINE
   };

   // Flipping modes
   typedef int FlipMode;
   enum _FlipMode
   {
      FlipNone       = CORACQ_VAL_FLIP_OFF,
      FlipHorizontal = CORACQ_VAL_FLIP_HORZ,
      FlipVertical   = CORACQ_VAL_FLIP_VERT
   };

   // Time stamp base
   typedef int TimeStampBase;
   enum _TimeStampBase
   {
      TimeStampNone              = 0,
      TimeStampMicroSec          = CORACQ_VAL_TIME_BASE_US,
      TimeStampMilliSec          = CORACQ_VAL_TIME_BASE_MS,
      TimeStampLineValid         = CORACQ_VAL_TIME_BASE_LINE_VALID,
      TimeStampLineTrigger       = CORACQ_VAL_TIME_BASE_LINE_TRIGGER,
      TimeStampFrameValid        = CORACQ_VAL_TIME_BASE_FRAME_VALID,
      TimeStampFrameTrigger      = CORACQ_VAL_TIME_BASE_FRAME_TRIGGER,
      TimeStampShaftEncoder      = CORACQ_VAL_TIME_BASE_SHAFT_ENCODER,
      TimeStampNanoSec           = CORACQ_VAL_TIME_BASE_NS,
      TimeStampPixelClock        = CORACQ_VAL_TIME_BASE_PIXEL_CLK,
      TimeStamp100NanoSec        = CORACQ_VAL_TIME_BASE_100NS
   };

   // Signal status
   typedef int SignalStatus;
   enum _SignalStatus
   {
      SignalNone               = 0,
      SignalHSyncPresent       = CORACQ_VAL_SIGNAL_HSYNC_PRESENT,
      SignalVSyncPresent       = CORACQ_VAL_SIGNAL_VSYNC_PRESENT,
      SignalPixelClkPresent    = CORACQ_VAL_SIGNAL_PIXEL_CLK_PRESENT,
      SignalChromaPresent      = CORACQ_VAL_SIGNAL_CHROMA_PRESENT,
      SignalHSyncLock          = CORACQ_VAL_SIGNAL_HSYNC_LOCK,
      SignalVSyncLock          = CORACQ_VAL_SIGNAL_VSYNC_LOCK,
      SignalPowerPresent       = CORACQ_VAL_SIGNAL_POWER_PRESENT,
      SignalPoCLActive         = CORACQ_VAL_SIGNAL_POCL_ACTIVE,
      SignalPoCLActive2        = CORACQ_VAL_SIGNAL_POCL_ACTIVE_2,
      SignalPixelClk1Present   = CORACQ_VAL_SIGNAL_PIXEL_CLK_1_PRESENT,
      SignalPixelClk2Present   = CORACQ_VAL_SIGNAL_PIXEL_CLK_2_PRESENT,
      SignalPixelClk3Present   = CORACQ_VAL_SIGNAL_PIXEL_CLK_3_PRESENT,
      SignalPixelClkAllPresent = CORACQ_VAL_SIGNAL_PIXEL_CLK_ALL_PRESENT,
      SignalPixelLinkLock      = CORACQ_VAL_SIGNAL_LINK_LOCK
   };

   // Video source scan types
   typedef int ScanType;
   enum _ScanType
   {
      ScanArea = CORACQ_VAL_SCAN_AREA,
      ScanLine = CORACQ_VAL_SCAN_LINE
   };

   // Video types
   enum VideoType
   {
      VideoMono  = CORACQ_VAL_VIDEO_MONO,
      VideoColor = CORACQ_VAL_VIDEO_COLOR,
      VideoYC    = CORACQ_VAL_VIDEO_YC,
      VideoRGB   = CORACQ_VAL_VIDEO_RGB,
      VideoBayer = CORACQ_VAL_VIDEO_BAYER
   };

   // Various constants
   enum MiscValues
   {
      MaxCameras = 32,
      MaxSerialPortNameSize = 64,
      MaxCustomParamSize = 4096
   };

public:
	// Constructor/Destructor
	SapAcquisition(SapLocation location = SapLocation::ServerSystem);
	SapAcquisition(SapLocation location, const char *configFile, EventType eventType = EventNone, SapAcqCallback pCallback = NULL, void *pContext = NULL);
	SapAcquisition(SapLocation location, const char *camfile, const char *vicfile, EventType eventType = EventNone, SapAcqCallback pCallback = NULL, void *pContext = NULL);
	SapAcquisition(const SapAcquisition &acq);
	SapAcquisition(const SapAcquisition &acq, EventType eventType, SapAcqCallback pCallback, void* pContext= NULL);
	virtual ~SapAcquisition();

	SapAcquisition &operator= (const SapAcquisition &acq);

	// Creation/destruction
	virtual BOOL Create();
	virtual BOOL Destroy();

public:
   // SapXferNode type information
   XferNodeType GetXferNodeType() { return NodeTypeAcquisition; }

	// Access to implementation
	const char *GetConfigFile()   const { return m_CamFile;}
	const char *GetCamFile()	   const { return m_CamFile; }
	const char *GetVicFile()	   const { return m_VicFile; }
	const char *GetLabel()        const { return m_Label;}
   EventType GetEventType()      const { return m_EventType; }
	SapAcqCallback	GetCallback()	const { return m_pCallback; }
	void				*GetContext()	const { return m_pContext; }
   FlipMode GetFlipMode()        const { return m_FlipMode; }
   int GetCamSel()               const { return m_CamSel; }

	virtual BOOL SetConfigFile( const char *configFile);
	virtual BOOL SetCamFile(const char *camfile);
	virtual BOOL SetVicFile(const char *vicfile);
	virtual BOOL SetCallbackInfo( SapAcqCallback pCallback, void *pContext= NULL);
   virtual BOOL SetEventType(EventType eventType);

#if COR_LINUX
	CORCAM GetCam();
	CORVIC GetVic();
#else
	CORCAM GetCam()	const { return m_hCam; }
	CORVIC GetVic()	const { return m_hVic; }
#endif

   virtual BOOL IsCapabilityValid(int cap);
   virtual BOOL IsParameterValid(int param);

   virtual BOOL GetCapability(int cap, void *pValue);
   virtual BOOL GetParameter(int param, void *pValue);
   virtual BOOL SetParameter(int param, int value, BOOL updateNow = TRUE);
   virtual BOOL SetParameter(int param, void *pValue, BOOL updateNow = TRUE);

	virtual BOOL SetFlipMode(FlipMode flipMode);
   virtual BOOL SetCamSel(int camSel);
   virtual BOOL SoftwareTrigger(SoftwareTriggerType triggerType);
	virtual BOOL SaveParameters(const char *camfile, const char *vicfile);
	virtual BOOL SaveParameters(const char *configFile);

   // Signal status management
   BOOL IsSignalStatusAvailable();
   BOOL IsSignalStatusAvailable(SignalStatus signalStatus);
   virtual BOOL GetSignalStatus(SignalStatus *pSignalStatus, SapAcqCallback pCallback = NULL, void *pContext = NULL);
   virtual BOOL GetSignalStatus(SignalStatus signalStatus, BOOL *pIsActive);

   // Look-up table management
   SapLut *GetLut(int lutIndex = 0);
	virtual BOOL ApplyLut(BOOL enable = TRUE);
	virtual BOOL ApplyLut(BOOL enable, int lutIndex);
   virtual BOOL EnableLut(BOOL enable = TRUE);
   BOOL CanEnableLut() { return m_LutEnableCap; }
   BOOL IsLutEnabled() { return m_LutEnable; }
   int GetNumLut() { return m_LutMax; }

   // Time stamp management
   BOOL IsTimeStampAvailable();
   TimeStampBase GetTimeStampBase() const { return m_TimeStampBase; }
   virtual BOOL SetTimeStampBase(TimeStampBase timeStampBase); 
   virtual BOOL ResetTimeStamp();

   // Planar input sources management
   int GetNumPlanarInputs() const { return m_NumPlanarInputs; }
   virtual BOOL GetPlanarInputs(BOOL *pCamEnable);
   virtual BOOL SetPlanarInputs(BOOL *pCamEnable, int numCameras);

   // Flat field correction management
   BOOL IsFlatFieldAvailable();

   // Bayer conversion management
   BOOL IsBayerAvailable();

   // Dynamic LUT management
   BOOL IsDynamicLutAvailable();

   // Serial port management
   BOOL GetSerialPortName(char *serialPortName);

   // Commands specific to each acquisition device
   BOOL CustomCommand(int command, void *inData, int inDataSize, void *outData, int outDataSize);

protected:
	// Utility methods
   virtual BOOL CreateRemaining();
   virtual BOOL DestroyLut();
   virtual void Construct(const char *camfile, const char *vicfile, EventType eventType, SapAcqCallback pCallback, void *pContext);
	virtual BOOL LoadConfig();
	virtual BOOL LoadCamVic();
   virtual BOOL SetCustomParams();
	virtual BOOL SetXferParams();
	virtual BOOL UpdateLutParams();
   virtual BOOL IsLutUpdateNeeded(int param);
   virtual BOOL GetParameter(int param, void *pValue, BOOL updateCamVic);
   virtual BOOL FixFilePath(const char *originalFilename, char *fullFilePath);
	virtual BOOL LoadLut(int lutIndex);
	static SAPSTATUS CCONV OnCallback(void *context, UINT32 eventType, UINT32 eventCount);

   // Only use DWORD_PTR starting with Visual Studio .NET 2003, since its presence depends on the
   // Platform SDK version with Visual Studio 6, and it is only needed for 64-bit compatibility anyway.
#if (defined(_MSC_VER) && _MSC_VER >= 1300) || defined(POSIX_HOSTPC)
   static void CALLBACK TimerProc(UINT timerId, UINT reserved1, DWORD_PTR userData, DWORD_PTR reserved2, DWORD_PTR reserved3);
#else
   static void CALLBACK TimerProc(UINT timerId, UINT reserved1, DWORD userData, DWORD reserved2, DWORD reserved3);
#endif

protected:
	// Parameters
	char m_CamFile[MAX_PATH];		         // CAM file name
	char m_VicFile[MAX_PATH];		         // VIC file name
	char m_Label[MaxLabelSize];            // Name of the acquisition resource
   EventType m_EventType;                 // Type of event for callback
	SapAcqCallback	m_pCallback;				// Application callback function
   void *m_pContext;                      // Application context

   // Look-up table management
   BOOL m_LutEnableCap;
   BOOL m_LutEnable;
   int m_LutEntries;
   SapFormat m_LutFormat;
   int m_LutMax;
   SapLut** m_AcqLut;

   // Pixel depth and shift for transfer parameters
   int m_PixelDepthWithLut;
   int m_PixelDepthNoLut;
   int m_PixelShiftWithLut;
   int m_PixelShiftNoLut;

   // Flipping mode management
   FlipMode m_FlipModeCap;
   FlipMode m_FlipMode;

   // Signal status management
   SignalStatus m_SignalStatusCap;     // Hardware capability
   SignalStatus m_SignalStatus;        // Current parameter value
	SapAcqCallback	m_pSignalCallback;   // Callback function for parameter value changes
   void *m_pSignalContext;             // Context information for signal status callback
   UINT m_SignalTimerId;               // Timer ID for periodic value change checking

   // Time stamp management
   TimeStampBase m_TimeStampBaseCap;
   TimeStampBase m_TimeStampBase;

   // Planar input sources management
   BOOL m_PlanarInputsCap;       // Hardware capability
   int m_PlanarInputsMask;       // Current parameter value
   int m_NumPlanarInputs;        // Number of cameras used simultaneously

   // Flat field correction management
   BOOL m_FlatFieldCap;          // Hardware capability

   // Bayer conversion management
   BOOL m_BayerCap;              // Hardware capability

   // Dynamic LUT management
   BOOL m_DynamicLutCap;         // Hardware capability

   // Management of custom parameters in configuration file
   char *m_CustomParamsBuf;
   int m_CustomParamsSize;

	// Others
	CORCAM	m_hCam;			         // Handle to CAM module
	CORVIC	m_hVic;			         // Handle to VIC module
   BOOL     m_bIsCamVicLoaded;      // TRUE if CAM/VIC files are loaded
   BOOL     m_bCallbackRegistered;  // TRUE if callback successfully registered
   int      m_NumPendingParams;     // Number of changed parameters no yet written to the acquisition hardware
   int      m_CamSel;               // Camera selector input
}; 

//
// SapAcqCallbackInfo class declaration
//
class SAPCLASSBASIC_CLASS SapAcqCallbackInfo
{
public:
   SapAcqCallbackInfo(SapAcquisition *pAcq, void *context, SapAcquisition::EventType eventType, int eventCount)
      { m_pAcq = pAcq; m_Context = context; m_EventType = eventType; m_EventCount = eventCount; m_SignalStatus = SapAcquisition::SignalNone; }
   SapAcqCallbackInfo(SapAcquisition *pAcq, void *context, SapAcquisition::SignalStatus signalStatus)
      { m_pAcq = pAcq; m_Context = context; m_EventType = SapAcquisition::EventNone; m_EventCount = 0; m_SignalStatus = signalStatus; }
	~SapAcqCallbackInfo() {}

   // Obsolete constructor
   SapAcqCallbackInfo(void *context, SapAcquisition::EventType eventType, int eventCount)
      { m_pAcq = NULL; m_Context = context; m_EventType = eventType; m_EventCount = eventCount; m_SignalStatus = SapAcquisition::SignalNone; }

   SapAcquisition *GetAcquisition() const { return m_pAcq; }
	void *GetContext() const { return m_Context; }
	SapAcquisition::EventType GetEventType() const { return m_EventType; }
	int GetEventCount() const { return m_EventCount; }
	SapAcquisition::SignalStatus GetSignalStatus() const { return m_SignalStatus; }

protected:
   SapAcquisition *m_pAcq;
	void *m_Context;
	SapAcquisition::EventType m_EventType;
	int m_EventCount;
   SapAcquisition::SignalStatus m_SignalStatus;
};

#endif	// _SAPACQUISITION_H_
