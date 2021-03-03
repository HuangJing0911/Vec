//	ImageWnd.cpp : implementation file
//

#include "stdafx.h"
#include "SapClassBasic.h"
#include "SapClassGui.h"
#include <math.h>

//
// Constructor/Destructor
//
CImageWnd::CImageWnd(SapView *pView, CWnd *pViewWnd, CScrollBar *pHorzScr, CScrollBar *pVertScr, CWnd *pAppWnd)
{
	// Parameters
	m_pView   	= pView;
	m_pViewWnd	= pViewWnd;
	m_pHorzScr	= pHorzScr;
	m_pVertScr	= pVertScr;
	m_pAppWnd	= pAppWnd;

	if( m_pAppWnd == NULL)
	{
		m_pAppWnd= m_pViewWnd;
		m_pViewWnd= NULL;
	}

   // Now obsolete in this class, but keep for compatibility with derived class in CamExpert
	m_hScrollScaleFact= 1;
	m_vScrollScaleFact= 1;

	m_roi= CRect(0,0,0,0);
	UpdateRectTracker();

	OnSize();
}

CImageWnd::CImageWnd(SapView *pView, CWnd *pAppWnd)
{
	// Parameters
	m_pView   	= pView;
	m_pViewWnd	= NULL;
	m_pHorzScr	= NULL;
	m_pVertScr	= NULL;
	m_pAppWnd	= pAppWnd;

   // Now obsolete in this class, but keep for compatibility with derived class in CamExpert
	m_hScrollScaleFact= 1;
	m_vScrollScaleFact= 1;

	m_roi= CRect(0,0,0,0);
	UpdateRectTracker();

	OnSize();
}

CImageWnd::~CImageWnd()
{
}

int CImageWnd::GetScrollPos( int nBar)
{
	SCROLLINFO scrollInfo= {0};

	scrollInfo.cbSize= sizeof( SCROLLINFO);
	scrollInfo.fMask = SIF_POS;

	// Get current position
	GetScrollInfo( nBar, &scrollInfo);

	return scrollInfo.nPos;
}

void CImageWnd::GetScrollRange( int nBar, LPINT pMinPos, LPINT pMaxPos)
{
	SCROLLINFO scrollInfo= {0};

	scrollInfo.cbSize= sizeof( SCROLLINFO);
	scrollInfo.fMask = SIF_RANGE;

	// Get current range
	GetScrollInfo( nBar, &scrollInfo);

	*pMinPos= scrollInfo.nMin;
	*pMaxPos= scrollInfo.nMax;

	return;
}

BOOL CImageWnd::GetScrollInfo( int nBar, LPSCROLLINFO pInfo)
{
	if( nBar == SB_HORZ)
	{
		if( m_pHorzScr != NULL) 
		{
			return m_pHorzScr->GetScrollInfo( pInfo);
		}
		else if( m_pAppWnd != NULL)  
		{
			return m_pAppWnd->GetScrollInfo( nBar, pInfo);
		}
	}
	else if( nBar == SB_VERT)
	{
		if( m_pVertScr != NULL)
		{
			return m_pVertScr->GetScrollInfo( pInfo);
		}
		else if( m_pAppWnd != NULL)  
		{
			return m_pAppWnd->GetScrollInfo( nBar, pInfo);
		}
	}	

	return FALSE;
}

int CImageWnd::SetScrollPos( int nBar, int nPos, BOOL bRedraw)
{
	SCROLLINFO scrollInfo= {0};

	scrollInfo.cbSize= sizeof( SCROLLINFO);
	scrollInfo.fMask = SIF_POS;

	// Get current position
	GetScrollInfo( nBar, &scrollInfo);
	int oldPos= scrollInfo.nPos;

	// Set new position
	scrollInfo.nPos= nPos;
	SetScrollInfo( nBar, &scrollInfo, bRedraw);

	return oldPos;
}

BOOL CImageWnd::SetScrollInfo( int nBar, LPSCROLLINFO pInfo, BOOL bRedraw)
{
	if( nBar == SB_HORZ)
	{
		if( m_pHorzScr != NULL) 
		{
			return m_pHorzScr->SetScrollInfo( pInfo, bRedraw);
		}
		else if( m_pAppWnd != NULL)  
		{
			return m_pAppWnd->SetScrollInfo( nBar, pInfo, bRedraw);
		}
	}
	else if( nBar == SB_VERT)
	{
		if( m_pVertScr != NULL) 
		{
			return m_pVertScr->SetScrollInfo( pInfo, bRedraw);
		}
		else if( m_pAppWnd != NULL)  
		{
			return m_pAppWnd->SetScrollInfo( nBar, pInfo, bRedraw);
		}
	}

	return FALSE;
}

BOOL CImageWnd::IsPointInside(CPoint point)
{
	return m_ViewRect.PtInRect(point);
}

CPoint CImageWnd::TranslateMousePos(CPoint point)
{
	// Check that the position is inside the view rectangle
	if( !IsPointInside( point))
		return CPoint( 0, 0);

	CPoint translatedPoint= point - m_ViewRect.TopLeft();

	return TranslatePos( translatedPoint);
}

CPoint CImageWnd::TranslatePos( CPoint point)
{
	CPoint translatedPoint= point;
	
	if( translatedPoint.x < 0)
		translatedPoint.x= 0;

	if( translatedPoint.y < 0)
		translatedPoint.y= 0;

	translatedPoint.x+= (int) (GetScrollPos( SB_HORZ) * m_pView->GetScaleParamsSrc().ZoomHorz());
	translatedPoint.y+= (int) (GetScrollPos(SB_VERT)  * m_pView->GetScaleParamsSrc().ZoomVert());

	translatedPoint.x= (int) (translatedPoint.x/m_pView->GetScaleParamsSrc().ZoomHorz());
	translatedPoint.y= (int) (translatedPoint.y/m_pView->GetScaleParamsSrc().ZoomVert());

   if (m_pView && m_pView->GetBuffer())
   {
	   if( translatedPoint.x >= m_pView->GetBuffer()->GetWidth())
		   translatedPoint.x= m_pView->GetBuffer()->GetWidth()-1;

	   if( translatedPoint.y >= m_pView->GetBuffer()->GetHeight())
		   translatedPoint.y= m_pView->GetBuffer()->GetHeight()-1;
   }

	return translatedPoint;
}

CPoint CImageWnd::UntranslatePos( CPoint point)
{
	CPoint translatedPoint= point;

	if( translatedPoint.x < 0)
		translatedPoint.x= 0;

	if( translatedPoint.y < 0)
		translatedPoint.y= 0;

	translatedPoint.x= (int) (translatedPoint.x * m_pView->GetScaleParamsSrc().ZoomHorz());
	translatedPoint.y= (int) (translatedPoint.y * m_pView->GetScaleParamsSrc().ZoomVert());

	translatedPoint.x-= (int) (GetScrollPos( SB_HORZ) * m_pView->GetScaleParamsSrc().ZoomHorz());
	translatedPoint.y-= (int) (GetScrollPos(SB_VERT)  * m_pView->GetScaleParamsSrc().ZoomVert());

   if (m_pView)
   {
	   if( translatedPoint.x >= m_pView->GetWidth())
		   translatedPoint.x= m_pView->GetWidth()-1;

	   if( translatedPoint.y >= m_pView->GetHeight())
		   translatedPoint.y= m_pView->GetHeight()-1;
   }

	return translatedPoint;
}

void CImageWnd::UpdateClientArea( COLORREF color)
{
	if (m_pView != NULL && m_pViewWnd == NULL && m_pAppWnd != NULL)
	{
		CDC *pDc= m_pAppWnd->GetDC();
		CRect appRect;

		m_pAppWnd->GetClientRect(appRect);

		// Check if unused region of the AppWnd should be repainted
		int viewWidth = m_pView->GetWidth();
		int viewHeight= m_pView->GetHeight();

		if( appRect.Width() > viewWidth)
		{
			CRect rect= appRect;
			rect.left= viewWidth;

			pDc->FillSolidRect( rect, color);		
		}

		if( appRect.Height() > viewHeight)
		{
			CRect rect= appRect;
			rect.top= viewHeight;

			pDc->FillSolidRect( rect, color);		
		}

		m_pAppWnd->ReleaseDC( pDc);
	}
}

void CImageWnd::UpdateClientArea()
{
   if (m_pViewWnd == NULL && m_pAppWnd != NULL)
   {
      CDC *pDc= m_pAppWnd->GetDC();
      COLORREF color = pDc->GetBkColor();
      m_pAppWnd->ReleaseDC(pDc);

      UpdateClientArea(color);
   }
}

void CImageWnd::OnMove()
{
	// Call corresponding handler if application window is not being iconified
   if (m_pView != NULL && m_pAppWnd != NULL && !IsIconic(m_pAppWnd->m_hWnd))
   {
   	m_pView->OnMove();
   }
}

void CImageWnd::OnPaint()
{
	// Call corresponding handler if application window is not being iconified
   if (m_pView != NULL && m_pAppWnd != NULL && !IsIconic(m_pAppWnd->m_hWnd))
   {
   	m_pView->OnPaint();
   }
}

void CImageWnd::OnSize(BOOL refreshNow)
{
	if( m_pAppWnd == NULL)
		return;

   // If application window is being iconified, hide the current view
	if( IsIconic(m_pAppWnd->m_hWnd))
   {
      if (m_pView)
      	m_pView->Hide();
      return;
   }

	// Get application rectangle
	CRect appRect;
	m_pAppWnd->GetClientRect(appRect);

	// Get view rectangle
	CRect viewRect;
	if( m_pViewWnd != NULL)
	{
		m_pViewWnd->GetWindowRect(viewRect);
		m_pAppWnd->ScreenToClient(viewRect);
	}
	else
	{
		viewRect= appRect;
	}

	if( m_pViewWnd != NULL && m_pHorzScr != NULL && m_pVertScr != NULL)
	{
		// Get scroll bars rectangles
		CRect horzRect, vertRect;
		m_pHorzScr->GetWindowRect(horzRect);
		m_pVertScr->GetWindowRect(vertRect);
		m_pAppWnd->ScreenToClient(horzRect);
		m_pAppWnd->ScreenToClient(vertRect);

		// Adjust windows' position
		viewRect.right = appRect.right - 5 - vertRect.Width();
		viewRect.bottom = appRect.bottom - 5 - horzRect.Height();
		m_pViewWnd->MoveWindow(viewRect.left, viewRect.top, viewRect.Width(), viewRect.Height(), TRUE);

		horzRect.top = viewRect.bottom;
		horzRect.right = viewRect.right;
		horzRect.bottom = appRect.bottom - 5;
		m_pHorzScr->MoveWindow(horzRect.left, horzRect.top, horzRect.Width(), horzRect.Height(), TRUE);

		vertRect.left = viewRect.right;
		vertRect.right = appRect.right - 5;
		vertRect.bottom = viewRect.bottom;
		m_pVertScr->MoveWindow(vertRect.left, vertRect.top, vertRect.Width(), vertRect.Height(), TRUE);	
	}

   if (m_pView)
   {
	   // Call corresponding handler
	   m_pView->OnSize(refreshNow);

	   // Update scroll bars' position and range
	   // Has to be called twice (JPC)
	   UpdateScrollBars();
	   UpdateScrollBars();

	   // Update view rectangle
	   m_ViewRect        = viewRect;
	   m_ViewRect.left  += m_pView->GetScaleParamsDst().Left();
	   m_ViewRect.top   += m_pView->GetScaleParamsDst().Top();
	   m_ViewRect.right  = m_ViewRect.left + m_pView->GetWidth();
	   m_ViewRect.bottom = m_ViewRect.top + m_pView->GetHeight();

      // Update tracker limits
      CRect limitRect;

      limitRect.top   = 0;
	   limitRect.left  = 0;
      limitRect.right = m_pView->GetWidth();
      limitRect.bottom= m_pView->GetHeight();
      m_RectTracker.SetLimitRect(&limitRect);

	   if( m_pViewWnd == NULL)
	   {
		   // Check if unused region of the AppWnd should be repainted
		   int viewWidth = m_pView->GetWidth();
		   int viewHeight= m_pView->GetHeight();

		   if( appRect.Width() > viewWidth)
		   {
			   CRect rect= appRect;

			   rect.left= viewWidth;
			   m_pAppWnd->InvalidateRect( rect, TRUE);
		   }
		   if( appRect.Height() > viewHeight)
		   {
			   CRect rect= appRect;
			
			   rect.top= viewHeight;
			   m_pAppWnd->InvalidateRect( rect, TRUE);
		   }
	   }
   }
}

void CImageWnd::Invalidate(BOOL bErase)
{
	if( m_pAppWnd != NULL)
		m_pAppWnd->InvalidateRect(m_ViewRect, bErase);
}

void CImageWnd::UpdateScrollBars()
{
   // Note: the view pointer has already been validated by the caller (OnSize method)
	int viewWidth  = m_pView->GetWidth();
	int viewHeight = m_pView->GetHeight();
	int pageWidth  = m_pView->GetWidth();
	int pageHeight = m_pView->GetHeight();

   if (m_pView->GetScalingMode() == SapView::ScalingNone)
	{
      if (m_pView->GetBuffer())
      {
		   viewWidth = m_pView->GetBuffer()->GetWidth();
		   viewHeight= m_pView->GetBuffer()->GetHeight();
      }
      // pageWidth and pageHeight are already initialized correctly
	}

	else if( m_pView->GetScalingMode() == SapView::ScalingFitToWindow)
	{
      // viewWidth and viewHeight are already initialized correctly
      // pageWidth and pageHeight are already initialized correctly
	}

	else
	{
      if (m_pView->GetBuffer())
      {
   		viewWidth = m_pView->GetBuffer()->GetWidth();
	   	viewHeight= m_pView->GetBuffer()->GetHeight();
      }
		pageWidth = m_pView->GetScaleParamsSrc().Width(); 
		pageHeight= m_pView->GetScaleParamsSrc().Height(); 
	}

	// Update tracker position
	UpdateRectTracker();

	// Set scroll bars' page size
   SCROLLINFO info;
   info.cbSize= sizeof(SCROLLINFO);    
   info.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;  

	info.nMin  = 0;
	info.nMax  = (int)((float)(viewWidth + 0.5)	- 1);
   info.nPage = pageWidth;

	info.nPos  = m_pView->GetScrollPos().x;

	SetScrollInfo(SB_HORZ, &info, TRUE);

	info.nMin  = 0;
	info.nMax  = (int)(((float)viewHeight + 0.5) - 1);
   info.nPage = pageHeight;

	info.nPos  = m_pView->GetScrollPos().y;

   SetScrollInfo(SB_VERT, &info, TRUE);

	if( m_pHorzScr != NULL && m_pVertScr != NULL)
	{
		// Enable/disable scroll bars
		m_pHorzScr->EnableWindow(m_pView->GetScrollRange().cx > 0);
		m_pVertScr->EnableWindow(m_pView->GetScrollRange().cy > 0);

		// Show/hide scroll bars
		m_pHorzScr->ShowScrollBar(m_pView->GetScrollRange().cx > 0);
		m_pVertScr->ShowScrollBar(m_pView->GetScrollRange().cy > 0);
	}
}

void CImageWnd::OnHScroll(UINT nSBCode, UINT nTrackPos)
{
	// Get current position and limits
	SCROLLINFO scrollInfo= {0};

	scrollInfo.cbSize= sizeof( scrollInfo);
	scrollInfo.fMask= SIF_TRACKPOS | SIF_RANGE | SIF_POS;

	GetScrollInfo( SB_HORZ, &scrollInfo);

	int left= scrollInfo.nPos;
	int hMin= scrollInfo.nMin;
	int hMax= scrollInfo.nMax;

   // Calculate new position
	switch (nSBCode)
	{
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION: 
			left = scrollInfo.nTrackPos; 
			break;
		case SB_LINELEFT: 
			if (left > hMin) left--; break;
		case SB_LINERIGHT: 
			if (left < hMax) left++; break;
		case SB_PAGELEFT: 
			left -= 10; 
			if (left < hMin) left = hMin; break;
		case SB_PAGERIGHT: 
			left += 10; 
			if (left > hMax) left = hMax; break;
	}

   // Store new position and reread it to validate
	scrollInfo.nPos= left;
	SetScrollInfo( SB_HORZ, &scrollInfo, TRUE);

   // Update view and tracker
   if (m_pView)
      m_pView->OnHScroll(left);

	UpdateRectTracker();
}

void CImageWnd::OnVScroll(UINT nSBCode, UINT nTrackPos) 
{
	// Get current position and limits
	SCROLLINFO scrollInfo= {0};

	scrollInfo.cbSize= sizeof( scrollInfo);
	scrollInfo.fMask= SIF_TRACKPOS | SIF_RANGE | SIF_POS;

	GetScrollInfo( SB_VERT, &scrollInfo);

	int top = scrollInfo.nPos;
	int vMin= scrollInfo.nMin;
	int vMax= scrollInfo.nMax;

   // Calculate new position
	switch( nSBCode)
	{
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION: 
			top = scrollInfo.nTrackPos; 
			break;
		case SB_LINEUP: 
			if (top > vMin) top--; break;
		case SB_LINEDOWN: 
			if (top < vMax) top++; break;
		case SB_PAGEUP: 
			top -= 10; 
			if (top < vMin) top = vMin; break;
		case SB_PAGEDOWN: 
			top += 10; 
			if (top > vMax) top = vMax; break;
	}

   // Store new position and reread it to validate
	scrollInfo.nPos= top;
	SetScrollInfo( SB_VERT, &scrollInfo, TRUE);

   // Update view and tracker
   if (m_pView)
   	m_pView->OnVScroll( top);

	UpdateRectTracker();
}

void CImageWnd::DisplayRoiTracker()
{
	if( m_pAppWnd == NULL)
		return;

	if( m_RectTracker.m_nStyle) 
	{
		CRect rect;
		CWnd *pViewWnd;

		pViewWnd= m_pViewWnd != NULL? m_pViewWnd: m_pAppWnd;

		pViewWnd->GetClientRect(&rect);

		CRgn clipRgn;
		clipRgn.CreateRectRgnIndirect(&rect);

		CClientDC dc(pViewWnd);
		dc.SelectClipRgn(&clipRgn);
		dc.SetWindowOrg(GetScrollPos(SB_HORZ), GetScrollPos(SB_VERT));

		m_RectTracker.Draw(&dc);
	}
}

void CImageWnd::HideRoiTracker()
{
	m_RectTracker.m_rect = CRect(0,0,0,0);
	m_RectTracker.m_nStyle = 0;

	m_roi= CRect(0,0,0,0);
}

void CImageWnd::UpdateRectTracker()
{
	m_RectTracker.m_rect.TopLeft()    = UntranslatePos( m_roi.TopLeft());
	m_RectTracker.m_rect.BottomRight()= UntranslatePos( m_roi.BottomRight());
}

CRect CImageWnd::ValidateRoi(CRect inRect)
{
	CRect outRect(inRect);
	outRect.NormalizeRect();

   if (m_pView == NULL || m_pView->GetBuffer() == NULL)
      return outRect;

	int viewWidth = m_pView->GetBuffer()->GetWidth();
	int viewHeight= m_pView->GetBuffer()->GetHeight();

	if (outRect.left < 0) 
		outRect.left = 0;

	if (outRect.right > viewWidth-1) 
		outRect.right = viewWidth-1;

	if (outRect.top < 0) 
		outRect.top = 0;

	if (outRect.bottom > viewHeight - 1) 
		outRect.bottom = viewHeight - 1;

   return outRect;
}

CRect CImageWnd::GetSelectedRoi()
{
   // We must normalize m_RectTracker.m_rect and adjust for scroll position here, as well as in 
   // OnLButtonDown, since GetSelectRoi may get called in the WM_LBUTTONUP handler of a global 
   // mouse hook function.  In that case, we got here before TrackRubberBand or Track had a chance to
   // return to OnLButtonDown.

	return m_roi;
}

void CImageWnd::SelectRoi(CRect rect)
{
   m_roi = ValidateRoi(rect);
	UpdateRectTracker();

   m_RectTracker.m_nStyle = CRectTracker::resizeInside | CRectTracker::dottedLine;
}

BOOL CImageWnd::OnLButtonDown(CPoint point)
{
	if( m_pAppWnd == NULL)
		return FALSE;

	if( IsPointInside(point))
	{
		CPoint pt= point - m_ViewRect.TopLeft();
		CWnd *pViewWnd;

		pViewWnd= m_pViewWnd != NULL? m_pViewWnd: m_pAppWnd;
		
      // TrackRubberBand and Track methods capture the mouse and return when user releases left mouse button,
      // presses right mouse button or escape key.
		if (m_RectTracker.HitTest(pt) == CRectTracker::hitNothing)
		{
			m_RectTracker.m_nStyle = CRectTracker::resizeInside | CRectTracker::dottedLine;
         m_RectTracker.TrackRubberBand(pViewWnd, pt);
		}
		else
		{
			m_RectTracker.Track(pViewWnd, pt, FALSE, pViewWnd);
		}

      // normalize m_rect for 4th quadrant coordinates and limit according to scroll position
      m_RectTracker.m_rect.NormalizeRect();

		m_roi.TopLeft()    = TranslatePos( m_RectTracker.m_rect.TopLeft());
		m_roi.BottomRight()= TranslatePos( m_RectTracker.m_rect.BottomRight());

		return TRUE;
	}

	return FALSE;
}

BOOL CImageWnd::OnSetCursor(UINT nHitTest)
{
	if( m_pAppWnd == NULL)
		return FALSE;

	CWnd *pViewWnd= m_pViewWnd != NULL? m_pViewWnd: m_pAppWnd;

	CPoint point;
	GetCursorPos(&point);
	pViewWnd->ScreenToClient(&point);

	CRect clientRect;
	pViewWnd->GetClientRect(&clientRect);

	if (m_RectTracker.m_nStyle && clientRect.PtInRect(point))
	{
		if (m_RectTracker.SetCursor(pViewWnd, nHitTest)) 
		{
			return TRUE;
		}
	}

	return FALSE;
}

CString CImageWnd::GetPixelString(CPoint point) 
{
   CString str = "[ Pixel data not available ]";

   // if there is no buffer to display, return right away
   if (m_pView == NULL || m_pView->GetBuffer() == NULL || !m_pView->GetBuffer()->IsMapped())
      return str;

   if (IsPointInside(point) && *m_pView->GetBuffer())
	{
		CPoint pt = TranslateMousePos(point);

		// Get pixel value at cursor's position and create string according to pixel format
		CString text;
		SapFormat format = m_pView->GetBuffer()->GetFormat();

		if (format == SapFormatHSI || format == SapFormatHSIP8)
      {
         SapDataHSI data;
         m_pView->GetBuffer()->ReadElement(pt.x, pt.y, &data);
			text.Format("[ x= %03ld y= %03ld H= %04X S= %04X I= %04X ]", pt.x, pt.y, data.H(), data.S(), data.I());
      }
		else if (format == SapFormatLAB || format == SapFormatLAB101010)
      {
         SapDataLAB data;
         m_pView->GetBuffer()->ReadElement(pt.x, pt.y, &data);
			text.Format("[ x= %03ld y= %03ld L= %04X A= %04X B= %04X ]", pt.x, pt.y, data.L(), data.A(), data.B());
      }
		else if (format == SapFormatLAB16161616)
      {
         SapDataLABA data;
         m_pView->GetBuffer()->ReadElement(pt.x, pt.y, &data);
			text.Format("[ x= %03ld y= %03ld L= %04X A= %04X B= %04X ]", pt.x, pt.y, data.L(), data.A(), data.B());
      }
		else if (format == SapFormatHSV)
      {
         SapDataHSV data;
         m_pView->GetBuffer()->ReadElement(pt.x, pt.y, &data);
			text.Format("[ x= %03ld y= %03ld H= %04X S= %04X V= %04X ]", pt.x, pt.y, data.H(), data.S(), data.V());
      }
		else if (CORDATA_FORMAT_IS_YUV(format))
      {
         SapDataYUV data;
         m_pView->GetBuffer()->ReadElement(pt.x, pt.y, &data);
			text.Format("[ x= %03ld y= %03ld Y= %04X U= %04X V= %04X ]", pt.x, pt.y, data.Y(), data.U(), data.V());
      }
		else if (CORDATA_FORMAT_IS_RGB(format))
      {
         if(format==SapFormatRGB16161616)
         {
            SapDataRGBA data;
            m_pView->GetBuffer()->ReadElement(pt.x, pt.y, &data);
   			text.Format("[ x= %03ld y= %03ld R= %04X G= %04X B= %04X ]", pt.x, pt.y, data.Red(), data.Green(), data.Blue());
         }
         else
         {
            SapDataRGB data;
            m_pView->GetBuffer()->ReadElement(pt.x, pt.y, &data);
   			text.Format("[ x= %03ld y= %03ld R= %04X G= %04X B= %04X ]", pt.x, pt.y, data.Red(), data.Green(), data.Blue());
         }
      }
		else if (format == SapFormatRGBP8 || format == SapFormatLABP8 || format == SapFormatRGBP16 || format == SapFormatLABP16)
      {

         SapData data;
         m_pView->GetBuffer()->ReadElement(pt.x, pt.y, &data);
         SapDataRGB dataRGB = data;

         char fmtStr8[] = "[ x= %03ld y= %03ld Value= %03d ]";
         char fmtStr16[] = "[ x= %03ld y= %03ld Value= %04X ]";
         char* pFmtStr;

         if (format == SapFormatRGBP8 || format == SapFormatLABP8)
            pFmtStr = fmtStr8;
         else 
            pFmtStr = fmtStr16;

         int page;
         m_pView->GetBuffer()->GetParameter(CORBUFFER_PRM_PAGE,&page);

         if(page==0)
            text.Format(pFmtStr, pt.x, pt.y, dataRGB.Red());
         else if(page==1)
            text.Format(pFmtStr, pt.x, pt.y, dataRGB.Green());
         else if(page==2)
            text.Format(pFmtStr, pt.x, pt.y, dataRGB.Blue());
      }
      else
      {
         SapDataMono data;
         m_pView->GetBuffer()->ReadElement(pt.x, pt.y, &data);
			text.Format("[ x= %03ld y= %03ld Value= %04X ]", pt.x, pt.y, data.Mono());
      }

		// Append string to application title
		str = "  " + CString(text);
	}

	return str;
}


//
// CSapRectTracker implementation
//
void CSapRectTracker::AdjustRect( int nHandle, LPRECT lpRect )
{
   // We limit the tracker's limits to be 1 pixel smaller than m_LimitRect on
   // each side, otherwise the tracker would be viewed beyond CImageWnd's view 
   // rect, and CImageWnd::Invalidate would not erase the tracker.

   if(lpRect->bottom <= 1)
      lpRect->bottom = 1;

   if(lpRect->top <= 1)
      lpRect->top = 1;

   if(lpRect->left <= 1)
      lpRect->left = 1;

   if(lpRect->right <= 1)
      lpRect->right = 1;

   if(lpRect->bottom >= (m_LimitRect.bottom - 1))
      lpRect->bottom = m_LimitRect.bottom - 1;

   if(lpRect->top  >= (m_LimitRect.bottom - 1))
      lpRect->top = m_LimitRect.bottom - 1;

   if(lpRect->left >= (m_LimitRect.right - 1))
      lpRect->left = m_LimitRect.right - 1;

   if(lpRect->right >= (m_LimitRect.right - 1))
      lpRect->right = m_LimitRect.right - 1;
}

void CSapRectTracker::SetLimitRect(CRect *rect)
{
   m_LimitRect.top   = rect->top;
   m_LimitRect.bottom= rect->bottom;
   m_LimitRect.left  = rect->left;
   m_LimitRect.right = rect->right;
}
