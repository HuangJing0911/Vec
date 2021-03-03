#include "stdafx.h"
#include "xcamimg_engine.h"

XCamImgEngine::XCamImgEngine(uint32_t timeout,
			     IXImgSink* img_sink_)
    :_is_open(0)
    ,_is_running(0)
    ,_timeout(timeout)
    ,_last_err(0)
	,_line_num(0)
    ,_img_sink_(img_sink_)
    ,_cmd_handle_(NULL)
    ,_acquisition_(NULL)
	,_frame_grabber(this)
	,_pixel_number(0)
	,_pixel_depth(0)
{

}

XCamImgEngine::XCamImgEngine(bool enline_info)
    :_is_open(0)
    ,_is_running(0)
    ,_en_line_info(enline_info)
    ,_timeout(XIMG_TIMEOUT)
    ,_last_err(0)
	,_line_num(0)
    ,_img_sink_(NULL)
    ,_cmd_handle_(NULL)
    ,_acquisition_(NULL)
	,_frame_grabber(this)
	,_pixel_number(0)
	,_pixel_depth(0)
{
    
}

XCamImgEngine::~XCamImgEngine()
{
    if(_is_open)
	Close();
}

#define FG_LINE_NUM			1	//Frame height of frame grabber output
#define FG_EXTRA_PIXEL16	6	//Info header(12 bytes) counts for 6 pixels for 16-bit pixel
#define FG_EXTRA_PIXEL18	4   //Info header counts for 4 pixels for 18-bit pixel
#define FG_INFO_BYTE		12  //Info header is 12 bytes long
#define FG_HORIZON_OFFSET   2   
bool XCamImgEngine:: Open(XDevice* device_)
{
	if(_is_open)
		return _is_open;
    try
    {
		_pixel_number = device_->GetPixelNumber();
		_pixel_depth = device_->GetPixelDepth();
		_frame_grabber.SetPixelDepth(_pixel_depth);
		uint32_t fg_pixel_num;
		if(18==_pixel_depth)
		{
			fg_pixel_num = _pixel_number+FG_EXTRA_PIXEL18;
			//if(fg_pixel_num%16)
			//	fg_pixel_num += 16-fg_pixel_num%16;
	
			_frame_grabber.SetFrameWidth(fg_pixel_num);
			_line_size = fg_pixel_num*3;
		}
		else
		{
			fg_pixel_num = _pixel_number+FG_EXTRA_PIXEL16;
			if(fg_pixel_num%16)
				fg_pixel_num += 16-fg_pixel_num%16;
			_frame_grabber.SetFrameWidth(fg_pixel_num);			
			_line_size = fg_pixel_num*2;
			//_frame_grabber.SetHorizonOffset(FG_HORIZON_OFFSET);
		}
		
		_frame_grabber.SetFrameHeight(FG_LINE_NUM);
					
        if(!_frame_grabber.Open())
			throw XException(XERROR_IMG_SOCK_OPEN_FAIL);
    }
    catch(XException exp)
    {
	_is_open = 0;
	_last_err = exp._error_code;
	if(_img_sink_)
	    _img_sink_->OnXError(exp._error_code, exp._error_msg.c_str());
	return _is_open;	
    }
    _is_open = 1;
    return _is_open;
}

void XCamImgEngine::Close()
{
    if(_is_open)
    {
		if(GetIsRunning())
			Stop();
		_frame_grabber.Close();
		_is_open = 0;
    }
}

bool XCamImgEngine::GetIsRunning()
{
	return _is_running;
}

bool XCamImgEngine::Start()
{
    if(GetIsRunning())
    	return 1;
    _line_num = 0;
	try
    {
	if(!_is_open)
	    throw XException(XERROR_IMG_ENGINE_NOT_OPEN );
	if(!_frame_grabber.Grab())
	    throw XException(XERROR_IMG_ENGINE_START_FAIL);
    }
    catch(XException exp)
    {
	_last_err = exp._error_code;
	if(_img_sink_)
	    _img_sink_->OnXError(exp._error_code, exp._error_msg.c_str());
	return 0;
    }
	//Sleep(1000);
	_cmd_handle_->SetPara(XPARA_EN_SCAN, 1);
    _is_running = 1;
    return 1;
}
/*
  Stop grab thread. If the thread was forced to terminate, return 0.
  In that case, it can't start again.
 */
bool XCamImgEngine::Stop()
{
    try
    {
	if(!_is_open)
	    throw XException(XERROR_IMG_ENGINE_NOT_OPEN );

	_frame_grabber.Stop();
	_cmd_handle_->SetPara(XPARA_EN_SCAN, 0);
	_is_running = 0;
    }
    catch(XException exp)
    {
	_last_err = exp._error_code;
	if(_img_sink_)
	    _img_sink_->OnXError(exp._error_code, exp._error_msg.c_str());
	return 0;
    }
    return 1;
}

void XCamImgEngine:: AttachObserver(XAcquisition* acq_)
{
    _acquisition_ = acq_;
}
uint32_t XCamImgEngine::GetLastError()
{
    return _last_err;
}

void XCamImgEngine:: SetTimeout(uint32_t timeout)
{
    _timeout = timeout;
}

void XCamImgEngine::SetImgSink(IXImgSink* img_sink_)
{
    _img_sink_ = img_sink_;
}

void XCamImgEngine:: SetPacketPool(XPacketPool* packet_pool_)
{
}
void XCamImgEngine::SetCmdHandle(XCommand* cmd_handle_)
{
    _cmd_handle_ = cmd_handle_;
}
void XCamImgEngine::SetFrameTransfer(IXTransfer* transfer_)
{
	_frame_transfer_ = transfer_;
}
#define LINE_NUM_POS	4
#define LINE_PIXEL_POS  12
uint32_t XCamImgEngine::GrabThreadMember(uint8_t *data_, uint32_t size, bool err)
{
	try
	{
		if(!err)
			throw(XException(XERROR_IMG_ENGINE_GRAB_ABNORMAL));
		
		if(16 == _pixel_depth)
		{
			
			//uint32_t line_size = FG_EXTRA_PIXEL16+FG_EXTRA;
			for(uint32_t i=0; i<FG_LINE_NUM; i++)
			{
				_line_num++;
				//Covert pixel byte order
				for(uint32_t j=LINE_PIXEL_POS; j<_line_size; j+=2)
				{
					uint8_t temp;
					temp = data_[_line_size*i +j];
					data_[_line_size*i+j] = data_[_line_size*i+j+1];
					data_[_line_size*i+j+1] = temp;
				}
				
				uint16_t line_id = *(uint16_t*)(data_ +_line_size*i+ LINE_NUM_POS);
				//TRACE("LINE ID: %d\n", line_id);
				uint16_t lost_line = 0;
				if(_line_num != line_id)
				{
					
					if(_line_num < line_id)
						lost_line = line_id - _line_num;
					else
						lost_line = line_id - _line_num + MAX_LINE_NUM;
	    
					if(_img_sink_)
						 _img_sink_->OnXEvent(XEVENT_IMG_PARSE_DATA_LOST, lost_line);
					_line_num = line_id;
				}
				//Copy line data with info header to frame transfer
                if(_en_line_info)
				_frame_transfer_->PutLine(data_+_line_size*i+LINE_NUM_POS, _line_size-LINE_NUM_POS, XIMG_DATA_OFFSET);
                else
				//Copy line data without info header to frame transfer
                _frame_transfer_->PutLine(data_+_line_size*i + 12, _line_size-12);
				
			}
		}
		else
		{
			
			for(uint32_t i=0; i<FG_LINE_NUM; i++)
			{
				_line_num++;
				
				uint16_t line_id = ntohs(*(uint16_t*)(data_ +_line_size*i+ LINE_NUM_POS));
				uint16_t lost_line = 0;
				if(_line_num != line_id)
				{
//					TRACE("Cur line id %d, expect id %d\n", line_id, _line_num);
					if(_line_num < line_id)
						lost_line = line_id - _line_num;
					else
						lost_line = line_id - _line_num + MAX_LINE_NUM;

					if(_img_sink_)
						_img_sink_->OnXEvent(XEVENT_IMG_PARSE_DATA_LOST, lost_line);
					_line_num = line_id;
				}
				*(uint16_t*)(data_ +_line_size*i+ LINE_NUM_POS) = line_id;
                if(_en_line_info)
                //Copy line data with info header to frame transfer
				_frame_transfer_->PutLine(data_+_line_size*i+LINE_NUM_POS, _line_size-LINE_NUM_POS, XIMG_DATA_OFFSET);
                else
				//Copy line data without info header to frame transfer
                _frame_transfer_->PutLine(data_+_line_size*i + 12, _line_size-12);

			}


		}
	}

    catch(XException exp)
    {
	_last_err = exp._error_code;
	if(_img_sink_)
	    _img_sink_->OnXError(exp._error_code, exp._error_msg.c_str());
	return 0;
    }
    return 1;
}
