#if !defined(AFX_BUFDLG_H__9089A902_F115_11D1_AF7D_00A0C91AC0FB__INCLUDED_)
#define AFX_BUFDLG_H__9089A902_F115_11D1_AF7D_00A0C91AC0FB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// BufDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Format table

class SapFormatInfo
{
public:
	SapFormat m_Value;
	char *m_Name;
};

/////////////////////////////////////////////////////////////////////////////
// CBufDlg dialog

class SAPCLASSGUI_CLASS CBufDlg : public CDialog
{
// Construction
public:
	CBufDlg(CWnd* pParent, SapBuffer *pBuffer, SapDisplay *pDisplay = NULL);
	SapBuffer &GetBuffer() { return m_Buffer; }

// Dialog Data
	//{{AFX_DATA(CBufDlg)
	enum { IDD = IDD_SCG_BUFFER };
	CComboBox	m_cbFormat;
	int m_Width;
	int m_Height;
	int m_Count;
	int		m_PixelDepth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBufDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL UpdateBuffer();
   void EnableControls();

	// Generated message map functions
	//{{AFX_MSG(CBufDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboFormat();
	afx_msg void OnContiguous();
	afx_msg void OnScatterGather();
	afx_msg void OnOffscreenVideo();
	afx_msg void OnOverlay();
	virtual void OnOK();
	afx_msg void OnBufferVirtual();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	SapBuffer m_Buffer;		// Internal SapBuffer object
	SapFormat m_Format;
	SapBuffer::Type m_Type;
	SapBuffer::Type m_ScatterGatherType;
	SapXferNode *m_pSrcNode;
   SapLocation m_Location;
   SapDisplay *m_pDisplay;

	static SapFormatInfo m_FormatInfo[];
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUFDLG_H__9089A902_F115_11D1_AF7D_00A0C91AC0FB__INCLUDED_)
