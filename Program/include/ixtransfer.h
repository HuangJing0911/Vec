/*
  Copyright (c) 2014, Detection Technology Inc.
  All right reserved.

  Author: Zhang Xu, 2014-5-5
 */

#ifndef IXTRANSFER_H
#define IXTRANSFER_H
//#include "xdevice.h"
//#include "ximage.h"
#include <stdint.h>
#include "xconfigure.h"
#include "ximage.h"
#define XLINE_NUM 1024

class IXImgSink;
class XDevice;
class XAcquisition;
class XMultiTransfer;

class  XDLL_EXPORT IXTransfer
{
public:
     IXTransfer();
     virtual ~IXTransfer();

	 virtual uint32_t GetPixelNumber() = 0;
     virtual uint32_t GetPixelByte() = 0;
     virtual uint32_t GetLineNum() = 0;
	 virtual uint32_t GetLastError() = 0;
     virtual void RegisterEventSink(IXImgSink*) = 0;
	 virtual bool GetIsRunning() = 0;
	 virtual XImage*  GetImage() = 0;
	 virtual void SetLineNum(uint32_t line_num) = 0;

     virtual bool Open(XDevice*, uint32_t data_offset=0) = 0;
     virtual void Close() = 0;
     virtual bool Start(uint32_t target_num) = 0;
     virtual bool Stop() = 0;

     virtual void PushFrame(XImage* image_) = 0;
     virtual void PutLine(uint8_t* data_, size_t size,
			  uint32_t data_offset=0) = 0;
     virtual void FrameReady() = 0;
     
     virtual void AttachObserver(XAcquisition* acq_) = 0;
     virtual void AttachMultiTransfer(XMultiTransfer* multi_transfer_,
				      uint32_t index) = 0;
	 virtual void Reset() = 0;

};

#endif // IXTRANSFER_H
