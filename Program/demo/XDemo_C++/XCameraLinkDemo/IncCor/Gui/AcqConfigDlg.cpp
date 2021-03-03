// AcqConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SapClassBasic.h"
#include "SapClassGui.h"
#include "BrowseDirDlg.h"
#include ".\acqconfigdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// Configuration parameters
//
static char *FileExt          = { ACQCFG_DEFAULT_EXTENSION};
static char *ConfigKeyName    = {"Camera Name"};
static char *CompanyKeyName   = {"Company Name"};
static char *ModelKeyName     = {"Model Name"};
static char *VicName          = {"Vic Name"};

/////////////////////////////////////////////////////////////////////////////
// CAcqConfigDlg dialog

CAcqConfigDlg::CAcqConfigDlg(CWnd* pParent, ServerCategory serverCategory)
   : CDialog(CAcqConfigDlg::IDD, pParent)
{
	m_App = AfxGetApp();
   m_ServerCategory = serverCategory;

	// Load parameters from registry
	LoadSettings();
}

CAcqConfigDlg::CAcqConfigDlg(CWnd* pParent, SapLocation loc, const char* configFile, ServerCategory serverCategory)
	: CDialog(CAcqConfigDlg::IDD, pParent)
{
	m_App = AfxGetApp();
   m_ServerCategory = serverCategory;

	// Initialize parameters
	char serverName[CORSERVER_MAX_STRLEN];
	SapManager::GetServerName(loc, serverName, sizeof(serverName));

	m_ServerName    = serverName;
	m_ResourceIndex = loc.GetResourceIndex();

   CorStrncpy( m_ConfigFile, configFile, sizeof(m_ConfigFile));
}

// Obsolete constructor, replaced by CAcqConfigDlg(CWnd*, SapLocation, const char*)
CAcqConfigDlg::CAcqConfigDlg(CWnd* pParent, SapAcquisition *pAcq, const char *productDir)
	: CDialog(CAcqConfigDlg::IDD, pParent)
{
	m_App = AfxGetApp();
   m_ProductDir = productDir;
   m_ServerCategory = ServerAcq;

	if (pAcq)
	{
      // Copy to internal object
      m_Acq = *pAcq;

		// Get parameters from acquisition object
		char serverName[CORSERVER_MAX_STRLEN];
		SapManager::GetServerName(pAcq->GetLocation(), serverName, sizeof(serverName));

		m_ServerName    = serverName;
		m_ResourceIndex = pAcq->GetLocation().GetResourceIndex();

      CorStrncpy( m_ConfigFile, pAcq->GetConfigFile(), sizeof(m_ConfigFile));
	}
	else
	{
		// Load parameters from registry
		LoadSettings();
	}
}

SapLocation CAcqConfigDlg::GetLocation()
{
   return SapLocation(m_ServerName, m_ResourceIndex);
}

BOOL CAcqConfigDlg::UpdateAcquisition()
{
	// Set new parameters
	SapLocation loc(m_ServerName, m_ResourceIndex);

	m_Acq.SetLocation(loc);
   m_Acq.SetConfigFile(m_ConfigFile);
	return TRUE;
}

void CAcqConfigDlg::LoadSettings() 
{
	// Read location (server and resource)
	m_ServerName    = m_App->GetProfileString("SapAcquisition", "Server", "");
	m_ResourceIndex = m_App->GetProfileInt("SapAcquisition", "Resource", 0);

	// Read config file name
	CString str;

	str = m_App->GetProfileString("SapAcquisition", "ConfigFile", "");
   CorStrncpy(m_ConfigFile, str, sizeof(m_ConfigFile));
}

void CAcqConfigDlg::SaveSettings()
{
	// Write location (server and resource)
	m_App->WriteProfileString("SapAcquisition", "Server", m_ServerName);
	m_App->WriteProfileInt("SapAcquisition", "Resource", m_ResourceIndex);

	// Write config file name
	m_App->WriteProfileString("SapAcquisition", "ConfigFile", m_ConfigFile);
}

void CAcqConfigDlg::SetDirectories()
{
	// Initialize directories
	m_currentConfigDir[0]     = '\0';
	m_currentConfigFileIndex  = -1;
	m_currentConfigFileName[0]= '\0';

	// Get installation directories
	char productDir[_MAX_PATH]= "";
	char saperaDir[_MAX_PATH]= "";
	
	GetEnvironmentVariable(m_ProductDir, productDir, sizeof(productDir));
	GetEnvironmentVariable("SAPERADIR", saperaDir, sizeof(saperaDir));

	CString pathName;

	char acDrive[_MAX_DRIVE];
	char acDir[_MAX_DIR];
	char acFname[_MAX_FNAME];
	char acExt[_MAX_EXT];

	// Get last CAM file accessed
   if (strlen(m_ConfigFile))
	{
      CorSplitpath(m_ConfigFile, acDrive, _MAX_DRIVE, acDir, _MAX_DIR, acFname, _MAX_FNAME, acExt, _MAX_EXT);
		CorSnprintf(m_currentConfigDir, sizeof(m_currentConfigDir), "%s%s", acDrive, acDir);
		CorSnprintf(m_currentConfigFileName, sizeof(m_currentConfigFileName), "%s%s", acFname, acExt);
	}
	else
	{
      if (strlen(productDir))
		{
         CorStrncpy(m_currentConfigDir, productDir, sizeof(m_currentConfigDir));
         CorStrncat(m_currentConfigDir, ACQCFG_DEFAULT_FOLDER, sizeof(m_currentConfigDir) - strlen(m_currentConfigDir));
		}
		else if (strlen(saperaDir))
		{
         CorStrncpy(m_currentConfigDir, saperaDir, sizeof(m_currentConfigDir));
         CorStrncat(m_currentConfigDir, ACQCFG_DEFAULT_FOLDER, sizeof(m_currentConfigDir) - strlen(m_currentConfigDir));
		}

		else
		{
			//GetCurrentDirectory( sizeof(m_currentConfigDir), m_currentConfigDir);
		}
	}

	// Remove trailing backslash
	if (m_currentConfigDir[strlen(m_currentConfigDir) - 1] == '\\')
		m_currentConfigDir[strlen(m_currentConfigDir) - 1] = '\0';
}

void CAcqConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAcqConfigDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_SCG_CV_COMBO_RESOURCE, m_cbResource);
	DDX_Control(pDX, IDC_SCG_CV_COMBO_SERVER, m_cbServer);
	DDX_Control(pDX, IDC_SCG_CV_COMBO_CONFIG, m_cbConfig);
	DDX_Control(pDX, IDC_SCG_CV_CONFIG_LIST_FILENAME, m_configListFileName);
	DDX_CBString(pDX, IDC_SCG_CV_COMBO_SERVER, m_ServerName);
	DDX_CBIndex(pDX, IDC_SCG_CV_COMBO_RESOURCE, m_ResourceIndex);
	DDX_Check(pDX, IDC_SCG_CV_CONFIG_FILE_CHECK, m_ConfigFileEnable);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAcqConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CAcqConfigDlg)
	ON_BN_CLICKED(IDC_SCG_CV_CONFIG_BROWSE, OnConfigBrowse)
	ON_EN_KILLFOCUS(IDC_SCG_CV_EDIT_CONFIG_PATH, OnKillfocusEditConfigPath)
	ON_CBN_SELCHANGE(IDC_SCG_CV_COMBO_CONFIG, OnSelchangeComboConfig)
	ON_CBN_SELCHANGE(IDC_SCG_CV_COMBO_SERVER, OnSelchangeComboServer)
	ON_CBN_SELCHANGE(IDC_SCG_CV_COMBO_RESOURCE, OnSelchangeComboResource)
	//}}AFX_MSG_MAP
   ON_BN_CLICKED(IDC_SCG_CV_CONFIG_FILE_CHECK, OnBnClickedConfigFileCheck)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAcqConfigDlg message handlers

BOOL CAcqConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Initialize location combo boxes
	InitServerCombo();

	// Initialize directories
	SetDirectories();

	// Display the current directory to look for the files
	SetDlgItemText(IDC_SCG_CV_EDIT_CONFIG_PATH, m_currentConfigDir);

	// Scan all files in the current directory and fill the list box
	UpdateNames();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAcqConfigDlg::InitServerCombo()
{
	for (int i=0; i < GetServerCount(); i++)
	{
		// Does this server support "Acq" (frame-grabber) or "AcqDevice" (camera)?
      bool bAcq = (m_ServerCategory == ServerAcq || m_ServerCategory == ServerAll) 
                  && (GetResourceCount(i, ResourceAcq) > 0);
      bool bAcqDevice = (m_ServerCategory == ServerAcqDevice || m_ServerCategory == ServerAll) 
                        && (GetResourceCount(i, ResourceAcqDevice) > 0);
		if (bAcq || bAcqDevice)
		{
			char name[128];
			if (GetServerName(i, name, sizeof(name)))
         {
            if (strstr(name,"CameraLink_") > 0)
            {
               if(bAcq)
                  m_cbServer.AddString(name);
            }
            else
            {
               // Add server name to combo
				   m_cbServer.AddString(name);

               // Store flag to tell whether or not it's a "AcqDevice" server
               // Only use DWORD_PTR starting with Visual Studio .NET 2003, since its presence depends on the
               // Platform SDK version with Visual Studio 6, and it is only needed for 64-bit compatibility anyway.
#if defined(_MSC_VER) && _MSC_VER >= 1300
               m_cbServer.SetItemData(m_cbServer.GetCount()-1, (DWORD_PTR) bAcq);
#else
               m_cbServer.SetItemData(m_cbServer.GetCount()-1, bAcq);
#endif
            }
         }
		}
	}

	// At least one server must be available
	if (m_cbServer.GetCount() <= 0)
	{
		OnCancel();
		return;
	}

	// Select server name
	if (m_cbServer.SelectString(-1, m_ServerName) == CB_ERR)
	{
		// If not found, select first one
		m_cbServer.SetCurSel(0);
		m_cbServer.GetLBText(0, m_ServerName);
	}

	// Initialize resource combo box
	InitResourceCombo();
}
	
void CAcqConfigDlg::InitResourceCombo()
{
	int i=0;
	// Clear combo content
	m_cbResource.ResetContent();
	// Add "Acq" resources (frame-grabbers) to combo
	for (i=0; i < GetResourceCount(m_ServerName, ResourceAcq); i++)
	{
		char name[128];
      if (GetResourceName(m_ServerName, ResourceAcq, i, name, sizeof(name)))
         if (IsResourceAvailable(m_ServerName, ResourceAcq, i) == FALSE)
            CorSnprintf(name,sizeof(name),"%s","Not Available - Resource In Use");
      m_cbResource.AddString(name);
	}

	// Add "AcqDevice" resources (cameras) to combo
	for (i=0; i < GetResourceCount(m_ServerName, ResourceAcqDevice); i++)
	{
		char name[128];

// Nov 19th 2013
// This code is required to avoid displaying resources without streaming ports (as seen on the Xcelera CLHS).
// It was removed to correct a crash seen with the new GigE driver. (RT #24455)
#if 0
      char server[128];
      CorStrncpy(server,m_ServerName,sizeof(server));
      SapAcqDevice camera(server);
		BOOL status = camera.Create();
		UINT32 nPort = 1; //default to 1 streaming port
      if (status)
         CorAcqDeviceGetPrm(camera.GetHandle(),CORACQDEVICE_PRM_NUM_PORTS,&nPort);
		// Destroy acquisition device object
		if (!camera.Destroy()) 
         continue;
	   if (nPort == 0)
      {
		   continue; //skip this AcqDevice since it doesn't have a video streaming port.
      }
#endif
		if (GetResourceName(m_ServerName, ResourceAcqDevice, i, name, sizeof(name)))
			m_cbResource.AddString(name);
	}

	// Select resource index
	if (m_cbResource.SetCurSel(m_ResourceIndex) == CB_ERR)
	{
		// If not found, select first one
		m_ResourceIndex = 0;
		m_cbResource.SetCurSel(m_ResourceIndex);
	}
}

//
// Browse for a directory where to look for the files
//
void CAcqConfigDlg::OnConfigBrowse() 
{
	CBrowseDirDlg dlg(m_currentConfigDir);
	if (dlg.DoModal() == IDOK)
	{
		UpdateCurrentDir(dlg.GetDir().GetBuffer(0));
		UpdateNames();
	}
}

void CAcqConfigDlg::OnKillfocusEditConfigPath() 
{
	char newCurrentDir[MAX_PATH];

	// Get new directory name
	GetDlgItemText(IDC_SCG_CV_EDIT_CONFIG_PATH, newCurrentDir, sizeof(newCurrentDir));

	// Update the dialog box
	UpdateCurrentDir(newCurrentDir);
	UpdateNames();	
}

//
// Update the current config directory in the dialog box, and if necessary reset some member variables
// to default values.
//
void CAcqConfigDlg::UpdateCurrentDir(char *newCurrentDir)
{
	// Remove trailing backslash
   if (newCurrentDir[strlen( newCurrentDir) - 1] == '\\')
      newCurrentDir[strlen( newCurrentDir) - 1] = '\0';

	char *currentDir			= m_currentConfigDir;
	char *currentFileName	= m_currentConfigFileName;
	UINT ctrlId					= IDC_SCG_CV_EDIT_CONFIG_PATH;

   if (CorStricmp( newCurrentDir, currentDir))
	{
		// New Directory selected, update the list box selection
      CorStrncpy(currentDir, newCurrentDir, sizeof(m_currentConfigDir));
		SetDlgItemText(ctrlId, newCurrentDir);
		currentFileName[0] = '\0';
	}
}

//
// Update the design list box
//
void CAcqConfigDlg::UpdateNames()
{
	char *serverName  = new char[CORSERVER_MAX_STRLEN];
	char *pathName    = new char[_MAX_PATH];
	char *fileName    = new char[_MAX_PATH];
	char *cameraName  = new char[_MAX_PATH];
	char *vicName     = new char[_MAX_PATH];
	char *companyName = new char[_MAX_PATH];
	char *modelName   = new char[_MAX_PATH];
	char *cameraDesc  = new char[_MAX_PATH * 2];

   memset(serverName, 0, CORSERVER_MAX_STRLEN);
   memset(pathName, 0, _MAX_PATH);
   memset(fileName, 0, _MAX_PATH);
   memset(cameraName, 0, _MAX_PATH);
   memset(vicName, 0, _MAX_PATH);
   memset(companyName, 0, _MAX_PATH);
   memset(modelName, 0, _MAX_PATH);
   memset(cameraDesc, 0, _MAX_PATH * 2);

	int index = 0;
	int fileIndex = 0;
   int curDescLength = 0;
   int maxDescLength = 0;

	CComboBox *combo			= &m_cbConfig;
	CListBox	*listFileName	= &m_configListFileName;
	char *currentDir			= m_currentConfigDir;
	char *currentFileName	= m_currentConfigFileName;
	int *currentFileIndex	= &m_currentConfigFileIndex;
	char *fileExt				= FileExt;
	char *keyName				= ConfigKeyName;
	BOOL *fileAvailable	   = &m_configFileAvailable;

	// fill listbox (hidden) with the design file names
	CorSnprintf(pathName, _MAX_PATH, "%s\\*.%s", currentDir, fileExt);

	listFileName->ResetContent();
	listFileName->Dir( 0, pathName);

	// Clear all entries
	combo->ResetContent();

	// Get current server name
	CString curServerName= m_ServerName;

	// Remove trailing board index from the server name
	char* pTmp= strrchr( curServerName.GetBuffer( CORSERVER_MAX_STRLEN), '_');
	if( pTmp != 0) *pTmp= '\0';

	curServerName.ReleaseBuffer();

	for (fileIndex = 0; fileIndex < listFileName->GetCount(); fileIndex++)
	{
		// For every file, create an entry in the combo box with the name of the camera
		listFileName->GetText( fileIndex, fileName);
		
		CorSnprintf( pathName, _MAX_PATH, "%s\\%s", currentDir, fileName);

		GetPrivateProfileString( "Board", "Server name", "Unknow", serverName, CORSERVER_MAX_STRLEN, pathName);

		// Check if the current configuration file has been created for the current server 
		if( curServerName.CompareNoCase( serverName) != 0)
			continue;

		GetPrivateProfileString( "General", keyName, "Unknown", cameraName, _MAX_PATH, pathName);
		GetPrivateProfileString( "General", CompanyKeyName, "", companyName, _MAX_PATH, pathName);
		GetPrivateProfileString( "General", ModelKeyName, "", modelName, _MAX_PATH, pathName);
		GetPrivateProfileString( "General", VicName, "", vicName, _MAX_PATH, pathName);

      if( strlen( companyName) && strlen( modelName))
		{
         CorStrncat( companyName, ", ", _MAX_PATH - strlen(companyName));
		}

      if( strlen( modelName) && strlen( cameraName))
		{
         CorStrncat( modelName, ", ", _MAX_PATH - strlen(modelName));
		}

		CorSnprintf( cameraDesc, _MAX_PATH * 2, "%s%s%s - %s", companyName, modelName, cameraName, vicName);

		index = combo->AddString( cameraDesc);
		combo->SetItemData( index, fileIndex);

      // Make drop-down list wide enough to accomodate long entries.
      // First find out length of current string.
      curDescLength = (int)strlen(cameraDesc);

      // If this length is longer than the current maximum, find the new combo
      // box list width in pixels as follows:
      //    - get the device context for the combo box
      //    - get the font for the combo box
      //    - select this font into the combo box device context
      //    - the total width in pixels is equal to:
      //          the string width in pixels +
      //          the average character width for the font +
      //          the vertical scroll bar width +
      //          the left and right border.
      //          (this avoids any clipping at the end of the string)
      //    - select the original font back into the combo box device context
      if (curDescLength > maxDescLength)
      {
         SIZE strSize;
         TEXTMETRIC tm;
         CDC *pDC = combo->GetDC();
         CFont *pComboFont = combo->GetFont();
         CFont *pOldFont = pDC->SelectObject(pComboFont);

         if (pDC->GetTextMetrics(&tm) && GetTextExtentPoint32(pDC->m_hDC, cameraDesc, curDescLength, &strSize))
         {
            int width = strSize.cx;
            width += tm.tmAveCharWidth;
            width += GetSystemMetrics(SM_CXVSCROLL);
            width += 2 * GetSystemMetrics(SM_CXEDGE);
            combo->SetDroppedWidth(width);
            maxDescLength = curDescLength;
         }

         pDC->SelectObject(pOldFont);
      }
	}

	if (combo->GetCount())
	{
		int newFileIndex = -1;

      if (strlen(currentFileName))
		{
			// Try to find the current camera file selected
			for (index = 0; index < combo->GetCount(); index++)
			{
            fileIndex = (int)combo->GetItemData( index);
				listFileName->GetText( fileIndex, fileName);
	
            if (!CorStricmp( fileName, currentFileName))
				{
					newFileIndex = index;
					break;
				}
			}
		}

		if (newFileIndex == -1) newFileIndex = 0;
	   combo->SetCurSel( newFileIndex);

      *currentFileIndex = (int)combo->GetItemData( newFileIndex);
		listFileName->GetText(*currentFileIndex, currentFileName);
		*fileAvailable = TRUE;
	}
	else
	{
		*currentFileIndex = -1;
		*fileAvailable = FALSE;
	}

	UpdateBoxAvailability();

	delete [] serverName;
	delete [] pathName;
	delete [] fileName;
	delete [] cameraName;
	delete [] companyName;
	delete [] modelName;
	delete [] cameraDesc;
	delete [] vicName;
}

void CAcqConfigDlg::UpdateBoxAvailability()
{
   // Is config file is required by this type of server?
   BOOL configFileRequired = m_cbServer.GetItemData(m_cbServer.GetCurSel()) ? true : false;
   GetDlgItem(IDC_SCG_CV_CONFIG_FILE_CHECK)->EnableWindow(!configFileRequired);
   if (configFileRequired)
   {
      // Force loading a config file
      m_ConfigFileEnable = configFileRequired;
      UpdateData(FALSE);
   }

	GetDlgItem(IDC_SCG_CV_COMBO_CONFIG)->EnableWindow(m_ConfigFileEnable && m_configFileAvailable);
	GetDlgItem(IDC_SCG_CV_EDIT_CONFIG_PATH)->EnableWindow(m_ConfigFileEnable);
	GetDlgItem(IDC_SCG_CV_CONFIG_BROWSE)->EnableWindow(m_ConfigFileEnable);

	GetDlgItem(IDOK)->EnableWindow(!m_ConfigFileEnable || m_configFileAvailable);
}

void CAcqConfigDlg::OnSelchangeComboConfig() 
{
	int index;			// List box selection
	int newFileIndex;	// File selection 

	// Get new selection
	index = m_cbConfig.GetCurSel();
   newFileIndex = (int)m_cbConfig.GetItemData( index);

	// Check if selection is different than current one
	if( newFileIndex != m_currentConfigFileIndex)
	{
		m_currentConfigFileIndex = newFileIndex;
		m_configListFileName.GetText( m_currentConfigFileIndex, m_currentConfigFileName);
	}
}

void CAcqConfigDlg::OnOK() 
{
	CDialog::OnOK();

	// Create complete strings
	CorSnprintf(m_ConfigFile, sizeof(m_ConfigFile), "%s\\%s", m_currentConfigDir, m_currentConfigFileName);

	// Update acquisition object
	UpdateAcquisition();
	
	// Save to registry
	SaveSettings();

   // Update server category if necessary
   if (m_ServerCategory == ServerAll)
   {
      if (GetResourceCount(m_Acq.GetLocation(), ResourceAcq) > 0)
         m_ServerCategory = ServerAcq;
      else if (GetResourceCount(m_Acq.GetLocation(), ResourceAcqDevice) > 0)
         m_ServerCategory = ServerAcqDevice;
   }
}

void CAcqConfigDlg::OnSelchangeComboServer() 
{
	UpdateData(TRUE);
	InitResourceCombo();
	UpdateNames();
	UpdateBoxAvailability();
}

void CAcqConfigDlg::OnSelchangeComboResource() 
{
	UpdateData(TRUE);
	UpdateNames();
}

void CAcqConfigDlg::OnBnClickedConfigFileCheck()
{
	UpdateData(TRUE);
	UpdateBoxAvailability();
}
