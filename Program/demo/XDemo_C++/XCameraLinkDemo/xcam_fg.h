#pragma once
#include "IncCor\Basic\SapClassBasic.h"
#include <stdint.h>
#ifdef _WIN64
#pragma comment(lib, "LibCor64\\SapClassBasic.lib")
#else
#pragma comment(lib, "LibCor32\\SapClassBasic.lib")
#endif

class XCamImgEngine;

class XCamFG
{
public:
	XCamFG(XCamImgEngine* engine_);
	~XCamFG(void);


private:
	SapLocation*	_sap_loc_;
	SapAcquisition* _sap_acq_;
	SapBuffer*		_sap_buf_;
	SapTransfer*	_sap_transfer_;
	XCamImgEngine*	_img_engine_;
	uint32_t		_image_width;
	uint32_t		_image_height;
	uint32_t		_pixel_depth;
	uint32_t		_pixel_bytes;
	bool			_is_open;
	char*			_conf_file_;
	uint8_t*		_raw_data_;		//RGB888
	uint32_t		_data_size;

public:
	static void XferCallback(SapXferCallbackInfo* info_);
	bool Open();
	void Close();
	bool Grab();
	bool Stop();
	void SetPixelDepth(uint32_t depth);
	void SetFrameWidth(uint32_t width);
	void SetFrameHeight(uint32_t height);
	void SetHorizonOffset(uint32_t offset);
	void Process(uint8_t* data_);
	
};

