/*
  Copyright (c) 2014, Detection Technology Inc.
  All right reserved.

  Author: Zhang Xu, 2014-4-21
 */

#ifndef XPACKET_POOL_H
#define XPACKET_POOL_H
#include "xconfigure.h"

#define XPAC_SIZE 9*1024//2*1024

#define XPAC_NUM  4096//10240
#define XPAC_DATA_POS 24  //The XPacket size is 20 in x64 mode, but due to the alignment actual size is 24
struct XPacket
{
     struct XPacket* next_;
     int32_t size;
     uint8_t* data_;
};
/*
  Allocate memory block for image line packet. Free list is the packets without
  image data, used list is the packets with image data.
 */

class XDLL_EXPORT XPacketPool
{
public:
     XPacketPool();
     ~XPacketPool();
     bool Initialize();
     XPacket* GetFreePacket();
     XPacket* GetUsedPacket();
     void PushFreePacket(XPacket* packet_);
     void PushUsedPacket(XPacket* packet_);
     void Reset();
private:
     uint32_t _free_num;
     uint32_t _used_num;
     bool     _is_init;
     uint8_t* _mem_pool_;
     struct XPacket* _free_head_;
     struct XPacket* _free_tail_;
     struct XPacket* _used_head_;
     struct XPacket* _used_tail_;

     XLock _free_lock;
     XLock _used_lock;
     XEvent _free_pac_ready;
     XEvent _used_pac_ready;
};
#endif //XPACKET_POOL_H
