/*
Copyright (c) 2014, Detection Technology Inc.
All rights reserved.

The header file is for configure the general parameters of different compilers.
Supported compilers: VS 2008\2010, gcc x.x.x.

Author: Zhang Xu, 2014-3-5
*/

#ifndef XCONFIGURE_H
#define XCONFIGURE_H

//#define XDEBUG	//For print debug info

//All const parameters
#define XSYS_TIMEOUT		2000
#define XCMD_TIMEOUT		20000
#define XCMD_MIN_TIMEOUT	12000
#define XIMG_TIMEOUT		20000
#define XCMD_START_CODE		0xBC
#define XCMD_END_CODE		0xFC
#define XCMD_BUF_SIZE		1460
#define XCMD_HEARTBEAT_PERIOD	2
#define XIMG_DATA_OFFSET	8

#define XERR_ACK_TIMEOUT	9
#define XERR_ACK_CRC		10
#define XERR_ACK_ERR_CODE	11
#include <assert.h>
#include <stdio.h>
#include "xlog.h"
#include "xexception.h"

#define SSE_ALIGN_BYTE		16
//Configure VS C++
#ifdef _MSC_VER
#include "stdafx.h"
#pragma warning(disable:4355)
//Define dll flag
#define XDLL_EXPORT __declspec(dllexport)
#define	XDLL_IMPORT __declspec(dllimport)

#include "xudp_win.h"
#include "xthread_win.h"
#include <windows.h>	//This must be after <winsock2.h>

#define XTHREAD_CALL uint32_t __stdcall
#define XSLEEP Sleep
#define XSLEEP_UNIT 1000  //Sleep(n ms)
//Define trace function
#ifdef XDEBUG
#define XTRACE(args) printf args
#else
#define XTRACE(args) 
#endif	//XDEBUG

#endif //_MCS_VER

//Configure gcc
#ifdef __GNUC__

#define _aligned_malloc(a,b) aligned_alloc(b,a)
#define _aligned_free(a) free(a)

//Define dll flag
#define XDLL_EXPORT
#define XDLL_IMPORT

#include "xudp_liu.h"
#include "xthread_liu.h"
#include <unistd.h>
#define XTHREAD_CALL void*
#define XSLEEP sleep
#define XSLEEP_UNIT 1 //sleep(n s)

#ifdef XDEBUG
#define XTRACE(args) printf args 
#else
#define XTRACE(args) 

#endif //XDEBUG
#endif //__GNUC__

#endif	//X_CONFIGURE_H
