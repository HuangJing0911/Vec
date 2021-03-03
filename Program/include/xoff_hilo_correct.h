
/*
  Copyright (c) 2014, Detection Technology Inc.
  All right reserved.
  
 */

#ifndef XOFF_HILO_CORRECT_H
#define XOFF_HILO_CORRECT_H

#include "xoff_correct.h"

class XDLL_EXPORT XOffHLCorrect : public XOffCorrect
{
public:
     XOffHLCorrect();
     ~XOffHLCorrect();

     virtual bool Open(XDevice* dev_);
     virtual void Close();
     virtual bool DoCorrect(XImage* image_);
     virtual bool CalculatePara(uint32_t type, XAcquisition* acq_,
		       XFrameTransfer* transfer_, uint32_t start,
		       uint32_t end, uint32_t target);
     virtual void Reset();

     virtual void SaveFile(const char* file_);
     virtual void LoadFile(const char* file_);
	

private:
     XOffHLCorrect(const XOffHLCorrect&);
     XOffHLCorrect& operator = (const XOffHLCorrect&);

     uint32_t* _offset_val_h_;
     float*     _gain_val_h_;
     bool       _is_open;
     
};
#endif
