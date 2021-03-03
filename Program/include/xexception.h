/*
Copyright (c) 2014, Detection Technology Inc.
All rights reserved.

In this file, all the exception cases are defined.

Author: Zhang Xu, 2014-3-7
*/

#ifndef XEXCEPTION_H
#define XEXCEPTION_H
#include <stdint.h>
/*
  Define error id, the software can't operate any more 
  if one the following error happens.
*/
#define XERROR_CODE				 0
#define XERROR_SYS_SOCK_OPEN_FAIL               XERROR_CODE + 1
#define XERROR_SYS_SOCK_BIND_FAIL               XERROR_CODE + 2
#define XERROR_SYS_SOCK_SEND_FAIL				XERROR_CODE + 3
#define XERROR_SYS_SOCK_RECV_TIMEOUT			XERROR_CODE + 4
#define XERROR_SYS_ENGINE_RECV_ERRCMD           XERROR_CODE + 5
#define XERROR_SYS_ENGINE_RECV_ERRCODE          XERROR_CODE + 6
#define XERROR_SYS_ENGINE_RECV_ERRCRC           XERROR_CODE + 7
#define XERROR_SYS_ENGINE_NOT_OPEN              XERROR_CODE + 8
#define XERROR_SYS_ALLOCATE_FAIL                XERROR_CODE + 9

#define XERROR_ASCPAS_FORMAT_ERR                XERROR_CODE + 10
#define XERROR_ASCPAS_NONE_CMD                  XERROR_CODE + 11

#define XERROR_CMD_SOCK_OPEN_FAIL               XERROR_CODE + 12
#define XERROR_CMD_SOCK_BIND_FAIL               XERROR_CODE + 13
#define XERROR_CMD_SOCK_SEND_FAIL               XERROR_CODE + 14
#define XERROR_CMD_SOCK_RECV_TIMEOUT            XERROR_CODE + 15
#define XERROR_CMD_ENGINE_RECV_ERRCMD           XERROR_CODE + 16
#define XERROR_CMD_ENGINE_RECV_ERRCODE          XERROR_CODE + 17
#define XERROR_CMD_ENGINE_RECV_ERRCRC           XERROR_CODE + 18
#define XERROR_CMD_ENGINE_NOT_OPEN              XERROR_CODE + 19
#define XERROR_CMD_ALLOCATE_FAIL                XERROR_CODE + 20

#define XERROR_IMG_SOCK_OPEN_FAIL               XERROR_CODE + 21
#define XERROR_IMG_SOCK_BIND_FAIL               XERROR_CODE + 22
#define XERROR_IMG_SOCK_RECV_TIMEOUT            XERROR_CODE + 23
#define XERROR_IMG_ALLOCATE_FAIL                XERROR_CODE + 24
#define XERROR_IMG_ENGINE_NOT_OPEN              XERROR_CODE + 25
#define XERROR_IMG_ENGINE_START_FAIL            XERROR_CODE + 26
#define XERROR_IMG_ENGINE_STOP_ABNORMAL         XERROR_CODE + 27
#define XERROR_IMG_PARSE_OPEN_FAIL              XERROR_CODE + 28
#define XERROR_IMG_PARSE_NOT_OPEN               XERROR_CODE + 29
#define XERROR_IMG_PARSE_START_FAIL             XERROR_CODE + 30
#define XERROR_IMG_PARSE_STOP_ABNORMAL          XERROR_CODE + 31
#define XERROR_IMG_TRANSFER_NOT_OPEN            XERROR_CODE + 32
#define XERROR_IMG_TRANSFER_START_FAIL          XERROR_CODE + 33
#define XERROR_IMG_TRANSFER_STOP_ABNORMAL       XERROR_CODE + 34
#define XERROR_IMG_PACKET_POOL_OPEN_FAIL        XERROR_CODE + 35

#define XERROR_MULTI_TRANS_ALLOCATE_FAIL        XERROR_CODE + 36
//#define XERROR_MULTI_TRANS_PIX_DEPTH_NOT_EQ   XERROR_CODE + 37
#define XERROR_MULTI_TRANS_NOT_OPEN             XERROR_CODE + 37

#define XERROR_DISP_ALLOCATE_FAIL               XERROR_CODE + 38
#define XERROR_CMD_HEARTBEAT_FAIL				XERROR_CODE + 39
#define XERROR_CMD_HEARTBEAT_START_FAIL			XERROR_CODE + 40
#define XERROR_CMD_HEARTBEAT_STOP_ABNORMAL		XERROR_CODE + 41
#define XERROR_CMD_HEARTBEAT_VOL_ERR			XERROR_CODE + 42
#define XERROR_IMG_ENGINE_GRAB_ABNORMAL			XERROR_CODE + 43

#define XERROR_FILE_OPERATE_ERROR              XERROR_CODE + 44
#define XERROR_FILE_TIF_TAG_ENTRY_ABNORMAL     XERROR_CODE + 45
#define XERROR_FILE_TIF_COMPRESSED             XERROR_CODE + 46
#define XERROR_FILE_TIF_ALLOC_FAIL             XERROR_CODE + 47
#define XERROR_MULTI_TRANS_START_FAIL		   XERROR_CODE + 48
#define XERROR_MULTI_TRANS_STOP_ABNORMAL       XERROR_CODE + 49

#define XEVENT_IMG_PARSE_DATA_LOST              XERROR_CODE + 50
#define XEVENT_IMG_TRANSFER_BUF_FULL            XERROR_CODE + 51
#define XEVENT_IMG_PARSE_DM_DROP                XERROR_CODE + 52
#define XEVENT_IMG_PARSE_PAC_LOST               XERROR_CODE + 53
#define XEVENT_IMG_PARSE_CRC_ERR				XERROR_CODE + 54
#define XEVENT_IMG_PARSE_VOL_ERR				XERROR_CODE + 55
#define XEVENT_CMD_HEARTBEAT_TEMPRA				XERROR_CODE + 56
#define XEVENT_CMD_HEARTBEAT_HUMIDITY			XERROR_CODE + 57

class XException
{
public:
     XException()
     {}
     explicit XException(uint32_t error_code)
     :_error_code(error_code)
     {
	  switch(_error_code)
	  {
	  case XERROR_SYS_SOCK_OPEN_FAIL:
	       _error_msg = "XSystem socket fail to open";
	       break;
	  case XERROR_SYS_SOCK_BIND_FAIL:
	       _error_msg = "XSystem socket fail to bind";
	  case  XERROR_SYS_SOCK_SEND_FAIL:
	       _error_msg = "XSystem socket fail to send command";
	       break;
	  case  XERROR_SYS_SOCK_RECV_TIMEOUT:
	       _error_msg = "XSystem socket receive timeout";
	       break;
	  case XERROR_SYS_ENGINE_RECV_ERRCMD:
	       _error_msg = "XSystem engine receive error ACK or send illegal cmd";
	       break;
	  case XERROR_SYS_ENGINE_RECV_ERRCODE:
	       _error_msg = "XSystem engine receive error code";
	       break;
	  case XERROR_SYS_ENGINE_RECV_ERRCRC:
	       _error_msg = "XSystem engine receive error crc";
	       break;

	  case XERROR_SYS_ENGINE_NOT_OPEN:
	       _error_msg = "XSystem engine not open";
	       break;
	  case XERROR_ASCPAS_FORMAT_ERR:
	       _error_msg = "XASCPasrse get error format ASCII command";
	       break;
	  case XERROR_ASCPAS_NONE_CMD:
	       _error_msg = "XASCParse get non-defined ASCII command";
	       break;
	  case XERROR_SYS_ALLOCATE_FAIL:
	       _error_msg = "XSystem fail to allocate";
	       break;
	  case XERROR_CMD_SOCK_OPEN_FAIL:
	       _error_msg = "XCmdEngine socket fail to open";
	       break;
	  case XERROR_CMD_SOCK_BIND_FAIL:
	       _error_msg = "XCmdEngine socket fail to bind ";
	       break;

	  case XERROR_CMD_SOCK_SEND_FAIL:
	       _error_msg = "XCmdEngine socket fail to send command";
	       break;
	  case XERROR_CMD_SOCK_RECV_TIMEOUT:
	       _error_msg = "XCmdEngine socket receive timeout ";
	       break;
	  case XERROR_CMD_ENGINE_RECV_ERRCMD:
	       _error_msg = "XCmd engine receive error ACK or send illegal cmd";
	       break;
	  case XERROR_CMD_ENGINE_RECV_ERRCODE:
	       _error_msg = "XCmd engine receive error code";
	       break;
	  case XERROR_CMD_ENGINE_RECV_ERRCRC:
	       _error_msg = "XCmd engine receive error crc";
	       break;
	  case XERROR_CMD_ENGINE_NOT_OPEN:
	       _error_msg = "XCmd engine not open";
	       break;
	  case XERROR_CMD_ALLOCATE_FAIL:
	       _error_msg = "XCmd fail to allocate";
	       break;
	  case XERROR_IMG_SOCK_OPEN_FAIL:
	       _error_msg = "XImgEngine socket fail to open";
	       break;
	  case XERROR_IMG_SOCK_BIND_FAIL:
	       _error_msg = "XImgEngine socket fail to bind";
	       break;
	  case  XERROR_IMG_SOCK_RECV_TIMEOUT:
	       _error_msg = "XImgEngine socket receive timeout";
	       break;
	  case XERROR_IMG_ALLOCATE_FAIL:
	       _error_msg = "XImg fail to allocate";
	       break;
	  case XERROR_IMG_ENGINE_NOT_OPEN:
	       _error_msg = "XImg engine not open";
	       break;
	  case XERROR_IMG_ENGINE_START_FAIL:
	       _error_msg = "XImg engine fail to start";
	       break;
	  case XERROR_IMG_ENGINE_STOP_ABNORMAL:
	       _error_msg = "XImg engine stop abnormally";
	       break;
	  case XERROR_IMG_PARSE_OPEN_FAIL:
	       _error_msg = "XImg parse fail to open";
	       break;
	  case XERROR_IMG_PARSE_NOT_OPEN:
	       _error_msg = "XImg parse not open";
	       break;
	  case XERROR_IMG_PARSE_START_FAIL:
	       _error_msg = "XImg parse fail to start";
	       break;
	  case XERROR_IMG_PARSE_STOP_ABNORMAL:
	       _error_msg = "XImg parse stop abnornmally";
	       break;
	  case XERROR_IMG_TRANSFER_NOT_OPEN: 
	       _error_msg = "XImg transfer not open";
	       break;
	  case XERROR_IMG_TRANSFER_START_FAIL:
	       _error_msg = "XImg transfer fail to start";
	       break;
	  case  XERROR_IMG_PACKET_POOL_OPEN_FAIL:
	       _error_msg = "XImg packet pool fail to open";
	       break;
	  case XERROR_MULTI_TRANS_ALLOCATE_FAIL:
	       _error_msg = "Multi transfer fail to allocate";
	       break;
	  /* case XERROR_MULTI_TRANS_PIX_DEPTH_NOT_EQ: */
	  /*      _error_msg = "Multi transfer pixel depth not equal"; */
	  /*      break; */
	  case XERROR_MULTI_TRANS_NOT_OPEN:
	       _error_msg = "Multi transfer not open";
	       break;
	  case XERROR_MULTI_TRANS_START_FAIL:
		   _error_msg = "Multi transfer thread fail to start";
		   break;
	  case XERROR_MULTI_TRANS_STOP_ABNORMAL:
		   _error_msg = "Multi transfer thread stop abnormally";
		   break;
	  case XERROR_DISP_ALLOCATE_FAIL:
	       _error_msg = "XDisp fail to allocate";
	       break;

	  case XERROR_CMD_HEARTBEAT_FAIL:
	       _error_msg = "XCmd fail to get heartbeat";
	       break;
	  case XERROR_CMD_HEARTBEAT_START_FAIL:
	       _error_msg = "XCmd heartbeat thread fail to start";
	       break;
	  case XERROR_CMD_HEARTBEAT_STOP_ABNORMAL:
	       _error_msg = "XCmd heartbeat thread stop abnormally";
	       break;
	  case XERROR_CMD_HEARTBEAT_VOL_ERR:
		  _error_msg = "XCmd heartbeat voltage error";
		  break;
	  case XERROR_IMG_ENGINE_GRAB_ABNORMAL:
		  _error_msg = "XImg engine grab abnormally";
		  break;

	  case XERROR_FILE_OPERATE_ERROR:
	       _error_msg = "XFILE operation fail";
	       break;

	  case XERROR_FILE_TIF_TAG_ENTRY_ABNORMAL:
	       _error_msg = "XFILE TIF tag entry abnormal";
	       break;
	  case XERROR_FILE_TIF_COMPRESSED:
	       _error_msg = "XFILE TIF compressed";
	       break;
	  case XERROR_FILE_TIF_ALLOC_FAIL:
	       _error_msg = "XTILE TIF fail to allocate";
	       break;
	  default:
	       break;
	  }
     };
     ~XException()
     {};

     uint32_t _error_code;
     std::string _error_msg;
private:
     //XException(const XException&);
     XException& operator = (const XException&);
};

#endif //XEXCEPTION_H
