/*
  Copyright (c) 2014, Detection Technology Inc.
  All rights reserved.
 */
#ifndef XDEVICE_H
#define XDEVICE_H
#include "xconfigure.h"
#include "xsystem.h"
#include "xcommand.h"
#include "xframe_transfer.h"
#include "xudpimg_parse.h"

/* #define XOP_MODE_SINGLE_CONTINUE    0 */
/* #define XOP_MODE_SINGLE_NONCONTINUE 1 */
/* #define XOP_MODE_DUAL_CONTINUE      2 */
/* #define XOP_MODE_DUAL_NONCONTINUE   3 */
/* #define XOP_MODE_SINGLE_CONSTANT    4 */
/* #define XOP_MODE_DUAL_CONSTANT      5 */
/* #define XOP_MODE_DUAL_NON_HILO      6 */

#define XOP_MODE_CONTINUE           0
#define XOP_MODE_NONCONTINUE        1
#define XOP_MODE_CONSTANT           2
#define XOP_MODE_NON_HILO           3

#define XENERGY_MODE_SINGLE         0
#define XENERGY_MODE_STACK          1
#define XENERGY_MODE_PARALLEL       2

#define XCARD_TYPE_DM               0
#define XCARD_TYPE_LUPU             1
#define XCARD_TYPE_LUNA             2
#define XCARD_TYPE_CARD2            3
#define XCARD_TYPE_DM_DOUBLE        4
#define XCARD_TYPE_CARD3A           5
#define XCARD_TYPE_ICT				6
#define XCARD_TYPE_CARD3B			7
/*
  XDevice class wraps the basic parameters of detector.
 */
class  XDLL_EXPORT XDevice
{
public:
XDevice(XSystem* sys_)
     :_serial_port(3)
     {_system_= sys_;	  
     }
XDevice()
     :_serial_port(3)
	  ,_card_number(0)
	  ,_system_(NULL)
     {
     }
     ~XDevice()
     {};

     friend class  XSystem;
     friend class  XCommand;
     friend class  XFrameTransfer;
     friend class  XUDPImgParse;
     friend class  XDisplay;
     friend class  XOffCorrect;
     friend class  XPiecewiseCorrect;
	 friend class XCommandICT;
     const char* GetIP()
     {
	  return (const char*) _ip;
     }
     void SetIP(const char* ip_)
     {
	  memcpy(_ip, ip_, 20);
     }
     uint8_t* GetMAC()
     {
	  return _mac;
     }
     void SetMAC(uint8_t* mac_)
     {
	  memcpy(_mac, mac_, 6);
     }
     uint16_t GetCmdPort()
     {
	  return _cmd_port;
     }
     void SetCmdPort(uint16_t port)
     {
	  _cmd_port = port;
     }
     uint16_t GetImgPort()
     {
	  return _img_port;
     }
     void SetImgPort(uint16_t port)
     {
	  _img_port = port;
     }
     char* GetSerialNum()
     {
	  return _serial_num;
     };
     XSystem* GetSystem()
     {
	  return _system_;
     }
     void SetSerialPort(uint32_t port)
     {
	  _serial_port = port;
     }
     uint32_t GetSerialPort()
     {
	  return _serial_port;
     }
     
     uint32_t GetPixelNumber()
     {
	  return _pixl_number;
     }
     uint32_t GetPixelDepth()
     {
	  return _pixl_depth;
     }
     uint32_t GetOPMode()
     {
	  return _op_mode;
     }
     uint32_t GetCardType()
     {
	  return _card_type;
     }
     uint32_t GetDMPixelNumber()
     {
	  return _dm_pixl_number;
     }
     uint32_t GetCardNumber()
     {
	  return _card_number;
     }
     uint32_t GetBinningMode()
     {
	  return _binning_mode;
     }
	 uint32_t GetEnergyMode()
	 {
	  return _energy_mode;
	 }
private:
     char	 _ip[20];
     char	 _serial_num[32];
     uint8_t     _mac[6];
     uint16_t    _cmd_port;
     uint16_t    _img_port;
     uint32_t	 _serial_port;
     //uint64_t    _serial_num;
     uint32_t    _pixl_number;
     uint32_t    _pixl_depth;
     uint32_t    _op_mode;
     uint32_t    _card_type;
     uint32_t    _dm_pixl_number;
     uint32_t    _card_number;
     uint32_t    _binning_mode;
     uint64_t    _channel_num;
     uint32_t    _energy_mode;
     XSystem*	 _system_;
};
#endif //XDEVICE_H
