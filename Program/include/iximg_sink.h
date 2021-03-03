/*
  Copyright (c) 2014, Detection Technology Inc.
  All rights reserved.
 */

#ifndef IXIMG_SINK_H
#define IXIMG_SINK_H
#include<stdint.h>
#include "ximage.h"
class IXImgSink
{
public:
     virtual void OnXError(uint32_t err_id, const char* err_msg_) = 0;
     virtual void OnXEvent(uint32_t event_id, uint32_t data) = 0;
     virtual void OnFrameReady(XImage* image_) = 0;
     //virtual void OnSubFrameReady() = 0;
};
#endif //IXIMG_SINK_H
