/*
  Copyright (c) 2014, Detection Technology Inc.
  All rights reserved.

  This provide the basic broadcast command method.

  Author: Zhang Xu, 2014-3-19
 */

#ifndef XBROAD_ENGINE_H
#define XBROAD_ENGINE_H
#include "xconfigure.h"
#include "xcrc.h"
#include "ixcmd_sink.h"
/*
  XBroadEngine class is responssible for the communication
  with detector's port 7001. It provids the CRC checking,
  command sending and command receiving functions. 
 */
#define XBROAD_PEER_PORT 7000
#define XBROAD_PEER_IP "255.255.255.255"
class XBroadEngine
{
public:
     explicit XBroadEngine(uint32_t timeout=XCMD_TIMEOUT,
			   IXCmdSink* cmd_sink_=NULL);
     ~XBroadEngine();

     bool Open(std::string local_ip, uint16_t local_port = 0);
     void Close();
     int32_t SendCommand(uint8_t cmd_code, uint8_t operation, uint8_t dm_id,
	              uint8_t data_size, uint8_t* send_data_,
		      uint8_t* recv_data_);
     uint32_t GetLastError();
     int32_t SendToMulti(uint8_t cmd_code, uint8_t operation, uint8_t dm_id,
			 uint8_t data_size, uint8_t* send_data_,
			 uint8_t* recv_data_);
     int32_t GetMultiNum();
private:
     XBroadEngine(const XBroadEngine&);
     XBroadEngine& operator = (const XBroadEngine&);

     //GenerateCmd();
     int32_t ParseRecv(uint8_t* recv_data_);
     int32_t ParseMultiRecv(uint8_t* recv_data_);
     bool _is_open;
     uint32_t _timeout;
     uint32_t _last_err;
     //For multi receive
     int32_t _multi_recv_len; //Multi-received total data length
     int32_t _multi_recv_num; //Multi-received total packet number
     
     XFastCrc _crc_check;
     XUDPSocket _broad_sock;
     IXCmdSink* _cmd_sink_;
     uint8_t _send_buf[XCMD_BUF_SIZE];
     uint8_t _recv_buf[XCMD_BUF_SIZE];
};

#endif //XBROAD_ENGINE_H
