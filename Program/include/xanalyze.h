/*
  Copyright (c) 2014, Detection Technology Inc.
  All right reserved.

 */

#ifndef XANALYZE_H
#define XANALYZE_H

#include "ximage.h"
#include "xconfigure.h"
class XDLL_EXPORT XAnalyze
{
public:
     XAnalyze();
     ~XAnalyze();
     bool DoAnalyze(XImage* image_, uint32_t type);
	 float GetCommonNoise(XImage* image_);
private:
     XAnalyze(const XAnalyze&);
     XAnalyze& operator = (const XAnalyze&);
     uint32_t  _img_width;
     uint32_t  _img_height;
	 uint8_t*  _img_data_;
public:
     uint32_t* _col_avg_;
     uint32_t* _col_max_;
     uint32_t* _col_min_;
     float*    _col_noise_;
     uint32_t* _row_avg_;
     uint32_t* _row_max_;
     uint32_t* _row_min_;
     float*    _row_noise_;
};

#endif
