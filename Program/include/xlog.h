/*
Copyright (c) 2014, Detection Technology Inc.
All rights reserved.

This file deines XLog class.

Author: Zhang Xu, 2014-3-17
 */
#ifndef XLOG_H
#define XLOG_H
#include"xfile_log.h" 

#define XLOG_ERROR   0
#define XLOG_WARNING 1
#define XLOG_INFO    2

#define XLOG_TIME_FORMAT "%04u-%02u-%02u %02u:%02u:%02u ms:%03u"
#define XLOG_STRING_FORMAT "Time: %s Level: %-12s Message: %s\n"
/*
XLog class, format the log info.
 */
class XLog
{
public:
     XLog();
     ~XLog();
	 void Enable(bool enable);
     bool Open(const std::string file_name = "xlog.dat");
     void SetAutoFlush(bool enable_flush);
     std::string Format(std::string format, ...);
     void LogString(int32_t log_level, const std::string& message);
     static XLog* Instance();
     
private:
     bool _is_open;
     bool _auto_flush;
	 bool _is_enable;
     XFileLog _file_log;
     std::string _log_header[3];
     static XLog* _log_obj_; 

     std::string GetFormatTime();
    
};
//XLog log_obj;
#endif //XLOG_H
