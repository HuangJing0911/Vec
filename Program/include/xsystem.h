/*
  Copyright (c) 2014, Detection Technology Inc.
  All rights reserved.
 */

#ifndef XSYSTEM_H
#define XSYSTEM_H

#include "ixcmd_sink.h"
#include "xconfigure.h"


class XBroadEngine;
//class XASCParse;
class IXCmdSink;
class XDevice;
class XDeviceList;
/*
  XSystem class communicates with detector by broadcast. It provides find
  device function to get the basic parameters from detector.
 */

class XDLL_EXPORT XSystem
{
public:
     //XSystem(std::string local_ip, uint16_t local_port = 0);
    explicit XSystem(const char* local_ip_, uint16_t local_port = 0,
	     uint32_t timeout = XSYS_TIMEOUT);
	XSystem();

     ~XSystem();

     bool Open();
     void Close();
     int32_t ConfigureDevice(const XDevice* dev_);
     int32_t FindDevice();
     XDevice* GetDevice(uint32_t device_index);
     void RegisterEventSink(IXCmdSink* cmd_sink_);
     int32_t RecoverDevice();
     //int32_t SendAscCmd(std::string asc_send, std::string& asc_recv);
     uint32_t GetLastError();
     void SetLocalIP(const char* ip_addr_);
     void SetTimeout(uint32_t time);
     bool GetIsOpen();
     size_t GetDeviceNum();
	 void EnableLog(bool enable);
private:
     XSystem(const XSystem&);
     XSystem& operator=(const XSystem&);
     bool TestConnection();
     bool Allocate();
     void Release();
     void CleanDevList();
     char _local_ip[20];
     uint16_t _local_port;
     uint32_t _timeout;
     
     bool _is_open;
     uint32_t _last_err;
     uint32_t _device_num;

	 XDeviceList* _dev_list_;

     XBroadEngine* _broad_engine_;
     //XASCParse* _asc_parse_;
     IXCmdSink* _cmd_sink_;

};

#endif //XSYSTEM_H
