/*
  Copyright (c) 2014, Detection Technology Inc.
  All rights reserved.

  This provid the ASCII command parsing functions.

  Author: Zhang Xu, 2014-3-24
 */

#ifndef XASC_PARSE_H
#define XASC_PARSE_H
#include "xconfigure.h"
#include "ixcmd_sink.h"
#include <map>
/*
  XASCParse class is responssible for the exchang of ASCII format command
  to hex format command.
  The ASCII format is as "[ST,W,00,3E8]", [CMD, OPE, DM, DATA]
  It uses a map to do the command exchange.
 */

class XASCParse
{
public:
     explicit XASCParse(IXCmdSink* cmd_sink_=NULL);
     virtual ~XASCParse();

     bool SendParse(std::string asc_cmd);
     std::string RecvParse(int32_t result, const uint8_t* recv_data_, uint8_t err_code);

     uint32_t GetLastError(){return _last_err;};
	 void SetCmdSink(IXCmdSink* cmd_sink_){_cmd_sink_ = cmd_sink_;};
     //Command sections
     uint8_t _cmd_code;
     uint8_t _operation;
     uint8_t _dm_id;
     uint8_t _data_size;
     uint64_t _send_data;

     //Command map, eg. map["ST,W"] = 0x02010002
     std::map<std::string, uint32_t> _cmd_map;

     uint32_t _last_err;
private:
     XASCParse(const XASCParse&);
     XASCParse& operator = (const XASCParse&);
     //uint8_t GetData(const char* asc_cmd_, int32_t comma_index);
     uint64_t GetData(const char* asc_cmd_);
     uint8_t GetDMID(const char* asc_cmd_);
     std::string GetCMD(const char* asc_cmd_);
     virtual void InitialCmdMap();
     std::string CleanCmd(std::string asc_cmd);

     IXCmdSink* _cmd_sink_;
};
#endif // XASC_PARSE_H
