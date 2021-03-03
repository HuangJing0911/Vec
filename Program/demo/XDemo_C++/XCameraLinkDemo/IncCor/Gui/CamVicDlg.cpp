// CamVicDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SapClassBasic.h"
#include "SapClassGui.h"
#include "BrowseDirDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//
// Configuration parameters
//
static char *FileExt[2] = {"cca", "cvi"};
static char *CameraKeyName[2] = {"Camera Name", "Vic Name"};
static char *CompanyKeyName = {"Company Name"};
static char *ModelKeyName = {"Model Name"};

/////////////////////////////////////////////////////////////////////////////
// CCamVicDlg dialog


CCamVicDlg::CCamVicDlg(CWnd* pParent, SapAcquisition *pAcq, const char *productDir)
	: CDialog(CCamVicDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCamVicDlg)
	m_ServerName = "";
	m_ResourceIndex = 0;
	//}}AFX_DATA_INIT

	m_App = AfxGetApp();
   m_ProductDir = productDir;

	if (pAcq)
	{
      // Copy to internal object
      m_Acq = *pAcq;

		// Get parameters from acquisition object
		char serverName[CORSERVER_MAX_STRLEN];
		SapManager::GetServerName(pAcq->GetLocation(), serverName, sizeof(serverName));
		m_ServerName = serverName;
		m_ResourceIndex = pAcq->GetLocation().GetResourceIndex();
      CorStrncpy(m_CamFile, pAcq->GetCamFile(), sizeof(m_CamFile));
      CorStrncpy(m_VicFile, pAcq->GetVicFile(), sizeof(m_VicFile));
	}
	else
	{
		// Load parameters from registry
		LoadSettings();
	}
}

BOOL CCamVicDlg::UpdateAcquisition()
{
	// Set new parameters
	SapLocation loc(m_ServerName, m_ResourceIndex);
	m_Acq.SetLocation(loc);
   m_Acq.SetCamFile(m_CamFile);
   m_Acq.SetVicFile(m_VicFile);
	return TRUE;
}

void CCamVicDlg::LoadSettings() 
{
	// Read location (server and resource)
	m_ServerName = m_App->GetProfileString("SapAcquisition", "Server", "");
	m_ResourceIndex = m_App->GetProfileInt("SapAcquisition", "Resource", 0);

	// Read CAM and VIC file names
	CString str;
	str = m_App->GetProfileString("SapAcquisition", "CamFile", "");
   CorStrncpy(m_CamFile, str, sizeof(m_CamFile));
	str = m_App->GetProfileString("SapAcquisition", "VicFile", "");
   CorStrncpy(m_VicFile, str, sizeof(m_VicFile));
}

void CCamVicDlg::SaveSettings()
{
	// Write location (server and resource)
	m_App->WriteProfileString("SapAcquisition", "Server", m_ServerName);
	m_App->WriteProfileInt("SapAcquisition", "Resource", m_ResourceIndex);

	// Write CAM and VIC file names
	m_App->WriteProfileString("SapAcquisition", "CamFile", m_CamFile);
	m_App->WriteProfileString("SapAcquisition", "VicFile", m_VicFile);
}

void CCamVicDlg::SetDirectories()
{
	// Initialize directories
	m_currentCamDir[0] = m_currentVicDir[0] = '\0';
	m_currentCamFileIndex = m_currentVicFileIndex = -1;
	m_currentCamFileName[0] = m_currentVicFileName[0] = '\0';

	// Get installation directories
	char productDir[_MAX_PATH], saperaDir[_MAX_PATH];
   CorStrncpy(productDir, "", sizeof(productDir));
	GetEnvironmentVariable(m_ProductDir, productDir, sizeof(productDir));
   CorStrncpy(saperaDir, "", sizeof(saperaDir));
	GetEnvironmentVariable("SAPERADIR", saperaDir, sizeof(saperaDir));

	CString pathName;
	char acDrive[_MAX_DRIVE];
	char acDir[_MAX_DIR];
	char acFname[_MAX_FNAME];
	char acExt[_MAX_EXT];

	// Get last CAM file accessed
   if (strlen(m_CamFile))
	{
      CorSplitpath(m_CamFile, acDrive, _MAX_DRIVE, acDir, _MAX_DIR, acFname, _MAX_FNAME, acExt, _MAX_EXT);
		CorSnprintf(m_currentCamDir, sizeof(m_currentCamDir), "%s%s", acDrive, acDir);
		CorSnprintf(m_currentCamFileName, sizeof(m_currentCamFileName), "%s%s", acFname, acExt);
	}
	else
	{
      if (strlen(productDir))
		{
         CorStrncpy(m_currentCamDir, productDir, sizeof(m_currentCamDir));
         CorStrncat(m_currentCamDir, "\\CamFiles", sizeof(m_currentCamDir) - strlen(m_currentCamDir));
		}
		else
		{
			GetCurrentDirectory(sizeof(m_currentCamDir), m_currentCamDir);
		}
	}

	// Get last VIC file accessed
   if (strlen(m_VicFile))
	{
      CorSplitpath(m_VicFile, acDrive, _MAX_DRIVE, acDir, _MAX_DIR, acFname, _MAX_FNAME, acExt, _MAX_EXT);
		CorSnprintf(m_currentVicDir, sizeof(m_currentVicDir), "%s%s", acDrive, acDir);
		CorSnprintf(m_currentVicFileName, sizeof(m_currentVicFileName), "%s%s", acFname, acExt);
	}
	else
	{
      if (strlen(productDir))
		{
         CorStrncpy(m_currentVicDir, productDir, sizeof(m_currentVicDir));
         CorStrncat(m_currentVicDir, "\\CamFiles", sizeof(m_currentVicDir) - strlen(m_currentVicDir));
		}
		else
		{
			GetCurrentDirectory(sizeof(m_currentVicDir), m_currentVicDir);
		}
	}

	// Remove trailing backslash
	if (m_currentCamDir[strlen(m_currentCamDir) - 1] == '\\')
		m_currentCamDir[strlen(m_currentCamDir) - 1] = '\0';

	if (m_currentVicDir[strlen(m_currentVicDir) - 1] == '\\')
		m_currentVicDir[strlen(m_currentVicDir) - 1] = '\0';

	// Get MakeVic utility filename
   if (strlen(saperaDir))
	{
      CorStrncpy(m_makeVicFile, saperaDir, sizeof(m_makeVicFile));
      CorStrncat(m_makeVicFile, "\\Bin\\MakeVic.exe", sizeof(m_makeVicFile) - strlen(m_makeVicFile));
	}
	else
	{
      CorStrncpy(m_makeVicFile, "", sizeof(m_makeVicFile));
	}
}

void CCamVicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCamVicDlg)
	DDX_Control(pDX, IDC_SCG_CV_COMBO_RESOURCE, m_cbResource);
	DDX_Control(pDX, IDC_SCG_CV_COMBO_SERVER, m_cbServer);
	DDX_Control(pDX, IDC_SCG_CV_COMBO_VIC, m_cbVic);
	DDX_Control(pDX, IDC_SCG_CV_COMBO_CAMERA, m_cbCamera);
	DDX_Control(pDX, IDC_SCG_CV_VIC_LIST_FILENAME, m_vicListFileName);
	DDX_Control(pDX, IDC_SCG_CV_CAM_LIST_FILENAME, m_camListFileName);
	DDX_CBString(pDX, IDC_SCG_CV_COMBO_SERVER, m_ServerName);
	DDX_CBIndex(pDX, IDC_SCG_CV_COMBO_RESOURCE, m_ResourceIndex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCamVicDlg, CDialog)
	//{{AFX_MSG_MAP(CCamVicDlg)
	ON_BN_CLICKED(IDC_SCG_CV_CAM_BROWSE, OnCamBrowse)
	ON_BN_CLICKED(IDC_SCG_CV_VIC_BROWSE, OnVicBrowse)
	ON_EN_KILLFOCUS(IDC_SCG_CV_EDIT_CAM_PATH, OnKillfocusEditCamPath)
	ON_EN_KILLFOCUS(IDC_SCG_CV_EDIT_VIC_PATH, OnKillfocusEditVicPath)
	ON_CBN_SELCHANGE(IDC_SCG_CV_COMBO_CAMERA, OnSelchangeComboCamera)
	ON_CBN_SELCHANGE(IDC_SCG_CV_COMBO_VIC, OnSelchangeComboVic)
	ON_BN_CLICKED(IDC_SCG_CV_VIC_GENERATE, OnGenerateVic)
	ON_CBN_SELCHANGE(IDC_SCG_CV_COMBO_SERVER, OnSelchangeComboServer)
	ON_CBN_SELCHANGE(IDC_SCG_CV_COMBO_RESOURCE, OnSelchangeComboResource)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCamVicDlg message handlers

BOOL CCamVicDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Initialize location combo boxes
	InitServerCombo();

	// Initialize directories
	SetDirectories();

	// Display the current directory to look for the files
	SetDlgItemText(IDC_SCG_CV_EDIT_CAM_PATH, m_currentCamDir);
	SetDlgItemText(IDC_SCG_CV_EDIT_VIC_PATH, m_currentVicDir);

	// Scan all files in the current directory and fill the list box
	UpdateNames(TRUE);
	UpdateNames(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCamVicDlg::InitServerCombo()
{
	for (int i=0; i < GetServerCount(); i++)
	{
		// Does this server support acquisition ?
		if (GetResourceCount(i, ResourceAcq))
		{
			// Add server name to combo
			char name[128];
			if (GetServerName(i, name, sizeof(name)))
				m_cbServer.AddString(name);
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
	
void CCamVicDlg::InitResourceCombo()
{
	// Clear combo context
	m_cbResource.ResetContent();

	// Fill resource combo
	for (int i=0; i < GetResourceCount(m_ServerName, ResourceAcq); i++)
	{
		char name[128];
		if (GetResourceName(m_ServerName, ResourceAcq, i, name, sizeof(name)))
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
void CCamVicDlg::OnCamBrowse() 
{
	CBrowseDirDlg dlg(m_currentCamDir);
	if (dlg.DoModal() == IDOK)
	{
		UpdateCurrentDir(dlg.GetDir().GetBuffer(0), TRUE);
		UpdateNames(TRUE);
	}
}

void CCamVicDlg::OnVicBrowse() 
{
	CBrowseDirDlg dlg(m_currentVicDir);
	if (dlg.DoModal() == IDOK)
	{
		UpdateCurrentDir(dlg.GetDir().GetBuffer(0), FALSE);
		UpdateNames(FALSE);
	}
}

void CCamVicDlg::OnKillfocusEditCamPath() 
{
	char newCurrentDir[MAX_PATH];

	// Get new directory name
	GetDlgItemText(IDC_SCG_CV_EDIT_CAM_PATH, newCurrentDir, sizeof(newCurrentDir));

	// Update the dialog box
	UpdateCurrentDir(newCurrentDir, TRUE);
	UpdateNames(TRUE);	
}

void CCamVicDlg::OnKillfocusEditVicPath() 
{
	char newCurrentDir[MAX_PATH];

	// Get new directory name
	GetDlgItemText(IDC_SCG_CV_EDIT_VIC_PATH, newCurrentDir, sizeof(newCurrentDir));

	// Update the dialog box
	UpdateCurrentDir(newCurrentDir, FALSE);
	UpdateNames(FALSE);	
}

//
// Update the current CCA or CVI directory in the dialog box, and if necessary reset some member variables
// to default values.
//
void CCamVicDlg::UpdateCurrentDir(char *newCurrentDir, BOOL bCamFile)
{
	// Remove trailing backslash
	if (newCurrentDir[strlen( newCurrentDir) - 1] == '\\')
		newCurrentDir[strlen( newCurrentDir) - 1] = '\0';

	char *currentDir			= bCamFile ? m_currentCamDir : m_currentVicDir;
	char *currentFileName	= bCamFile ? m_currentCamFileName : m_currentVicFileName;
	UINT ctrlId					= bCamFile ? IDC_SCG_CV_EDIT_CAM_PATH : IDC_SCG_CV_EDIT_VIC_PATH;

   if (CorStricmp(newCurrentDir, currentDir))
	{
		// New Directory selected, update the list box selection
      CorStrncpy(currentDir, newCurrentDir, bCamFile ? sizeof(m_currentCamDir) : sizeof(m_currentVicDir));
		SetDlgItemText(ctrlId, newCurrentDir);
		currentFileName[0] = '\0';
	}
}

//
// Update the design list box
//
void CCamVicDlg::UpdateNames(BOOL bCamFile)
{
	char *pathName = new char[_MAX_PATH];
	char *fileName = new char[_MAX_PATH];
	char *cameraName = new char[_MAX_PATH];
	char *companyName = new char[_MAX_PATH];
	char *modelName = new char[_MAX_PATH];
	char *cameraDesc = new char[_MAX_PATH * 2];

   memset(pathName, 0, _MAX_PATH);
   memset(fileName, 0, _MAX_PATH);
   memset(cameraName, 0, _MAX_PATH);
   memset(companyName, 0, _MAX_PATH);
   memset(modelName, 0, _MAX_PATH);
   memset(cameraDesc, 0, _MAX_PATH * 2);

	int index = 0;
	int fileIndex = 0;
   int curDescLength = 0;
   int maxDescLength = 0;

	CComboBox *combo			= bCamFile ? &m_cbCamera : &m_cbVic;
	CListBox	*listFileName	= bCamFile ? &m_camListFileName : &m_vicListFileName;
	char *currentDir			= bCamFile ? m_currentCamDir : m_currentVicDir;
	char *currentFileName	= bCamFile ? m_currentCamFileName : m_currentVicFileName;
	int *currentFileIndex	= bCamFile ? &m_currentCamFileIndex : &m_currentVicFileIndex;
	char *fileExt				= bCamFile ? FileExt[0] : FileExt[1];
	char *keyName				= bCamFile ? CameraKeyName[0] : CameraKeyName[1];
	BOOL *fileAvailable	= bCamFile ? &m_camFileAvailable : &m_vicFileAvailable;

	// fill listbox (hidden) with the design file names
	CorSnprintf(pathName, _MAX_PATH, "%s\\*.%s", currentDir, fileExt);

	listFileName->ResetContent();
	listFileName->Dir( 0, pathName);

	// Clear all entries
	combo->ResetContent();

	for (fileIndex = 0; fileIndex < listFileName->GetCount(); fileIndex++)
	{
		// For every file, create an entry in the combo box with the name of the camera
		listFileName->GetText( fileIndex, fileName);
		
		CorSnprintf( pathName, _MAX_PATH, "%s\\%s", currentDir, fileName);

		GetPrivateProfileString("General", keyName, "Unknown", cameraName, _MAX_PATH, pathName);

		if( bCamFile)
		{
			GetPrivateProfileString( "General", CompanyKeyName, "", companyName, _MAX_PATH, pathName);
			GetPrivateProfileString( "General", ModelKeyName, "", modelName, _MAX_PATH, pathName);

         if( strlen( companyName) && strlen( modelName))
			{
            CorStrncat( companyName, ", ", _MAX_PATH - strlen(companyName));
			}

         if( strlen( modelName) && strlen( cameraName))
			{
            CorStrncat( modelName, ", ", _MAX_PATH - strlen(modelName));
			}
		}
		else
		{
         CorStrncpy( companyName, "", _MAX_PATH);
         CorStrncpy( modelName, "", _MAX_PATH);
		}

		CorSnprintf( cameraDesc, _MAX_PATH * 2, "%s%s%s", companyName, modelName, cameraName);

		index = combo->AddString( cameraDesc);
		combo->SetItemData( index, fileIndex);

      // Make drop-down list wide enough to accomodate long entries.
      // Assume default system font width of 8 pixels per character.
      curDescLength = (int)strlen(cameraDesc);
      if (curDescLength > maxDescLength)
      {
         maxDescLength = curDescLength;
	      combo->SetDroppedWidth(maxDescLength * 8);
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

	delete [] pathName;
	delete [] fileName;
	delete [] cameraName;
	delete [] companyName;
	delete [] modelName;
	delete [] cameraDesc;
}

void CCamVicDlg::UpdateBoxAvailability()
{
	GetDlgItem(IDC_SCG_CV_COMBO_CAMERA)->EnableWindow(m_camFileAvailable);
	GetDlgItem(IDC_SCG_CV_COMBO_VIC)->EnableWindow(m_vicFileAvailable);
	GetDlgItem(IDC_SCG_CV_VIC_GENERATE)->EnableWindow(MakeVicAvailable());
	GetDlgItem(IDOK)->EnableWindow(m_camFileAvailable && m_vicFileAvailable);
}

void CCamVicDlg::OnSelchangeComboCamera() 
{
	int index;			// List box selection
	int newFileIndex;	// File selection 

	// Get new selection
	index = m_cbCamera.GetCurSel();
   newFileIndex = (int)m_cbCamera.GetItemData( index);

	// Check if selection is different than current one
	if( newFileIndex != m_currentCamFileIndex)
	{
		m_currentCamFileIndex = newFileIndex;
		m_camListFileName.GetText( m_currentCamFileIndex, m_currentCamFileName);
	}
}

void CCamVicDlg::OnSelchangeComboVic() 
{
	int index;			// List box selection
	int newFileIndex;	// File selection 

	// Get new selection
	index = m_cbVic.GetCurSel();
   newFileIndex = (int)m_cbVic.GetItemData( index);

	// Check if selection is different than current one
	if( newFileIndex != m_currentVicFileIndex)
	{
		m_currentVicFileIndex = newFileIndex;
		m_vicListFileName.GetText( m_currentVicFileIndex, m_currentVicFileName);
	}
}

BOOL CCamVicDlg::MakeVicAvailable() 
{
   // Check for presence of the MakeVic utility
	OFSTRUCT ofStruct;
	if (OpenFile(m_makeVicFile, &ofStruct, OF_EXIST) == HFILE_ERROR)
      return FALSE;

   // Check that acquisition resource is available
   return !m_ServerName.IsEmpty() && IsResourceAvailable(m_ServerName, ResourceAcq, m_ResourceIndex);
}

void CCamVicDlg::OnGenerateVic() 
{
	// Server name
	CString serverName(m_ServerName);

	// Resource index
	CString resourceIndex;
	resourceIndex.Format("%d", m_ResourceIndex);

	// Default VIC file name
	CString vicFileName = m_currentCamFileName;
	if (vicFileName.Replace(".cca", ".cvi") == 0)
		vicFileName += ".cvi";

	// Create command line
	CString commandLine = CString(m_makeVicFile)
		+ " " + serverName 
		+ " " + resourceIndex 
		+ " \"" + m_currentCamDir + "\\" + m_currentCamFileName + "\""
		+ " \"" + m_currentVicDir + "\\" + vicFileName + "\"";

	// Launch MakeVic utility
	STARTUPINFO startUpInfo;
	PROCESS_INFORMATION processInfo;
	GetStartupInfo(&startUpInfo);
	startUpInfo.dwFlags = STARTF_USESHOWWINDOW;
	startUpInfo.wShowWindow = SW_HIDE;

	BeginWaitCursor();

	if (!CreateProcess(NULL, commandLine.GetBuffer(0), NULL, NULL, TRUE, CREATE_NEW_PROCESS_GROUP | NORMAL_PRIORITY_CLASS, NULL, NULL, &startUpInfo, &processInfo))
	{
		AfxMessageBox("The MakeVic utility cannot be started.");
	}
	else
	{
		WaitForSingleObject(processInfo.hProcess, 10000);
	}

	EndWaitCursor();

	commandLine.ReleaseBuffer();

	// Update VIC name and list box
   CorStrncpy(m_currentVicFileName, vicFileName, sizeof(m_currentVicFileName));
	UpdateNames(FALSE);
}

void CCamVicDlg::OnOK() 
{
	CDialog::OnOK();

	// Create complete strings
	CorSnprintf(m_CamFile, sizeof(m_CamFile), "%s\\%s", m_currentCamDir, m_currentCamFileName);
	CorSnprintf(m_VicFile, sizeof(m_VicFile), "%s\\%s", m_currentVicDir, m_currentVicFileName);

	// Update acquisition object
	UpdateAcquisition();
	
	// Save to registry
	SaveSettings();
}

void CCamVicDlg::OnSelchangeComboServer() 
{
	UpdateData(TRUE);
	InitResourceCombo();
	UpdateBoxAvailability();
}

void CCamVicDlg::OnSelchangeComboResource() 
{
	UpdateData(TRUE);
}
