#include "StdAfx.h"
#include "xcam_fg.h"
#include "xcamimg_engine.h"


#define PIXEL_DEPTH_S	16
#define PIXEL_DEPTH_L	18
#define CONF_FILE_S		".\\Conf_File_S.ccf"
#define CONF_FILE_L		".\\Conf_File_L.ccf"
XCamFG::XCamFG(XCamImgEngine*  engine_)
	:_sap_loc_(NULL)
	,_sap_acq_(NULL)
	,_sap_buf_(NULL)
	,_sap_transfer_(NULL)
	,_img_engine_(engine_)
	,_image_width(0)
	,_image_height(0)
	,_pixel_depth(0)
	,_pixel_bytes(0)
	,_is_open(0)
	,_conf_file_(NULL)
	,_raw_data_(NULL)
	,_data_size(0)
	
{
}


XCamFG::~XCamFG(void)
{
	if(_is_open)
		Close();
}

void XCamFG::XferCallback(SapXferCallbackInfo* info_)
{
	XCamFG* cam_fg_ = (XCamFG*) info_->GetContext();
	void* data_;
	cam_fg_->_sap_buf_->GetAddress(&data_);
	cam_fg_->Process((uint8_t*)data_);

}
void XCamFG::Process(uint8_t* data_)
{
	TRACE("%X\n",data_);
	if(_pixel_depth == PIXEL_DEPTH_S)
	{
		_img_engine_->GrabThreadMember(data_, _data_size, 1);
	}
	else
	{
		//Change RGB8888 to RGB888
		uint32_t line_size_3 = _image_width*3;
		uint32_t line_size_4 = _image_width*4;
		for(uint32_t i = 0; i<_image_height; i++)
			for(uint32_t j=0; j<_image_width; j++)
			{
				memcpy(_raw_data_+i*line_size_3+ j*3, data_+i*line_size_4+j*4, 3);
			}
		_img_engine_->GrabThreadMember(_raw_data_, _data_size, 1);
	}
}
#define BUF_NUM	256

bool XCamFG::Open()
{
	if(_is_open)
		return _is_open;

	switch(_pixel_depth)
	{
	case PIXEL_DEPTH_S:
		_sap_loc_ = new SapLocation("Xcelera-CL_PX4_1", 0);
		break;
	case PIXEL_DEPTH_L:
		_sap_loc_ = new SapLocation("Xcelera-CL_PX4_1", 2);
		break;
	default:
		_sap_loc_ = new SapLocation("Xcelera-CL_PX4_1", 2);
		break;
	}
	if(!_sap_loc_)
	{
		_is_open = 0;
		return _is_open;
	}
	//switch(_pixel_depth)
	//{
	//case PIXEL_DEPTH_S:
	//	_sap_acq_ = new SapAcquisition(*_sap_loc_, CONF_FILE_S);
	//	break;
	//case PIXEL_DEPTH_L:
	//	_sap_acq_ = new SapAcquisition(*_sap_loc_,CONF_FILE_L);
	//	break;
	//}
	_sap_acq_ = new SapAcquisition(*_sap_loc_, _conf_file_);
	//BOOL ret = _sap_acq_->Create();
	if(_sap_acq_ && !*_sap_acq_ && !_sap_acq_->Create())
	{
		Close();
		_is_open = 0;
		return _is_open;
	}
	_sap_buf_ = new SapBuffer(BUF_NUM, _sap_acq_);
	//ret = _sap_buf_->Create();
	if(_sap_buf_ && !*_sap_buf_ && !_sap_buf_->Create())
	{
		Close();
		_is_open = 0;
		return _is_open;
	}
	_sap_transfer_ = new SapAcqToBuf(_sap_acq_, _sap_buf_, XferCallback, this);
	if(_sap_transfer_ && !*_sap_transfer_ && !_sap_transfer_->Create())
	{
		Close();
		_is_open = 0;
		return _is_open;
	}

	_data_size = _image_width * _image_height * _pixel_bytes;
	if(3 == _pixel_bytes)
	{
		_raw_data_ = new uint8_t[_data_size];
		if(NULL == _raw_data_)
		{
			Close();
			_is_open = 0;
			return _is_open;
		}
	}
	_is_open = 1;
	return _is_open;
}

void XCamFG::Close()
{
	if(_sap_transfer_ && *_sap_transfer_)
		_sap_transfer_->Destroy();
	if(_sap_buf_ && *_sap_buf_)
		_sap_buf_->Destroy();
	if(_sap_acq_ && *_sap_acq_)
		_sap_acq_->Destroy();

	if(_sap_transfer_)
	{
		delete _sap_transfer_;
		_sap_transfer_ = NULL;
	}
	if(_sap_buf_)
	{
		delete _sap_buf_;
		_sap_buf_ = NULL;
	}
	if(_sap_acq_)
	{
		delete _sap_acq_;
		_sap_acq_ = NULL;
	}
	if(_raw_data_)
		delete [] _raw_data_;
	_is_open = 0;
}

bool XCamFG::Grab()
{
	if(_is_open)
		return _sap_transfer_->Grab();
	else
		return 0;
}
bool XCamFG::Stop()
{
	if(_is_open)
	{
		_sap_transfer_->Freeze();
		return (bool)_sap_transfer_->Wait(5000);
	}
	else
		return 0;
}
void XCamFG::SetPixelDepth(uint32_t depth)
{
	_pixel_depth = depth;
	switch(_pixel_depth)
	{
	case PIXEL_DEPTH_S:
		_conf_file_ = CONF_FILE_S;
		_pixel_bytes = 2;
		break;
	case PIXEL_DEPTH_L:
		_conf_file_ = CONF_FILE_L;
		_pixel_bytes = 3;
		break;
	default:
		_conf_file_ = CONF_FILE_L;
		_pixel_bytes = 3;
		break;
	}
}
void XCamFG::SetFrameWidth(uint32_t width)
{
	_image_width = width;
	CString str;
	str.Format("%d",_image_width);

	WritePrivateProfileString("Stream Conditioning","Crop Width", str,_conf_file_);
	WritePrivateProfileString("Stream Conditioning","Scale Horizontal", str,_conf_file_);
	WritePrivateProfileString("Signal Timings","Horizontal Active", str,_conf_file_);
}

void XCamFG::SetFrameHeight(uint32_t height)
{
	_image_height = height;
	CString str;
	str.Format("%d", _image_height);

	WritePrivateProfileString("Stream Conditioning","Crop Height", str,_conf_file_);
	WritePrivateProfileString("Stream Conditioning","Scale Vertical", str,_conf_file_);
	WritePrivateProfileString("Signal Timings","Vertical Active", str,_conf_file_);
}
void XCamFG::SetHorizonOffset(uint32_t offset)
{
	CString str;
	str.Format("%d", offset);
	WritePrivateProfileString("Signal Timings","Horizontal Back Invalid", str,_conf_file_);
}