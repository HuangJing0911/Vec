/*
  Copyright (c) 2015, Detection Technology Inc.
  All right reserved.
*/

#ifndef XREFERENCE_CORRECT_H
#define XREFERENCE_CORRECT_H

#include "ixcorrect.h"
#include "xdevice.h"
#include "xanalyze.h"
class XAcquisition;
class XFrameTransfer;

#define REFERENCE_PIX_NUM 32
class XDLL_EXPORT XReferenceCorrect : public IXCorrect
{
public:
     XReferenceCorrect();
     ~XReferenceCorrect();

     bool Open(XDevice* dev_,uint32_t ref_num=REFERENCE_PIX_NUM);
     void Close();
     bool DoCorrect(XImage* image_);
     // XImage* GetImage();
     bool CalculatePara(XAcquisition* acq_, XFrameTransfer* transfer_);
private:
     XReferenceCorrect(const XReferenceCorrect&);
     XReferenceCorrect& operator =(const XReferenceCorrect&);
     float* _gain_;
     uint32_t _ref_num; //The reference pixel number
     XAnalyze _analyze;
     
};

#endif //XREFERENCE_CORRECT_H
  
