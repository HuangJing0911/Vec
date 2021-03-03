/*
  Copyright (c) 2014, Detection Technology Inc.
  All right reserved.

  Author: Zhang Xu, 2014-6-10
 */

#ifndef XCORRECT_H
#define XCORRECT_H

#include "xconfigure.h"
#include "ximage.h"

class XDLL_EXPORT IXCorrect
{
public:
	IXCorrect(){}; 
	virtual ~IXCorrect(){};
    virtual bool DoCorrect(XImage* image_) = 0;
};

#endif // XCORRECT_H
