// PixProDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SapClassBasic.h"
#include "SapClassGui.h"
#include "PixpKey.h"
#include "BrowseDirDlg.h"
#include "PixProAbsSubDlg.h"
#include "PixProIntScaleDlg.h"
#include "PixProRefImageDlg.h"
#include "PixProUflowSubDlg.h"
#include "PixProWAvgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPixProDlg dialog
//
CPixProDlg::CPixProDlg(CWnd* pParent, SapPixPro *pPixPro, SapXferNode *pSrcNode)
	: CDialog(CPixProDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPixProDlg)
	m_Function = 0;
	m_Config = 0;
	//}}AFX_DATA_INIT

	m_App = AfxGetApp();

	if (pPixPro) 
   {
      // Get all parameters from external object
      m_PixPro = *pPixPro;
   }
   else
   {
      // Get some parameters from source node
      if (pSrcNode)
      {
         SapLocation loc(pSrcNode->GetLocation(), 0);
         m_PixPro = SapPixPro(loc, "", pSrcNode, NULL);
      }

   	// Get remaining parameters from registry
	   LoadSettings();
   }
}

void CPixProDlg::LoadSettings()
{
	// Read CPI file
	CString str = m_App->GetProfileString("SapPixPro", "File", "");
   m_PixPro.SetFile(str.GetBuffer(0));

	// Get last function and config
	int functionIndex = m_App->GetProfileInt("SapPixPro", "Function", 0);
	int configIndex = m_App->GetProfileInt("SapPixPro", "Config", 0);

   // Update PixPro object
   SapPixProParams params = *m_PixPro.GetParams();
   params.SetFunctionIndex(functionIndex);
   params.SetConfigIndex(configIndex);
   m_PixPro.SetParams(&params);
}

void CPixProDlg::SaveSettings()
{
	// Write CPI file name
	m_App->WriteProfileString("SapPixPro", "File", m_PixPro.GetFile());

	// Write function and config indexes
	m_App->WriteProfileInt("SapPixPro", "Function", m_PixPro.GetParams()->GetFunctionIndex());
	m_App->WriteProfileInt("SapPixPro", "Config", m_PixPro.GetParams()->GetConfigIndex());
}

void CPixProDlg::SetDirectories()
{
	m_currentDir[0] = '\0';
	m_currentFileIndex = -1;
	m_currentFileName[0] = '\0';

	CString pathName;
	char acDrive[_MAX_DRIVE];
	char acDir[_MAX_DIR];
	char acFname[_MAX_FNAME];
	char acExt[_MAX_EXT];

	// Get last CPI file accessed
	if (strlen(m_PixPro.GetFile()))
	{
      CorSplitpath(m_PixPro.GetFile(), acDrive, _MAX_DRIVE, acDir, _MAX_DIR, acFname, _MAX_FNAME, acExt, _MAX_EXT);
		CorSnprintf(m_currentDir, sizeof(m_currentDir), "%s%s", acDrive, acDir);
		CorSnprintf(m_currentFileName, sizeof(m_currentFileName), "%s%s", acFname, acExt);
	}
	else
	{
		if (GetEnvironmentVariable("COBRADIR", m_currentDir, sizeof(m_currentDir)) > 0 ||
			 GetEnvironmentVariable("VIPERDIGDIR", m_currentDir, sizeof(m_currentDir)) > 0)
         CorStrncat(m_currentDir, "\\PixPro", sizeof(m_currentDir) - strlen(m_currentDir));
		else
			GetCurrentDirectory(sizeof(m_currentDir), m_currentDir);
	}

	// Remove trailing backslash
   if (m_currentDir[strlen(m_currentDir) - 1] == '\\')
      m_currentDir[strlen(m_currentDir) - 1] = '\0';
}

void CPixProDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPixProDlg)
	DDX_Control(pDX, IDC_SCG_PP_COMBO, m_cbDesign);
	DDX_Control(pDX, IDC_SCG_PP_LIST_FILENAME, m_listFilename);
	DDX_Control(pDX, IDC_SCG_PP_FUNCTION, m_cbFunction);
	DDX_Control(pDX, IDC_SCG_PP_CONFIG, m_cbConfig);
	DDX_CBIndex(pDX, IDC_SCG_PP_FUNCTION, m_Function);
	DDX_CBIndex(pDX, IDC_SCG_PP_CONFIG, m_Config);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPixProDlg, CDialog)
	//{{AFX_MSG_MAP(CPixProDlg)
	ON_BN_CLICKED(IDC_SCG_PP_BROWSE, OnBrowse)
	ON_EN_KILLFOCUS(IDC_SCG_PP_EDIT_PATH, OnKillfocusEditPath)
	ON_CBN_SELCHANGE(IDC_SCG_PP_COMBO, OnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_SCG_PP_FUNCTION, OnSelchangeFunction)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPixProDlg message handlers

BOOL CPixProDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Initialize directories
	SetDirectories();

	// Display the current directory to look for the files
	SetDlgItemText(IDC_SCG_PP_EDIT_PATH, m_currentDir);

	// Scan all files in the current directory and fill the list box
	UpdateNames();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//
// Browse for a directory where to look for the files
//
void CPixProDlg::OnBrowse() 
{
	CBrowseDirDlg dlg(m_currentDir);
	if (dlg.DoModal() == IDOK)
	{
		UpdateCurrentDir(dlg.GetDir().GetBuffer(0));
		UpdateNames();
	}
}

void CPixProDlg::OnOK() 
{
	CDialog::OnOK();

	// Create complete string
   char file[MAX_PATH];
	CorSnprintf(file, sizeof(file), "%s\\%s", m_currentDir, m_currentFileName);
	m_PixPro.SetFile(file);

	// Set function/config indexes
   SapPixProParams params = *m_PixPro.GetParams();
   int functionIndex = (int) m_cbFunction.GetItemData(m_Function);
   params.SetFunctionIndex(functionIndex);
   params.SetConfigIndex(m_Config);

	// Select function specific parameters
	CORPPL_FCT_PROP prop;	
	m_PixPro.GetFunctionInfo(functionIndex, &prop, NULL, 0);
	switch (functionIndex)
	{
		// Pass-through and Invert image
		case CORPPL_FCT_PASSTHROUGH:
		case CORPPL_FCT_INVERT_IMAGE:
			params.SetBankId(0);
			break;

		// Reference image
		case CORPPL_FCT_REF_IMAGE:
		{
			CPixProRefImageDlg(this, &prop, &params).DoModal();
			break;
		}
			
		// Absolute subtraction
		case CORPPL_FCT_ABS_IMAGE_SUB:
		{
			CPixProAbsSubDlg(this, &prop, &params).DoModal();
			break;
		}
				
		// Underflow subtraction
		case CORPPL_FCT_UNDERFLOW_IMAGE_SUB:
		{
			CPixProUflowSubDlg(this, &prop, &params).DoModal();
			break;
		}
				
		// Weigthed average
		case CORPPL_FCT_WEIGHTED_AVG:
		{
			CPixProWAvgDlg(this, &prop, &params).DoModal();
			break;
		}
				
		// Integrate and scale
		case CORPPL_FCT_INTEGRATE_SCALE:
		{
			CPixProIntScaleDlg(this, &prop, &params).DoModal();
			break;
		}
	}

   // Update PixPro object
   m_PixPro.SetParams(&params);

	// Save to registry
	SaveSettings();
}

void CPixProDlg::OnKillfocusEditPath() 
{
	char newCurrentDir[MAX_PATH];

	// Get new directory name
	GetDlgItemText(IDC_SCG_PP_EDIT_PATH, newCurrentDir, sizeof(newCurrentDir));

	// Update the dialog box
	UpdateCurrentDir(newCurrentDir);
	UpdateNames();	
}

//
// Update the current CPI directory in the dialog box, and if necessary reset some member variables
// to default values.
//
void CPixProDlg::UpdateCurrentDir(char *newCurrentDir)
{
	// Remove trailing backslash
   if (newCurrentDir[strlen( newCurrentDir) - 1] == '\\')
      newCurrentDir[strlen( newCurrentDir) - 1] = '\0';

   if (CorStricmp(newCurrentDir, m_currentDir))
	{
		// New Directory selected, update the list box selection
      CorStrncpy(m_currentDir, newCurrentDir, sizeof(m_currentDir));
		SetDlgItemText(IDC_SCG_PP_EDIT_PATH, newCurrentDir);
		m_currentFileName[0] = '\0';
	}
}


//
// Update the design list box
//
void CPixProDlg::UpdateNames()
{
	char pathName[MAX_PATH];
	char fileName[MAX_PATH];
	char designName[128];
	int index = 0;
	int fileIndex = 0;

	// fill listbox (hidden) with the design file names
	CorSnprintf(pathName, sizeof(pathName), "%s\\*.%s", m_currentDir, PIXP_FILE_EXT);

	m_listFilename.ResetContent();
	m_listFilename.Dir( 0, pathName);

	// Clear all entries
	m_cbDesign.ResetContent();

	for( fileIndex = 0; fileIndex < m_listFilename.GetCount(); fileIndex++)
	{
		// For every file, create an entry in the combo box with the name of the design
		m_listFilename.GetText( fileIndex, fileName);
		
		CorSnprintf( pathName, sizeof(pathName), "%s\\%s", m_currentDir, fileName);

		GetPrivateProfileString( PIXPSECTION_GENERAL, PIXPKEY_DESIGN_NAME, "No Name", designName, sizeof(designName), pathName);

		index = m_cbDesign.AddString( designName);
		m_cbDesign.SetItemData( index, fileIndex);

	}

	if( m_cbDesign.GetCount())
	{
		int newFileIndex = -1;

      if( strlen( m_currentFileName))
		{
			// Try to find the current design file selected
			for (index = 0; index < m_cbDesign.GetCount(); index++)
			{
            fileIndex = (int) m_cbDesign.GetItemData( index);
				m_listFilename.GetText( fileIndex, fileName);
	
				if( !CorStricmp( fileName, m_currentFileName))
				{
					newFileIndex = index;
					break;
				}
			}
		}

		if (newFileIndex == -1) newFileIndex = 0;
	   m_cbDesign.SetCurSel( newFileIndex);

      m_currentFileIndex = (int) m_cbDesign.GetItemData( newFileIndex);
		m_listFilename.GetText( m_currentFileIndex, m_currentFileName);
		m_fileAvailable = TRUE;
	}
	else
	{
		m_currentFileIndex = -1;
		m_fileAvailable = FALSE;
	}

	FillFunctionCombo();
	UpdateBoxAvailability();
}


void CPixProDlg::UpdateBoxAvailability()
{
	GetDlgItem(IDC_SCG_PP_COMBO)->EnableWindow(m_fileAvailable);
	GetDlgItem(IDOK)->EnableWindow(m_fileAvailable);
	GetDlgItem(IDC_SCG_PP_FUNCTION)->EnableWindow(m_fileAvailable);
	GetDlgItem(IDC_SCG_PP_CONFIG)->EnableWindow(m_fileAvailable && m_Function == 0);
}

void CPixProDlg::OnSelchangeCombo() 
{
	int index;			// List box selection
	int newFileIndex;	// File selection 

	// Get new selection
	index = m_cbDesign.GetCurSel();
   newFileIndex = (int) m_cbDesign.GetItemData( index);

	// Check if selection is different than current one
	if( newFileIndex != m_currentFileIndex)
	{
		m_currentFileIndex = newFileIndex;
		m_listFilename.GetText( m_currentFileIndex, m_currentFileName);
	}

	// Read functions and configurations
	FillFunctionCombo();
	UpdateBoxAvailability();
}

void CPixProDlg::FillFunctionCombo()
{
	// Reset combo contents
	m_cbFunction.ResetContent();
	m_cbConfig.ResetContent();

   if (m_fileAvailable)
   {
      // Read new design
	   CString str;
	   str.Format("%s\\%s", m_currentDir, m_currentFileName);
	   m_PixPro.ReadDesign(str.GetBuffer(0));

      // Do not declare in 'for' statement, as we get an error with Visual Studio 2005 in the next loop
      int i;

	   // Fill function combo
	   char name[256];

      m_Function = 0;
	   m_cbFunction.SetItemData(m_cbFunction.AddString("Predefined Configuration"), (DWORD) -1);

	   for (i=0; i < m_PixPro.GetNumFunctions(); i++)
	   {
		   if (m_PixPro.GetFunctionInfo(i, NULL, name, sizeof(name)))
         {
			   m_cbFunction.SetItemData(m_cbFunction.AddString(name), i);
            if (m_PixPro.GetParams()->GetFunctionIndex() == i)
               m_Function = m_cbFunction.GetCount() - 1;
         }
	   }

	   // Fill configuration combo
	   for (i=0; i < m_PixPro.GetNumConfigs(); i++)
	   {
		   m_PixPro.GetConfigInfo(i, name, sizeof(name));
		   m_cbConfig.AddString(name);
	   }
      m_Config = m_PixPro.GetParams()->GetConfigIndex();
      if (m_Config >= m_cbConfig.GetCount()) m_Config = 0;
   }

	// Update controls
	UpdateData(FALSE);
}

void CPixProDlg::OnSelchangeFunction() 
{
	UpdateData(TRUE);
	UpdateBoxAvailability();
}

