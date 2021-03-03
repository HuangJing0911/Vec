/*
  Copyright (c) 2014, Detection Technology Inc.
  All right reserved.

 */
#ifndef XCAMIMG_ENGINE_H
#define XCAMIMG_ENGINE_H
#include "iximg_engine.h"
#include "iximg_sink.h"
#include "xcommand.h"
#include "xacquisition.h"
//#include "FGImage.h"
#include "xcam_fg.h"
#include "ixtransfer.h"

/*
  XCamImgEngine is responsible for grabbing image data packet and putting the
  received to frame transfer object..
 */

class XCamImgEngine : public IXImgEngine
{
public:
     explicit XCamImgEngine(uint32_t timeout,
			    IXImgSink* img_sink_ = NULL);
     XCamImgEngine(bool enline_info);
     ~XCamImgEngine();
     bool Open(XDevice* device_);
     void Close();
     bool Start();
     bool Stop();

     uint32_t GetLastError();
     void SetTimeout(uint32_t timeout);
     void SetImgSink(IXImgSink* img_sink_);
     void SetCmdHandle(XCommand* cmd_handle_);
     void SetPacketPool(XPacketPool* packet_pool_);
     bool GetIsRunning();
     void AttachObserver(XAcquisition* acq_);
	 void SetFrameTransfer(IXTransfer* transfer_);
	 uint32_t GrabThreadMember(uint8_t *data_, uint32_t size, bool err);
	  uint32_t GetGrabTimeDiff() {return 0;}
private:
     XCamImgEngine(const XCamImgEngine&);
     XCamImgEngine& operator = (const XCamImgEngine&);
     
     bool _is_open;
     bool _is_running;
     bool _en_line_info;
     uint32_t _timeout;
     uint32_t _last_err;
	 uint16_t _line_num;
     IXImgSink* _img_sink_; 
     XCommand* _cmd_handle_;
     XAcquisition* _acquisition_;
	 IXTransfer* _frame_transfer_;
	 XCamFG		_frame_grabber;
	 uint32_t _pixel_number;
	 uint32_t _pixel_depth;
	 uint32_t _line_size;
};
#endif //XCAMIMG_ENGINE_H



  
