/*
  Copyright (c) 2014, Detection Technology Inc.
  All right reserved.

  Author: Zhang Xu
 */
#ifndef XPIXEL_CORRECT_H
#define XPIXEL_CORRECT_H

#include "ixcorrect.h"

class XPixelList;
class XDLL_EXPORT XPixelCorrect : public IXCorrect
{
public:
     XPixelCorrect();
     ~XPixelCorrect();

     bool DoCorrect(XImage *image_);
     void PushPixel(uint32_t pixel_pos);
     void Refresh();

private:
     XPixelCorrect(const XPixelCorrect&);
     XPixelCorrect& operator = (const XPixelCorrect&);

     XPixelList* _pixel_list_;
};
#endif //XPIXEL_CORRECT_H
