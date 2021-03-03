/*
  Copyright (c) 2014, Detection Technology Inc.
  All right reserved.
 */

#ifndef XACQUISITION_H
#define XACQUISITION_H

#include "ixacquisition.h"
#include "xconfigure.h"
#include "xcommand.h"
class IXImgEngine;
class IXImgParse;
class XDevice;
class XPacketPool;
class IXFactory;
//class XFrameTransfer;
class IXTransfer;
class IXImgSink;

class XDLL_EXPORT XAcquisition : public IXAcquisition
{
public:
     explicit XAcquisition(IXFactory* factory_, uint32_t timeout = XIMG_TIMEOUT);
	 XAcquisition();
     ~XAcquisition();

     bool Open(XDevice* dev_, XCommand* cmd_handle_);
     void Close();
     void RegisterEventSink(IXImgSink* img_sink_);
     bool Grab(uint32_t frame_num);
     bool Stop();
     bool Snap();
     void RegisterFrameTransfer(IXTransfer* transfer_);
     void OnError();
     void OnFrameComplete();
     void EnableLineInfo(bool enable);
     uint32_t GetLastError();
     void SetFactory(IXFactory* factory_);
     void SetTimeout(uint32_t time);
     bool GetIsOpen();
     bool GetIsGrabbing();
	 XCommand* GetCmdHandle();
	 uint32_t GetGrabTimeDiff();
	// void SetExFrame(bool enable);
	// bool GetExFrame();
	 //void ResetExFrame();
private:
     XAcquisition(const XAcquisition&);
     XAcquisition& operator = (const XAcquisition&); 
     bool Allocate();
     void Release();
     
     bool _is_open;
     bool _is_grabbing;
     uint32_t _last_err;
     uint32_t _timeout;
     bool _enline_info;
	 bool _exframe;

     IXImgSink* _img_sink_;
     IXImgEngine* _img_engine_;
     IXImgParse*  _img_parse_;
     XPacketPool* _pac_pool_;
     IXTransfer* _frame_transfer_;
     IXFactory*   _factory_;
	 XCommand*	 _cmd_handle_;
     XEvent _frame_complete;
};

#endif //XACQUISITION_H
