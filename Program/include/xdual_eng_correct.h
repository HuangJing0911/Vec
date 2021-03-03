/*
  Copyright (c) 2014, Detection Technology Inc.
  All right reserved.

 */
#ifndef XDUAL_ENG_CORRECT_H
#define XDUAL_ENG_CORRECT_H

//#include "xconfigure.h"
//#include "ximage.h"

#include "ixcorrect.h"

/*
  For LCS dual energy mode, heigh and low energy lines come alternantly.
  This class will separate the two kinds of lines into two part, left part is
  low energy, right part is high energy.
  The input raw line must have info header.
 */

class XDLL_EXPORT XDualEngCorrect : public IXCorrect
{
public:
     XDualEngCorrect();
     ~XDualEngCorrect();

     bool DoCorrect(XImage* image_);
     XImage* GetImage();
	 
private:
     XDualEngCorrect(const XDualEngCorrect&);
     XDualEngCorrect& operator = (const XDualEngCorrect&);
	 void Reset();
     XImage _dual_img;
     uint8_t** _hilo_addr_[2];
     uint8_t* _reserv_lo_data_;
     bool _reserv_flag;
};
#endif //XDUAL_ENG_CORRECT_H
