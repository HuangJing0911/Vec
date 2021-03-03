/*
  Copyright (c) 2014, Detection Technology Inc.
  All right reserved.

  Author: Zhang Xu, 2014-5-29
 */

#ifndef XUDP_IMG_INFO_PARSE_H
#define XUDP_IMG_INFO_PARSE_H
#include "xudpimg_parse.h"

/*
  XMultiImgParse inherits XUDPImgParse, rewrite allocate and parse function.
  It adds line number and stap info to frame data.
 */

class XUDPImgInfoParse : public XUDPImgParse
{
public:
     XUDPImgInfoParse();
     virtual ~XUDPImgInfoParse();


private:
     XUDPImgInfoParse(const XUDPImgInfoParse&);
     XUDPImgInfoParse& operator = (const XUDPImgInfoParse&);

     virtual void CopyHeadInfo();
     virtual void Allocate();
     virtual void CopyLine();
};

#endif
