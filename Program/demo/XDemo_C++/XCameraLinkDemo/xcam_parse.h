#ifndef XCAM_PARSE_H
#define XCAM_PARSE_H

#include "iximg_parse.h"

class XCamParse : public IXImgParse
{
public:	
	XCamParse(){};
	virtual ~XCamParse(){};

	bool Open(XDevice* dev_){return 1;};
	void Close(){};
	bool Start() {return 1;};
	bool Stop() {return 1;};
	uint32_t GetLastError() {return 0;};
	void SetImgSink(IXImgSink*) {};
	void SetPacketPool(XPacketPool*) {};
	bool GetIsRunning() {return 0;};
	void SetFrameTransfer(IXTransfer*) {};
	void Reset() {};
};
#endif