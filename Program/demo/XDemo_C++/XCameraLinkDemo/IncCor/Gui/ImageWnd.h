#ifndef _IMAGEWND_H_
#define _IMAGEWND_H_

//	ImageWnd.h : header file
//


//
// CSapRectTracker class declaration
//

class CSapRectTracker: public CRectTracker
{
public:
   void AdjustRect( int nHandle, LPRECT lpRect );

   void SetLimitRect(CRect *rect);

protected:
   CRect m_LimitRect;
};


//
// CImageWnd class declaration
//
class SAPCLASSGUI_CLASS CImageWnd
{
public:
	// Constructor/Destructor
	CImageWnd(SapView *pView, CWnd *pViewWnd, CScrollBar *pHorzScr, CScrollBar *pVertScr, CWnd *pAppWnd= NULL);
	CImageWnd(SapView *pView, CWnd *pAppWnd);
	virtual ~CImageWnd();

public:
	BOOL IsPointInside(CPoint point);
	CPoint TranslateMousePos(CPoint point);
	CPoint TranslatePos( CPoint point);
	CPoint UntranslatePos( CPoint point);
	void Invalidate(BOOL bErase = TRUE);

	void UpdateClientArea();
	void UpdateClientArea( COLORREF color);

	void OnMove();
	void OnPaint();
	void OnSize(BOOL refreshNow = TRUE);
	void OnHScroll(UINT nSBCode, UINT nPos);
	void OnVScroll(UINT nSBCode, UINT nPos);
	BOOL OnLButtonDown(CPoint point);
	BOOL OnSetCursor(UINT nHitTest);
	CString GetPixelString(CPoint point);

	BOOL  IsRoiTrackerActive() { return m_RectTracker.m_nStyle; }
	void  DisplayRoiTracker();
	void  HideRoiTracker();
	CRect GetSelectedRoi();
	void  SelectRoi(CRect rect);

protected:
	void  UpdateScrollBars();
	void  UpdateRectTracker();
	int   GetScrollPos( int nBar);
	void  GetScrollRange( int nBar, LPINT pMinPos, LPINT pMaxPos);
	int   SetScrollPos( int nBar, int nPos, BOOL bRedraw= TRUE);

	BOOL  GetScrollInfo( int nBar, LPSCROLLINFO pInfo);
	BOOL  SetScrollInfo( int nBar, LPSCROLLINFO pInfo, BOOL bRedraw= TRUE);

   CRect ValidateRoi(CRect inRect);

protected:
	// Parameters
	SapView	   *m_pView;
	CWnd			*m_pViewWnd;
	CScrollBar	*m_pHorzScr;
	CScrollBar	*m_pVertScr;
	CWnd			*m_pAppWnd;

	// Other variables
	CRect				   m_ViewRect;
	CSapRectTracker	m_RectTracker;
	CRect					m_roi;

   // Now obsolete in this class, but keep for compatibility with derived class in CamExpert
	int               m_hScrollScaleFact;
	int			      m_vScrollScaleFact;
}; 

#endif	// _IMAGEWND_H_

