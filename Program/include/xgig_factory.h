/*
  Copyright (c) 2014, Detection Technology Inc.
  All right reserved.
  Author: Zhang Xu, 2014-4-14
 */

#ifndef XGIG_FACTORY_H
#define XGIG_FACTORY_H
#include "ixfactory.h"

/*
  Gige objects factory
 */
class XDLL_EXPORT XGigFactory : public IXFactory
{
public:
     XGigFactory();
     ~XGigFactory();

     IXCmdEngine* GetCmdEngine();
     //XASCParse*   GetAscParse();
     IXImgEngine* GetImgEngine(bool enline_info);
     IXImgParse*  GetImgParse(bool enline_info);
     XPacketPool* GetPacketPool();
private:
     XGigFactory(const XGigFactory&);
     XGigFactory& operator = (const XGigFactory&);

};

#endif //XGIG_FACTORY_H
