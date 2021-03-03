/*
  Copyright (c) 2014, Detection Technology Inc.
  All right reserved.

  Author: Zhang Xu, 2014-4-28
 */
#ifndef XUDPIMG_PARSE_H
#define XUDPIMG_PARSE_H
#include "iximg_parse.h"
#include "iximg_sink.h"
#include "xpacket_pool.h"
#include "ixtransfer.h"
//#include "xdevice.h"
struct XHeader
{
     uint8_t  _cmd_flag;
     uint8_t  _packet_id;
     uint16_t _line_id;
     uint16_t _payload_size;
};
struct XDMInfo
{

};

struct XImgInfo
{
     uint32_t _line_stamp;
     uint32_t _line_size;
     uint8_t  _pixel_size;
     uint8_t  _energy_flag;
     uint8_t  _compress_flag;
     uint8_t  _dm_num;
};

struct XLineBuf
{
     size_t _cur_pos;
     uint8_t* _line_buf_;
};

#define XPARSE_CARD_SINGLE     0
#define XPARSE_CARD_DUAL_LUPU  1
#define XPARSE_CARD_DUAL_DM    2
#define XPARSE_CARD_DM_DOUBLE  3
#define XPARSE_CARD_DUAL_DM_DOUBLE 4
/*
  XUDPImgParse getts image pakcets form memory pool and parse each.
  Then put the real image data into frame transfer.
 */
#define LINE_BUF_SIZE 32*1024
#define MAX_LINE_NUM  0xFFFF

class XUDPImgParse : public IXImgParse
{
public:
     XUDPImgParse();
     virtual ~XUDPImgParse();

     bool Open(XDevice* dev_);
     void Close();
     bool Start();
     bool Stop();

     uint32_t GetLastError();
     void SetImgSink(IXImgSink*);
     void SetPacketPool(XPacketPool*);
     bool GetIsRunning();
     void Reset();
     void SetFrameTransfer(IXTransfer* transfer_);

//private:
protected:
     XUDPImgParse(const XUDPImgParse&);
     XUDPImgParse operator = (const XUDPImgParse&);
     static XTHREAD_CALL ParseThread(void* arg);
     uint32_t ParseThreadMember();
     bool Parse();
     void FindFirstPacket();
     void GetInfo(XPacket* packet_);
     void GetHeader(XPacket* packet_);
     bool IsLeader();
     void ParseFirstLine();
     virtual void Allocate();
     virtual void CopyHeadInfo();
     virtual void CopyLine();
     /* void ParseDualLupu(); */
     /* void ParseDualDM(); */
     /* void ParseDMDouble(); */
     /* void ParseDualDMDouble(); */

     bool _is_open;
     bool _is_running;
     bool _data_lost;
     uint32_t _last_err;
     uint16_t _line_num;
     uint16_t _packet_num;
     uint16_t _max_packet_num;
     IXImgSink* _img_sink_;
     XPacketPool* _packet_pool_;
     IXTransfer* _frame_transfer_;

     XLineBuf _line_buffer;
     XThread _parse_thread;
     struct XHeader _img_header;
     struct XImgInfo _img_info;
     uint32_t _card_type;
     uint32_t _card_number;
};

#endif //XUDPIMG_PARSE_H
