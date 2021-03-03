/*
  Copyright (c) 2015, Detection Technology Inc.
  All right reserved.
 */

#ifndef XPIECEWISE_CORRECT_H
#define XPIECEWISE_CORRECT_H

#include "ixcorrect.h"
#include "xdevice.h"
#include "xanalyze.h"

class XAcquisition;
class XFrameTransfer;

class XDLL_EXPORT XPiecewiseCorrect : public IXCorrect
{
public:
     XPiecewiseCorrect();
     ~XPiecewiseCorrect();
     bool Open(XDevice* dev_);
     void Close();
     bool DoCorrect(XImage* image_);
     bool CalculateOffset1(XAcquisition* acq_, XFrameTransfer* transfer_);
     bool CalculateGain1(XAcquisition* acq_, XFrameTransfer* transfer_, uint32_t target);
     bool CalculatePara2(XAcquisition* acq_, XFrameTransfer* transfer_, uint32_t target);
     bool CalculatePara3(XAcquisition* acq_, XFrameTransfer* transfer_, uint32_t target);
     void SaveFile(const char* file_);
     void LoadFile(const char* file_);
private:
     XPiecewiseCorrect(const XPiecewiseCorrect&);
     XPiecewiseCorrect& operator = (const XPiecewiseCorrect&);

     uint32_t* _offset1_;
     int32_t* _offset2_;
     int32_t* _offset3_;
     float*    _gain1_;
     float*    _gain2_;
     float*    _gain3_;
     uint32_t* _respond1_;
     uint32_t* _respond2_;
     uint8_t* _img_data_;
     bool     _is_open;
     uint32_t _pixel_num;
     uint32_t _target1;
     uint32_t _target2;
     XAnalyze _analyze;
};
#endif
