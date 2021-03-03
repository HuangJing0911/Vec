/*
  Copyright (c) 2014, Detection Technology Inc.
  All rights reserved.
 */
#ifndef IXCMD_SINK_H
#define IXCMD_SINK_H
//#include "xconfigure.h"
#include <stdint.h>
//Interface of command and system sink

class IXCmdSink
{
public:
     virtual void OnXError(uint32_t err_id, const char* err_msg_ ) = 0;
     virtual void OnXEvent(uint32_t event_id, float data) = 0;
};

#endif // IXCMD_SINK_H
