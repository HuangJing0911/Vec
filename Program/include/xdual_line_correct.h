/*
  Copyright (c) 2014, Detection Technology Inc.
  All right reserved.
 */

#ifndef XDUAL_LINE_CORRECT_H
#define XDUAL_LINE_CORRECT_H

#include "ixcorrect.h"
/*
  For dual-line LCS system, two lines come within a big line. This class separate the big into two lines.
  The direction can be changable to determine which line is on the front. For dual-energy system, the date
  come in the format of "LaLb HaHb", this class will correct it as "LaHa LbHb" or "LbHb LaHa". 
 */

class XDLL_EXPORT XDualLineCorrect : public IXCorrect
{
public:
     XDualLineCorrect();
     ~XDualLineCorrect();

     bool DoCorrect(XImage* image_);
     XImage* GetImage();
     void SetDirect(bool dir);
     void SetDualEnergy(bool eng);

private:
     XDualLineCorrect(const XDualLineCorrect&);
     XDualLineCorrect& operator = (const XDualLineCorrect&);
     void Reset();
     XImage _correct_img;
     uint8_t** _hilo_addr_[2];
     uint8_t* _reserve_lo_data_;
     bool _reserve_flag;
     bool _direction_flag;
     bool _energy_flag;
};
#endif //XDUAL_LINE_CORRECT_H
