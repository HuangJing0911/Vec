
#ifndef XDDRAW_H
#define XDDRAW_H
#include "..\Include\ddraw.h"
#include <stdint.h>

#ifdef _WIN64
#pragma comment(lib, "..\\Lib\\ddraw64.lib")
#else
#pragma comment(lib, "..\\Lib\\ddraw.lib")
#endif

class XDDraw
{
public:
	XDDraw(void);
	~XDDraw(void);

	
	bool Initialize(HWND hwnd, uint32_t width, uint32_t height);
	void Release();
	bool GetIsInitial();
	void CopyImageToBack(uint32_t width, uint32_t height, CONST VOID* data_, CONST BITMAPINFO * bmi_);
	void DisplayPrimary();
	void SetDispRect(RECT rect);

private:
	XDDraw(const XDDraw&);
	XDDraw& operator = (const XDDraw&);
	LPDIRECTDRAW            _ddraw_;
	LPDIRECTDRAWSURFACE     _dprimary_;
	LPDIRECTDRAWSURFACE     _dback_;
	LPDIRECTDRAWPALETTE     _dpalette_;
	LPDIRECTDRAWCLIPPER		_dclipper_;
		
	bool _is_initial;
	RECT _dis_rect;
};

#endif //XDDRAW_H
