#include "stdafx.h"
#include "xcam_factory.h"
#include "xserialcmd_engine.h"
#include "xcamimg_engine.h"
#include "xpacket_pool.h"
#include "xcam_parse.h"
XCamFactory::XCamFactory()
{

}
XCamFactory::~XCamFactory()
{

}
    
IXCmdEngine* XCamFactory::GetCmdEngine()
{
	return new XSerialCmdEngine();
}
//XASCParse*  XCamFactory::GetAscParse()
//{
//	return new XASCParse;
//}

IXImgEngine* XCamFactory::GetImgEngine(bool enline_info)
{
    return new XCamImgEngine(enline_info);
}

IXImgParse* XCamFactory::GetImgParse(bool enline_info)
{
	return new XCamParse;
}

XPacketPool* XCamFactory::GetPacketPool()
{
	return new XPacketPool;
}
