/*
  Copyright (c) 2014, Detection Technology Inc.
  All right reserved.

  Author: Zhang Xu, 2014-5-21
 */

#ifndef XMULTI_TRANSFER_H
#define XMULTI_TRANSFER_H

#include "ixtransfer.h"
#include "xconfigure.h"
#include "ximage.h"
//#include <vector>

class IXImgSink;
class XFrameTransfer;
 struct XDLL_EXPORT XFrameInfo
{
     uint32_t _pixel_num;
     uint32_t _pixel_byte;
     uint32_t _line_size;
     uint32_t _cur_line_num;
     volatile uint32_t _frame_ready;
     uint8_t **_line_addr_;
};
#define XMULTI_FRAME_NUM   8//4  //Buffer size
#define XMAX_TRANSFER_NUM  12  
class XDLL_EXPORT XMultiTransfer
{
public:
     XMultiTransfer(/*uint32_t line_num*/);
     ~XMultiTransfer();
     friend class XFrameTransfer;
     bool AddTransfer(IXTransfer* transfer_);
     

     bool Open(bool en_line_info = 0);
     void Close();
     void RegisterEventSink(IXImgSink* img_sink_);

     uint32_t GetLastError();
     XImage* GetImage();
	 uint32_t GetPixelNumber();
	 uint32_t GetLineNumber();

     
private:
     XMultiTransfer(const XMultiTransfer&);
     XMultiTransfer& operator = (const XMultiTransfer&);
     void CleanTransfer();
     void PutFrame(uint8_t* data_, uint32_t size, uint32_t index);
     void Reset(uint32_t index);
     void FrameReady(uint32_t index);
	 static XTHREAD_CALL TransferThread(void* arg);
	 uint32_t TransferThreadMember();
     bool Start();
	 bool Stop();

	 uint32_t _total_pixel_num;
	 uint32_t _line_num;
     bool _is_open;
     IXImgSink* _img_sink_;
     uint8_t* _mem_pool_;
     uint32_t _transfer_num;
     uint32_t _last_err;
     uint32_t _total_line_num;
     uint32_t _cur_frame_num;
     struct XFrameInfo _frame_info[XMAX_TRANSFER_NUM];
     XLock _lock;
     XLock _reset_lock;
     XImage _image[XMULTI_FRAME_NUM];
     XImage* _image_;
     //uint32_t _pixel_num;
     //uint32_t _pixel_byte;
     //int32_t _frame_count[4];
     //std::vector<IXTransfer*> _transfer_list;
     //std::vector<uint8_t**> _line_addr_list;
	 bool	_en_line_info;
	 XThread _transfer_thread;
	 XEvent _frame_ready;
	 bool _is_running;
};
#endif //XMULTI_TRANSFER_H
