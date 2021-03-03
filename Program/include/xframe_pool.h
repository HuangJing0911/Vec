/*
  Copyright (c), Detection Technology Inc.
  All right reserved.

  Author: Zhang Xu, 2014-5-5
 */
#ifndef XFRAME_POOL_H
#define XFRAME_POOL_H
#include "xconfigure.h"
#include "ximage.h"
#include "ixtransfer.h"

#define XFRAME_NUM 4
//class IXTransfer;
class  XDLL_EXPORT XFramePool
{
public:
     XFramePool(IXTransfer* transfer_);
     ~XFramePool();

     bool Initialize(uint32_t img_width, uint32_t img_height,
		     uint32_t pixel_depth, uint32_t data_offset=0);

     void PutLine(uint8_t* line_data_, size_t line_size, uint32_t data_offset = 0);
     void Reset();
	 void Release();
     uint32_t GetImgWidth();
     uint32_t GetPixelByte();
private:
 
     IXTransfer* _transfer_;
     uint8_t* _mem_pool_;
     uint32_t _img_width;
     uint32_t _img_height;
     uint32_t _pixel_byte;
     uint32_t _line_size;
     uint32_t _total_line_num;
     uint32_t _cur_frame_num;
     uint32_t _cur_line_num;  
     uint8_t** _line_addr_;
	 uint8_t* _align_line_;
	 uint16_t _pre_id;
     bool     _is_init; 
     XImage _image[XFRAME_NUM];    
};
#endif //XFRAME_POOL_H
