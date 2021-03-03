/*
	Copyright (c) 2014, Detection Technology Inc.
	All right reserved.

	Author: Zhang Xu, 2018-3-20
	*/

#ifndef XASC_PARSE_ICT_H
#define XASC_PARSE_ICT_H

#include"xasc_parse.h"
/*
	XASCParseICT inherits XASCParse, rewriting InitialCmdMap().

*/
class XDLL_EXPORT XASCParseICT : public XASCParse
{
public:
	XASCParseICT();
	~XASCParseICT();

private:
	XASCParseICT(const XASCParseICT&);
	XASCParseICT& operator = (const XASCParseICT&);

	void InitialCmdMap();

};


#endif 