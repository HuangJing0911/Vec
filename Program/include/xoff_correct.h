/*
  Copyright (c) 2014, Detection Technology Inc.
  All right reserved.
  
 */
#ifndef XOFF_CORRECT_H
#define XOFF_CORRECT_H
#include "ixcorrect.h"
#include "xdevice.h"
#include "xanalyze.h"
class XAcquisition;
class XFrameTransfer;
class XCommand;


class XDLL_EXPORT XOffCorrect : public IXCorrect
{
public:
     XOffCorrect();
     ~XOffCorrect();
     virtual bool Open(XDevice* dev_);
     virtual void Close();
     virtual bool DoCorrect(XImage* image_);
     virtual bool CalculatePara(uint32_t type, XAcquisition* acq_,
		       XFrameTransfer* transfer_, uint32_t start,
		       uint32_t end, uint32_t target);
     virtual bool CalculatePara(uint32_t type, XAcquisition* acq_,
		       XFrameTransfer* transfer_, uint32_t target);
     virtual void Reset();

     virtual void SaveFile(const char* file_);
     virtual void LoadFile(const char* file_);
	
     virtual bool SaveFlash(XCommand* cmd_, uint8_t index = 0);
     virtual bool LoadFlash(XCommand* cmd_, uint8_t index = 0);
//private:
protected:
     XOffCorrect(const XOffCorrect&);
     XOffCorrect& operator = (const XOffCorrect&);

     uint32_t* _offset_val_;
     float*    _gain_val_;
     uint8_t*  _img_data_;
     uint32_t  _pixel_num;
     uint32_t  _dm_pix_num;
     uint32_t  _op_mode;
     uint32_t  _card_num;
     uint32_t  _binning_mode;
     uint32_t  _energy_mode;
     uint64_t  _channel_num;
     XAnalyze  _analyze;
};
#endif //XOFF_CORRECT_H
