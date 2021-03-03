/*
Copyright (c) 2014, Detection Technology Inc.
All rights reserved.

This is the UDP socket wrapper class for Windows.

Author: Zhang Xu, 2014-2-24
*/
#ifndef XUDP_WIN_H
#define XUDP_WIN_H
#include <stdint.h>
#include "Winsock2.h"
#pragma comment(lib, "ws2_32.lib")

#define XUDP_RCVBUF_SIZE	128*1024	//socket receive buffer size 128K
#define XUDP_RCV_TIMEOUT	1			//socket receive timeout 10ms
#define XUDP_BUF_SIZE		1024*9		//client buffer size

/*
UDP socket wrapper class. If use it as client,Connect() Send() and
Recv()should be used. If use it as server, Bind(), Recvfrom() and Sendto()
should be used.
*/
class XUDPSocket
{
public:
	XUDPSocket();
	~XUDPSocket();
	
	bool Open(uint32_t recv_buf_size = XUDP_RCVBUF_SIZE);
	void Close();
	void SetPeer(const char* peer_ip_, uint16_t peer_port);
	bool Bind(const char* local_ip_, uint16_t local_port);
	void CleanRevBuffer();
	bool IsPeerSet();
	bool IsOpen();
	bool IsBind();
	int32_t IsDataAvailable(int32_t millisecond);

	int32_t Send(const unsigned char* send_buf_, int32_t buf_len);
	int32_t	Recv(unsigned char* recv_buf_, int32_t buf_len); 

private:
	XUDPSocket(const XUDPSocket&);
	XUDPSocket& operator = (const XUDPSocket&);

	SOCKET _socket;
	sockaddr_in _peer_serv;
	sockaddr_in _local_serv;
	bool _is_peer_set;
	bool _is_open;
	bool _is_bind;
};
#endif