
// XCameraLinkDemoDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "XConfigDlg.h"
#include "XOperateDlg.h"
#include "XControlDlg.h"

#include "xcommand.h"
#include "xcam_factory.h"
#include "xacquisition.h"
#include "xframe_transfer.h"
#include "xdisplay.h"
#include "xoff_correct.h"
#include "xanalyze.h"
#include "xtif_format.h"
#include "afxwin.h"

//#ifdef _WIN64
//#pragma comment(lib, "..\\..\\..\\lib\\x64\\XLibDll.lib")
//#else
//#pragma comment(lib, "..\\..\\..\\lib\\x86\\XLibDll.lib")
//#endif

#define WM_EVENT		 WM_USER+1
#define WM_ERROR		 WM_USER+2
#define WM_FRAME_READY	 WM_USER+3
#define WM_ANALYSIS		 WM_USER+4

// CXCameraLinkDemoDlg dialog
class CXCameraLinkDemoDlg : public CDialog
{
// Construction
public:
	CXCameraLinkDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_XCAMERALINKDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl		_control_tab;
	CXConfigDlg		_config_dlg;
	CXOperateDlg	_operate_dlg;
	CXControlDlg	_control_dlg;

	XDevice			_xdevice;
	XCommand		_xcommand;
	XAcquisition	_xacquisition;
	XFrameTransfer	_xtransfer;
	XDisplay		_xdisplay;
	XOffCorrect		_xoff_correct;
	XCamFactory		_xfactory;
	XAnalyze		_xanalyze;

	uint32_t		_frame_counter;
	uint32_t		_line_lost_num;
	XImage*			_cur_image_;

	CStatic _avg_map;
	CStatic _noi_map;

	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	void RegisterSink();
	void GetSN();
	CWnd* GetDispWnd();

	LRESULT	OnFrameReadyMsg(WPARAM wParam, LPARAM lParam);
	LRESULT OnErrorMsg(WPARAM wParam, LPARAM lParam);
	LRESULT OnEventMsg(WPARAM wParam, LPARAM lParam);
	LRESULT	OnAnalysisMsg(WPARAM wParam, LPARAM lParam);
	void DrawMap(uint32_t* data_, uint32_t size);
	void DrawMap(float* data_, uint32_t size);
	void SaveImage(CString file_name);
	afx_msg void OnClose();
};
