/*
  Copyright (c) 2014, Detection Technology Inc.
  All right reserved.

  Author: Zhang Xu, 2014-4-14
 */

#ifndef IXCMD_ENGINE_H
#define IXCMD_ENGINE_H
#include "xdevice.h"
#include "xconfigure.h"
/*
  Interface of command engine.
 */
class IXCmdSink;
class XDLL_EXPORT IXCmdEngine
{
public:
     IXCmdEngine();
     virtual ~IXCmdEngine();

     virtual bool Open(XDevice* device_) = 0;
     virtual void Close() = 0;
     virtual int32_t SendCommand(uint8_t cmd_code, uint8_t operation,
				 uint8_t dm_id, uint16_t data_size,
				 uint8_t* send_data_, uint8_t* recv_data_,uint8_t& err_code) = 0;
     virtual uint32_t GetLastError() = 0;
     virtual void SetTimeout(uint32_t timeout) = 0;
     virtual void SetCmdSink(IXCmdSink* cmd_sink_) = 0;
	 virtual int32_t RecvHeartbeat(uint8_t* recv_data_, int32_t len) = 0;
};
#endif //IXCMD_ENGINE_H
