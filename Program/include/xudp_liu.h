/*
  Copyright (c) 2014, Detection Technology Inc.
  All rights reserved.

  This is UDP socket wrapper class for Linux.

  Author: Zhang Xu, 2014-3-11

*/

#ifndef XUDP_LIU_H
#define XUDP_LIU_H
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/select.h>
#define XUDP_RCVBUF_SIZE 128*1024 /*Socket receive buffer size 128k  */
#define XUDP_RCV_TIMEOUT 1000     /*Socket receive timeout 1ms  */
#define XUDP_BUF_SIZE    1024*9	  /*Client buffer size  */
#define INVALID_SOCKET   0
#define SOCKET_ERROR     -1
/*
  UDP socket wrapper class. 
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
  int32_t Recv(unsigned char* recv_buf_, int32_t buf_len);
  
 private:
  XUDPSocket(const XUDPSocket&);
  XUDPSocket& operator= (const XUDPSocket&);

  int32_t _socket;
  sockaddr_in _peer_serv;
  sockaddr_in _local_serv;
  bool _is_peer_set;
  bool _is_open;
  bool _is_bind;

};
#endif
