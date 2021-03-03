/*
	Copyright (c) 2014, Detection Technology Inc.
	All right reserved.

	Author: Zhang Xu, 2018-3-21
	*/

#ifndef XCOMMAND_ICT_H
#define XCOMMAND_ICT_H

#include "xcommand.h"

/*
	XCcommandICT inherits XCommamnd
*/

class XDLL_EXPORT XCommandICT : public XCommand
{
public:
	XCommandICT();
	~XCommandICT();

    virtual int32_t GetPara(uint32_t para, float &temperature, float &humidity, float &v1, float &v2, float &v3,
		     float &v4, float &v5, float &v6, float &v7, float &high_temp,  uint8_t dm_id=0);
    //int32_t ExecutePara(uint32_t para, uint64_t data=0, uint8_t dm_id=0);
private:
	XCommandICT(const XCommandICT&);
	XCommandICT& operator = (const XCommandICT&);
	virtual bool InitDev(XDevice* dev_);
	virtual uint32_t HeartbeatThreadMember();
};

#endif