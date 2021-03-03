/*
  Copyright (c) 2016, Detection Technology Inc.
  All right reserved.

  Author: Zhang Xu, 2016-09-14
*/

#ifndef IXIMG_FORMAT_H
#define IXIMG_FORMAT_H

#include "xconfigure.h"

/*
  Interface of image format.
 */

class XDLL_EXPORT IXImageFormat
{
public:
     IXImageFormat();
     virtual ~IXImageFormat();

     virtual bool Load(const char* file_) = 0;
     virtual bool Save(const char* file_) = 0;
     virtual bool GetPara(uint32_t para, uint32_t &data) = 0;
     virtual bool GetPara(uint32_t para, uint8_t** data_) = 0;
     virtual bool GetPara(uint32_t para, float &data) = 0;
     virtual bool SetPara(uint32_t para, uint32_t data) = 0;
     virtual bool SetPara(uint32_t para, uint8_t* data_) = 0;
     virtual bool SetPara(uint32_t para, float data) = 0;
     
     
};
#endif
