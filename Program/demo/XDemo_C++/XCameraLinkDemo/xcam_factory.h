/* 
 Copyright (c) 2014, Detection Technology Inc.
 All right reserved.
*/

#ifndef XCAM_FACTORY_H
#define XCAM_FACTORY_H
#include "ixfactory.h"

class XDLL_EXPORT XCamFactory : public IXFactory
{
public:
	XCamFactory();
	~XCamFactory();
    
	IXCmdEngine* GetCmdEngine();
    //XASCParse*   GetAscParse();
    IXImgEngine* GetImgEngine(bool enline_info);
    IXImgParse*  GetImgParse(bool enline_info);
    XPacketPool* GetPacketPool();
private:
	XCamFactory(const XCamFactory&);
	XCamFactory& operator = (const XCamFactory&);
};
#endif
