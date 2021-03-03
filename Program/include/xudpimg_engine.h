/*
  Copyright (c) 2014, Detection Technology Inc.
  All right reserved.

  Author: Zhang Xu, 2014-4-24
 */
#ifndef XUDPIMG_ENGINE_H
#define XUDPIMG_ENGINE_H
#include "iximg_engine.h"
#include "iximg_sink.h"
#include "xpacket_pool.h"
#include "xcommand.h"
#include "xacquisition.h"
#include <time.h>
/*
  XUDPImgEngine is responsible for grabbing image data packet and putting the
  received to memory pool.
 */

class XUDPImgEngine : public IXImgEngine
{
public:
     explicit XUDPImgEngine(uint32_t timeout,
			    IXImgSink* img_sink_ = NULL);
     XUDPImgEngine();
     ~XUDPImgEngine();

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
	 void SetFrameTransfer(IXTransfer*){};
	 uint32_t GetGrabTimeDiff();
private:
     XUDPImgEngine(const XUDPImgEngine&);
     XUDPImgEngine& operator = (const XUDPImgEngine&);
     static XTHREAD_CALL GrabThread(void* arg);
     uint32_t GrabThreadMember();
     
     bool _is_open;
     bool _is_running;
     uint32_t _timeout;
     uint32_t _last_err;
     XUDPSocket _udp_sock;
     IXImgSink* _img_sink_;
     XPacketPool* _packet_pool_;
     XCommand* _cmd_handle_;
     XThread _grab_thread;
     XAcquisition* _acquisition_;
	 clock_t _start_thread;
	 clock_t _start_grab;
};
#endif //XUDPIMG_ENGINE_H



  
