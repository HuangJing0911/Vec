/*
Copyright (c) 2014, Detection Technology Inc.
All rights reserved.

This file defines file log class.

Author: Zhang Xu, 2014-3-17
 */

#ifndef XFILE_LOG_H
#define XFILE_LOG_H
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <stdlib.h>
#include <string.h>

#define XFILELOG_FILE_SIZE 1024*1024*10 //Maximum file size
#define XFILELOG_BUF_SIZE 1024*100      //Maximum buffer size

/*
File log class, save string to file buffer the to file.
 */
class XFileLog
{
public:
    explicit  XFileLog(int32_t buf_size = XFILELOG_BUF_SIZE)
	  :_log_file_(NULL)
	  ,_buffer_(NULL)
	  ,_buf_size(buf_size)
	  ,_buf_position(0)
     { };
     ~XFileLog()
     {
	  if(NULL != _log_file_)
	       fclose(_log_file_);
	  if(_buffer_)
	       free(_buffer_);
     };
     bool Open(const std::string& file_name)
     {
	  if(_log_file_)
	       return 1;
	  
	  _log_file_ = fopen(file_name.c_str(), "a");
	  if(NULL == _log_file_)
	  {
	       //Failed, try again
	       _log_file_ = fopen(file_name.c_str(), "a");
	       if(NULL == _log_file_)
		    return 0;
	  }
	  
	  //Caculate the file size
	  fseek(_log_file_, 0, SEEK_END);
	  int32_t file_size = ftell(_log_file_);
	  //If file size is bigger than maximum file size, clear an reopen file
	  if(XFILELOG_FILE_SIZE < file_size)
	  {
	       fclose(_log_file_);
	       _log_file_ = fopen(file_name.c_str(), "w");
	       if(NULL == _log_file_)
	       {
		    //Failed, try again
		    _log_file_ = fopen(file_name.c_str(), "w");
		    if(NULL == _log_file_)
			 return 0;
	       }
	  }
	  //Allocate buffer
	  _buffer_ = (uint8_t*) malloc(XFILELOG_BUF_SIZE);
	  if(NULL == _buffer_)
	  {
	       //Try again
	       _buffer_ = (uint8_t*) malloc(XFILELOG_BUF_SIZE);
	       if(NULL == _buffer_)
		    return 0;
	  }
	  return 1;
     };
     /*
       Save data from buffer to file.
      */
     void FlushData()
     {
	  if(_buf_position > 0)
	       fwrite(_buffer_, sizeof(uint8_t), _buf_position, _log_file_);
	  fflush(_log_file_);
	  _buf_position = 0;
     };
     /*
       Save data to buffer, if overflow happens, it'll save data to file.
      */
     size_t WriteString(const std::string& message)
     {
	  size_t str_len = message.length();
	  if(str_len > _buf_size)
	       return fwrite(message.c_str(), 1, str_len, _log_file_);
	  if(str_len + _buf_position > _buf_size)
	  {
	       FlushData();
	       return fwrite(message.c_str(), 1, str_len, _log_file_);
	  }

	  memcpy(_buffer_+_buf_position, message.c_str(), str_len);
	  _buf_position += str_len;
	  return str_len;
     };
private:
     FILE* _log_file_;
     uint8_t* _buffer_;
     uint32_t _buf_size;
     size_t _buf_position;
     
};
#endif //XFILE_LOG_H
