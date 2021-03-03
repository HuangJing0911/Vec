
// XCameraLinkDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XCameraLinkDemo.h"
#include "XCameraLinkDemoDlg.h"

#include "ixcmd_sink.h"
#include "iximg_sink.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CXCameraLinkDemoDlg dialog


CXCameraLinkDemoDlg* demo_dlg_;

CXCameraLinkDemoDlg::CXCameraLinkDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXCameraLinkDemoDlg::IDD, pParent)
	, _frame_counter(0)
	, _line_lost_num(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	demo_dlg_ = this;
}

void CXCameraLinkDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, _control_tab);
	DDX_Control(pDX, IDC_AVG_MAP, _avg_map);
	DDX_Control(pDX, IDC_NOI_MAP, _noi_map);
}

BEGIN_MESSAGE_MAP(CXCameraLinkDemoDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CXCameraLinkDemoDlg::OnTcnSelchangeTab1)
	ON_MESSAGE(WM_FRAME_READY, OnFrameReadyMsg)
	ON_MESSAGE(WM_EVENT, OnEventMsg)
	ON_MESSAGE(WM_ERROR, OnErrorMsg)
	ON_MESSAGE(WM_ANALYSIS,OnAnalysisMsg)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CXCameraLinkDemoDlg message handlers

BOOL CXCameraLinkDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	_control_tab.InsertItem(0, "Configure");
	_control_tab.InsertItem(1, "Control");
	_control_tab.InsertItem(2, "Operate");

	CRect rect;
	_control_tab.GetClientRect(&rect);
	rect.top += 25;
	rect.bottom -= 1;
	rect.left += 1;
	rect.right -= 1;
	
	_config_dlg.Create(IDD_CONFIGURE_DIALOG, GetDlgItem(IDC_TAB1));
	_operate_dlg.Create(IDD_OPERATE_DIALOG, GetDlgItem(IDC_TAB1));
	_control_dlg.Create(IDD_CONTROL_DIALOG, GetDlgItem(IDC_TAB1));

	_config_dlg.MoveWindow(&rect);
	_config_dlg.ShowWindow(TRUE);
	_operate_dlg.MoveWindow(&rect);
	_operate_dlg.ShowWindow(FALSE);
	_control_dlg.MoveWindow(&rect);
	_control_dlg.ShowWindow(FALSE);

	_control_tab.SetCurSel(0);

	_config_dlg._xacquisition_	= &_xacquisition;
	_config_dlg._xcommand_		= &_xcommand;
	_config_dlg._xdisplay_		= &_xdisplay;
	_config_dlg._xfactory_		= &_xfactory;
	_config_dlg._xoff_correct_  = &_xoff_correct;
	_config_dlg._xdev_			= &_xdevice;
	_config_dlg._xtransfer_		= &_xtransfer;

	_operate_dlg._xcommand_		= &_xcommand;
	_operate_dlg._xacquisition_ = &_xacquisition;
	_operate_dlg._xoff_correct_ = &_xoff_correct;
	_operate_dlg._xdisplay_		= &_xdisplay; 
	_operate_dlg._xtransfer_	= &_xtransfer;

	_control_dlg._xcommand_		= &_xcommand;


	RegisterSink();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CXCameraLinkDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CXCameraLinkDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CXCameraLinkDemoDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	int cur_tab = _control_tab.GetCurSel();

	switch(cur_tab)
	{
	case 0:
		_config_dlg.ShowWindow(TRUE);
		_operate_dlg.ShowWindow(FALSE);
		_control_dlg.ShowWindow(FALSE);
		break;
	case 1:
		_config_dlg.ShowWindow(FALSE);
		_operate_dlg.ShowWindow(FALSE);
		_control_dlg.ShowWindow(TRUE);
		_control_dlg.Initial();
		break;

	case 2:
		_config_dlg.ShowWindow(FALSE);
		_operate_dlg.ShowWindow(TRUE);
		_operate_dlg.Initial();
		_control_dlg.ShowWindow(FALSE);
		break;
	}
}

float temperature;
float humidity;
CString err_str;

class XCmdSink : public IXCmdSink
{
public:
	void OnXError(uint32_t err_id, const char* err_msg_)
    {		
		err_str = "Error: " + CString(err_msg_);
		PostMessage(demo_dlg_->m_hWnd, WM_ERROR, 0,0);
    }
	void OnXEvent(uint32_t event_id, float data)
	{
		switch(event_id)
		{
		case 56:
			temperature = data;
			break;
		case 57:
			humidity = data;
			break;
		}
		PostMessage(demo_dlg_->m_hWnd, WM_EVENT, event_id, 0);
	}
};

class XImgSink : public IXImgSink
{
public:
    void OnXError(uint32_t err_id, const char* err_msg_)
	{
		err_str = "Error: " + CString(err_msg_);
		PostMessage(demo_dlg_->m_hWnd, WM_ERROR, 0,0);
	}
    void OnXEvent(uint32_t event_id, uint32_t data)
	{
	   PostMessage(demo_dlg_->m_hWnd, WM_EVENT, event_id, data);
	}
    void OnFrameReady(XImage* image_)
	{
		
		PostMessage(demo_dlg_->m_hWnd,WM_FRAME_READY,(WPARAM)image_,0);
		demo_dlg_->_cur_image_ = image_;
		//Calculate average
		demo_dlg_->_xanalyze.DoAnalyze(image_, 2);
		//Calculate noise
		demo_dlg_->_xanalyze.DoAnalyze(image_,3);

		PostMessage(demo_dlg_->m_hWnd, WM_ANALYSIS, image_->_width,0);
	}

};

XCmdSink xcmd_sink;
XImgSink ximg_sink;

void CXCameraLinkDemoDlg::RegisterSink()
{
	_xcommand.RegisterEventSink(&xcmd_sink);
	_xacquisition.RegisterEventSink(&ximg_sink);
	_xtransfer.RegisterEventSink(&ximg_sink);
}

LRESULT	CXCameraLinkDemoDlg::OnFrameReadyMsg(WPARAM wParam, LPARAM lParam)
{
	_frame_counter++;
	CString str;
	str.Format("Frame Count: %d", _frame_counter);
	GetDlgItem(IDC_FRAME_COUNT)->SetWindowText(str);
	
	XImage* img_ = (XImage*) wParam;
	//Display
	_xdisplay.Display(img_);



	return 0;
}

LRESULT CXCameraLinkDemoDlg::OnErrorMsg(WPARAM wParam, LPARAM lParam)
{

	GetDlgItem(IDC_ERROR)->SetWindowText(err_str);
	return 0;
}

LRESULT CXCameraLinkDemoDlg::OnEventMsg(WPARAM wParam, LPARAM lParam)
{
	CString str;
	switch(wParam)
	{
	case 50:
	case 53:
		_line_lost_num += static_cast<uint32_t>(lParam);
		str.Format("Line Lost: %u", _line_lost_num);
		GetDlgItem(IDC_LOST)->SetWindowText(str);
		break;

	case 56:
		str.Format("Temperature: %.1fC",temperature);
		GetDlgItem(IDC_XGCU_TEMP)->SetWindowText(str);
		break;

	case 57:
		str.Format("Humidity: %.2f%%",humidity);
		GetDlgItem(IDC_XGCU_HUM)->SetWindowText(str);
		break;

	}
	return 0;
}


LRESULT	CXCameraLinkDemoDlg::OnAnalysisMsg(WPARAM wParam, LPARAM lParam)
{
	DrawMap(_xanalyze._col_avg_,(uint32_t)wParam);
	DrawMap(_xanalyze._col_noise_,(uint32_t)wParam);
	return 0;
}

void CXCameraLinkDemoDlg::DrawMap(uint32_t* data_, uint32_t size)
{
	if(NULL == data_)
		return;
	CRect rect;
	CRect map_rect;
	_avg_map.GetClientRect(&rect);
	map_rect.left = rect.left+10;
	map_rect.right = rect.right-10;
	map_rect.top = rect.top + 5;
	map_rect.bottom = rect.bottom - 10;

	CDC* dc_;
	dc_ = _avg_map.GetDC();
	CDC	mem_dc;
	CBitmap mem_bitmap;

	mem_dc.CreateCompatibleDC(NULL);
	mem_bitmap.CreateCompatibleBitmap(dc_, rect.Width(), rect.Height());
	CBitmap* old_bit_ = mem_dc.SelectObject(&mem_bitmap);
	mem_dc.FillSolidRect(0, 0, rect.Width(), rect.Height(), RGB(255,255,255));
	mem_dc.Rectangle(map_rect);
	
	CFont font;
	font.CreatePointFont(64,"Arial");
	mem_dc.SelectObject(font);

	mem_dc.TextOutA(map_rect.right-70, map_rect.top, "AVERAGE");
	CString str;
	for(uint32_t i=0; i<8; i++)
	{
		str.Format("%d", i*size/8);
		mem_dc.TextOutA(map_rect.left+ i*map_rect.Width()/8, map_rect.bottom+1, str);
	}
	str.Format("%d", size);
	mem_dc.TextOutA(map_rect.right-20, map_rect.bottom+1, str);


	uint32_t max_val = 0;
	for(uint32_t i =0; i<size; i++)
	{
		if(max_val < data_[i])
			max_val = data_[i];
	}
	if(max_val == 0)
	{
		CPen pen;
		pen.CreatePen(PS_COSMETIC,1, RGB(255,0,0));
		CPen* old_pen = mem_dc.SelectObject(&pen);
		mem_dc.SetROP2(R2_NOTXORPEN);
		mem_dc.MoveTo(map_rect.left,map_rect.bottom);
		mem_dc.LineTo(map_rect.right, map_rect.bottom);
	}
	else
	{
		str.Format("%d", max_val);
		mem_dc.TextOutA(map_rect.left, map_rect.top, str);
		str.Format("%d", max_val/2);
		mem_dc.TextOutA(map_rect.left, map_rect.top+map_rect.Height()/2, str);

		CPen pen;
		pen.CreatePen(PS_COSMETIC,1, RGB(255,0,0));
		CPen* old_pen = mem_dc.SelectObject(&pen);
		mem_dc.SetROP2(R2_NOTXORPEN);
		int rect_height = map_rect.Height();
		int rect_width = map_rect.Width();
		for(uint32_t i =0; i<size-1; i++)
		{
			mem_dc.MoveTo(map_rect.left+ i*rect_width/size, map_rect.bottom- data_[i]*(rect_height)/max_val);
			mem_dc.LineTo(map_rect.left+ (i+1)*rect_width/size, map_rect.bottom - data_[i+1]*(rect_height)/max_val);
		}
	}
	dc_->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &mem_dc, 0, 0, SRCCOPY);
	mem_bitmap.DeleteObject();
	mem_dc.DeleteDC();
	dc_->DeleteDC();
}

void CXCameraLinkDemoDlg::DrawMap(float* data_, uint32_t size)
{
	if(NULL == data_)
		return;
	CRect rect;
	CRect map_rect;
	_noi_map.GetClientRect(&rect);
	map_rect.left = rect.left+10;
	map_rect.right = rect.right-10;
	map_rect.top = rect.top + 5;
	map_rect.bottom = rect.bottom - 10;

	CDC* dc_;
	dc_ = _noi_map.GetDC();
	CDC	mem_dc;
	CBitmap mem_bitmap;

	mem_dc.CreateCompatibleDC(NULL);
	mem_bitmap.CreateCompatibleBitmap(dc_, rect.Width(), rect.Height());
	CBitmap* old_bit_ = mem_dc.SelectObject(&mem_bitmap);
	mem_dc.FillSolidRect(0, 0, rect.Width(), rect.Height(), RGB(255,255,255));
	mem_dc.Rectangle(map_rect);
	
	CFont font;
	font.CreatePointFont(64,"Arial");
	mem_dc.SelectObject(font);

	mem_dc.TextOutA(map_rect.right-70, map_rect.top, "NOISE");
	CString str;
	for(uint32_t i=0; i<8; i++)
	{
		str.Format("%d", i*size/8);
		mem_dc.TextOutA(map_rect.left+ i*map_rect.Width()/8, map_rect.bottom+1, str);
	}
	str.Format("%d", size);
	mem_dc.TextOutA(map_rect.right-20, map_rect.bottom+1, str);


	float max_val = 0.0f;
	for(uint32_t i =0; i<size; i++)
	{
		if(max_val < data_[i])
			max_val = data_[i];
	}
	if(max_val == 0)
	{
		CPen pen;
		pen.CreatePen(PS_COSMETIC,1, RGB(255,0,0));
		CPen* old_pen = mem_dc.SelectObject(&pen);
		mem_dc.SetROP2(R2_NOTXORPEN);
		mem_dc.MoveTo(map_rect.left,map_rect.bottom);
		mem_dc.LineTo(map_rect.right, map_rect.bottom);
	}
	else
	{
		str.Format("%f", max_val);
		mem_dc.TextOutA(map_rect.left, map_rect.top, str);
		str.Format("%f", max_val/2);
		mem_dc.TextOutA(map_rect.left, map_rect.top+map_rect.Height()/2, str);

		CPen pen;
		pen.CreatePen(PS_COSMETIC,1, RGB(255,0,0));
		CPen* old_pen = mem_dc.SelectObject(&pen);
		mem_dc.SetROP2(R2_NOTXORPEN);
		int rect_height = map_rect.Height();
		int rect_width = map_rect.Width();
		for(uint32_t i =0; i<size-1; i++)
		{
			mem_dc.MoveTo(map_rect.left+ i*rect_width/size, map_rect.bottom- static_cast<int>(data_[i]*rect_height/max_val));
			mem_dc.LineTo(map_rect.left+ (i+1)*rect_width/size, map_rect.bottom - static_cast<int>(data_[i+1]*rect_height/max_val));
		}
	}
	dc_->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &mem_dc, 0, 0, SRCCOPY);
	mem_bitmap.DeleteObject();
	mem_dc.DeleteDC();
	dc_->DeleteDC();
}

void CXCameraLinkDemoDlg::GetSN()
{
	char sn_str[64];
	if(_xcommand.GetPara(XPARA_GCU_SERIAL,sn_str))
	{
		CString str = "SN: " + CString(sn_str);
		GetDlgItem(IDC_SN)->SetWindowText(str);
	}
}

CWnd* CXCameraLinkDemoDlg::GetDispWnd()
{
	return GetDlgItem(IDC_DISPLAY_WND);
}
void CXCameraLinkDemoDlg::SaveImage(CString file_name)
{
	if(_cur_image_)
		_cur_image_->Save(file_name.GetBuffer());
}
void CXCameraLinkDemoDlg::OnClose()
{
	_xoff_correct.Close();
	_xdisplay.Close();
	_xacquisition.Close();
	_xcommand.Close();
	CDialog::OnClose();
}
