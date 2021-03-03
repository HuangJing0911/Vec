/*
  Copyright (c) 2014, Detection Technology Inc.
  All right reserved.
 */

#ifndef XSERIALCMD_ENGINE_H
#define XSERIALCMD_ENGINE_H


#include "ixcmd_engine.h"
#include "xcrc.h"
#include "ixcmd_sink.h"
#include "Serial.h"


class XSerialCmdEngine : public IXCmdEngine
{
public:
     explicit XSerialCmdEngine(uint32_t timeout,
			    IXCmdSink* cmd_sink_ = NULL);
     XSerialCmdEngine();
     ~XSerialCmdEngine();

     bool Open(XDevice* device_);
     void Close();

     int32_t SendCommand(uint8_t cmd_code, uint8_t operation, uint8_t dm_id,
	              uint16_t data_size, uint8_t* send_data_,
		      uint8_t* recv_data_, uint8_t& err_code);
     uint32_t GetLastError();
     void SetTimeout(uint32_t timeout);
     void SetCmdSink(IXCmdSink* cmd_sink_);
	 int32_t RecvHeartbeat(uint8_t* recv_data_, int32_t len);
private:
     XSerialCmdEngine(const XSerialCmdEngine&);
     XSerialCmdEngine& operator = (const XSerialCmdEngine&);

     int32_t ParseRecv(uint8_t* recv_data_, uint8_t& err_code);
     bool _is_open;
     uint32_t _timeout;
     uint32_t _last_err;
     XFastCrc _crc_check;
     CSerial  _serial_sock;
     IXCmdSink* _cmd_sink_;
     uint8_t _send_buf[XCMD_BUF_SIZE];
     uint8_t _recv_buf[XCMD_BUF_SIZE];
};

#endif //XSERIALCMD_ENGINE_H
