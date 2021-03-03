/*
  Copyright (c), Detection Technology Inc.
  All right reserved.

  Author: Zhang Xu, 2014-5-5
 */

#ifndef XFRAME_TRANSFER_H
#define XFRAME_TRANSFER_H
#include "ixtransfer.h"
#include "xconfigure.h"
#include "xframe_pool.h"
#include "xacquisition.h"

class XFrameQueue;

class  XDLL_EXPORT XFrameTransfer : public IXTransfer
{
public:
     explicit XFrameTransfer(uint32_t line_num);
     XFrameTransfer();
     ~XFrameTransfer();
     friend class XAcquisition;
     
     uint32_t GetLastError();
     void RegisterEventSink(IXImgSink* img_sink_);
     bool GetIsRunning();
     XImage*  GetImage();
     uint32_t GetPixelNumber();
     uint32_t GetPixelByte();
     uint32_t GetLineNum();
     void     SetLineNum(uint32_t line_num);
private:
     bool Open(XDevice* dev_, uint32_t data_offset=0);
     void Close();
     bool Start(uint32_t target_num);
     bool Stop();
     void PushFrame(XImage* image_);
     void PutLine(uint8_t* data_, size_t size, uint32_t data_offset);
     void FrameReady();
     void AttachObserver(XAcquisition* acq_);
     void AttachMultiTransfer(XMultiTransfer* multi_transfer_,
			      uint32_t index);
	 void Reset();
private:
     XFrameTransfer(const XFrameTransfer&);
     XFrameTransfer& operator = (const XFrameTransfer&);
     static XTHREAD_CALL TransferThread(void* arg);
     uint32_t TransferThreadMember();
     void PutMultiFrame(uint8_t* data_, size_t size);
     
     bool _is_open;
     bool _is_running;
     uint32_t _last_err;
     uint32_t _line_num;
     IXImgSink* _img_sink_;
     XAcquisition* _acquisition_;
     XMultiTransfer* _multi_transfer_;
     XFrameQueue* _frame_que_;
     XLock _lock;
     XFramePool _frame_pool;

     XThread _transfer_thread;
     XEvent _frame_ready;
     uint32_t _target_num;
     uint32_t _multi_index;
     XImage*  _image_;
};
#endif //XFRAME_TRANSFER_H
