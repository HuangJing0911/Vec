/*
  Copyright (c) 2014, Detection Technology Inc.
  All right reserved.

  Author: Zhang Xu, 2014-4-28
 */

#ifndef IXIMG_PARSE_H
#define IXIMG_PARSE_H
#include "xconfigure.h"
//#include "xdevice.h"
class XDevice;
class IXImgSink;
class XPacketPool;
class IXTransfer;
class XDLL_EXPORT IXImgParse
{
public:
     IXImgParse();
     virtual ~IXImgParse();

     virtual bool Open(XDevice* dev_) = 0;
     virtual void Close() = 0;
     virtual bool Start() = 0;
     virtual bool Stop() = 0;
     virtual uint32_t GetLastError() = 0;
     virtual void SetImgSink(IXImgSink*) = 0;
     virtual void SetPacketPool(XPacketPool*) = 0;
     virtual bool GetIsRunning() = 0;
     virtual void SetFrameTransfer(IXTransfer*) = 0;
	 virtual void Reset() = 0;
};
#endif //IXIMG_PARSE_H
