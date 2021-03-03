#ifndef XDISPLAY_H
#define XDISPLAY_H

#include "xconfigure.h"
#include "ximage.h"
#include "XDevice.h"

#define XDISPLAY_COLOR_GRAY	0
#define XDISPLAY_COLOR_SIN	1
#define XDISPLAY_COLOR_COS	2
#define XDISPLAY_COLOR_HOT	3
#define XDISPLAY_COLOR_JET	4


class XDDraw;

struct XWindow
{
	float		gama;
	uint32_t	start;
	uint32_t	end;
	uint32_t	max;
};

class XDLL_EXPORT XDisplay
{
public:
	XDisplay(void);
	~XDisplay(void);

	bool Open(uint32_t width, uint32_t height, uint32_t pixel_depth, HWND hwnd, uint32_t color_mode = 0);
	bool Open(XDevice* dev_, uint32_t line_num, HWND hwnd, uint32_t color_mode = 0);
	void Close();
	bool GetIsOpen();
	bool GetIsDDrawEnable();
	uint32_t GetLastError();
	void SetGama(float gama_val);
	float GetGama();

	bool Display(XImage* image_);
	//void Display(uint8_t* data_);

private:
	XDisplay(const XDisplay&);
	XDisplay& operator = (const XDisplay&);

	void GetGrayImage();
	void RotateGrayImage();
	void DdrawDisplay();
	void AutoSetWindow();
	bool CreateBMI(uint32_t color_mode);
	bool CreateLUT();
	bool CreateGrayBuf();
	//uint32_t GetPixelValue(uint32_t row, uint32_t col);

	bool	_is_open;
	uint32_t _width;
	uint32_t _height;
	uint32_t _pixel_depth;
	uint32_t _pixel_byte;
	uint32_t _last_err;

	XImage*  _raw_data_;
	uint8_t* _gray_data_;
	uint8_t* _rot_gray_data_;
	uint8_t* _lut_;
	XDDraw*  _ddraw_;
	BITMAPINFO* _bmi_;
	
	HWND	   _hwnd;
	XWindow	   _window;
};

#endif //XDISPLAY_H

