#include "stdafx.h"
#include "xserialcmd_engine.h"


XSerialCmdEngine::XSerialCmdEngine(uint32_t timeout, IXCmdSink* cmd_sink_)
    :_is_open(0)
    ,_timeout(timeout)
    ,_last_err(0)
    ,_crc_check(XCRC32_KEY)
    ,_cmd_sink_(cmd_sink_)
{

}
XSerialCmdEngine::XSerialCmdEngine()
    :_is_open(0)
    ,_timeout(XCMD_TIMEOUT)
    ,_last_err(0)
    ,_crc_check(XCRC32_KEY)
    ,_cmd_sink_(NULL)
{

}
XSerialCmdEngine::~XSerialCmdEngine()
{
    if(_is_open)
	Close();
}

void XSerialCmdEngine::SetTimeout(uint32_t timeout)
{
    _timeout = timeout;
}
void XSerialCmdEngine::SetCmdSink(IXCmdSink* cmd_sink_)
{
    _cmd_sink_= cmd_sink_;
}
/*
  Open udp socket according to the property of device. Default local ip is
  not binded specificly, and local port is the same with detector's.
 */
bool XSerialCmdEngine::Open( XDevice* device_)
{
    if(_is_open)
	return _is_open;
    assert(device_);
    try
    {
	char port_str[20];
	_serial_sock.SetupReadTimeouts(CSerial::EReadTimeoutNonblocking);
	sprintf(port_str,"COM%d", device_->GetSerialPort());
	if(_serial_sock.Open(port_str) != ERROR_SUCCESS)
	    throw XException(XERROR_CMD_SOCK_OPEN_FAIL);
	if(0 == device_->GetCmdPort())
		_serial_sock.Setup(CSerial::EBaud115200);
	else
		_serial_sock.Setup(CSerial::EBaud9600);	
	
    }
    catch(XException exp)
    {
	_is_open = 0;
	_last_err = exp._error_code;
	//log_obj.LogString(XLOG_ERROR, exp._error_msg.c_str());
	
	if(_cmd_sink_)
	    _cmd_sink_->OnXError(exp._error_code, exp._error_msg.c_str());
	return _is_open;
    }

    _is_open = 1;
    return _is_open;

}
void XSerialCmdEngine::Close()
{
    if(_is_open)
    {
	_serial_sock.Close();
	_is_open = 0;
    }
}
uint32_t XSerialCmdEngine::GetLastError()
{
    return _last_err;
}

//Define the position of each section
#define START_BYTE 0
#define CMD_BYTE   2
#define OPE_BYTE   3
#define DMID_BYTE  4
#define SIZE_BYTE  5
#define DATA_BYTE  6

#define CRC_SIZE  4
#define TRY_TIME  2

#define RECV_WAIT_TIME 100
#define HEARTBEAT_LENGTH 24
int32_t XSerialCmdEngine::SendCommand(uint8_t cmd_code, uint8_t operation,
				   uint8_t dm_id, uint16_t data_size,
				   uint8_t* send_data_,
				   uint8_t* recv_data_, uint8_t& err_code)
{
    try
    {
	if(!_is_open)
	    throw XException(XERROR_CMD_ENGINE_NOT_OPEN);

	//Copy to send buffer
	_send_buf[START_BYTE] = XCMD_START_CODE;
	_send_buf[START_BYTE+1] = XCMD_START_CODE;
	_send_buf[CMD_BYTE] = cmd_code;
	_send_buf[OPE_BYTE] = operation;
	_send_buf[DMID_BYTE]= dm_id;
	_send_buf[SIZE_BYTE]= static_cast<uint8_t> (data_size);
	if(data_size)
	    memcpy(&_send_buf[DATA_BYTE], send_data_, data_size);

	//Caculate CRC
	for(int32_t i = CMD_BYTE; i<(DATA_BYTE+data_size); i++)
	    _crc_check.PutByte(_send_buf[i]);

	uint32_t crc_result = _crc_check.Done();
	//Copy big-endian crc result to send buffer
	crc_result = htonl(crc_result);
	memcpy(&_send_buf[DATA_BYTE+data_size], &crc_result,
	       sizeof(crc_result));

	//End code
	_send_buf[DATA_BYTE+data_size+CRC_SIZE] = XCMD_END_CODE;
	_send_buf[DATA_BYTE+data_size+CRC_SIZE+1] = XCMD_END_CODE;

	DWORD recv_len;
	//Clean buffet
	//for(int32_t i = 0; i< 2; i++)
	{
		_serial_sock.Read(_recv_buf, XCMD_BUF_SIZE,&recv_len, 0, CSerial::EReadTimeout::EReadTimeoutNonblocking);
		//Sleep(RECV_WAIT_TIME);
	}

	
	//Send command
	int32_t try_time = 0;  //Counter of the try times

	int32_t len = DATA_BYTE+data_size+CRC_SIZE+2;
	
	DWORD send_len = 0;
	if(ERROR_SUCCESS != _serial_sock.Write(_send_buf, len, &send_len))
	{
	    //Fail to send, try again
	    len = DATA_BYTE+data_size+CRC_SIZE+2;
	
	   if(ERROR_SUCCESS != _serial_sock.Write(_send_buf, len, &send_len))
		throw XException(XERROR_CMD_SOCK_SEND_FAIL);
	}

	//Receive
	DWORD start = GetTickCount();
	DWORD end = start;
	
	int32_t try_recv = 0;
//TRY_RECV:
	while(1)
	{
		Sleep(RECV_WAIT_TIME);
		_serial_sock.Read(_recv_buf, XCMD_BUF_SIZE,&recv_len, 0, CSerial::EReadTimeout::EReadTimeoutNonblocking);
		if(recv_len)
			break;
		else
		{
			end = GetTickCount();
			if(end - start > _timeout)
				 throw XException(XERROR_CMD_SOCK_RECV_TIMEOUT);
		}

	}
	int32_t recv_data_len;
	//if(0xFF == _recv_buf[CMD_BYTE])
	//{
	//	//Get heartbeat packet, parse from the end of heartbeat packet
	//	recv_data_len = ParseRecv(recv_data_, err_code);
	//}
	//else
	//Parse received code
	recv_data_len = ParseRecv(recv_data_, err_code);
	
	return recv_data_len;
    }

    catch(XException exp)
    {
	_last_err = exp._error_code;
	//log_obj.LogString(XLOG_ERROR, exp._error_msg.c_str());
	//XTRACE(("%s\n", exp._error_msg.c_str()));
	if(_cmd_sink_)
	    _cmd_sink_->OnXError(exp._error_code, exp._error_msg.c_str());
	return -1;
    }
 
}
/*
  Parse the received ack. 
  The ACK protocol:
  0xBCBC CMD(1byte) ERRID(1byte) DMID(1byte) DATASIZE(1byte) CRC(4bytes) 0xFCFC
  Chec CMD should be same with sended command, ERRID should be 0 and crc resutlt
 */
#define ERR_BYTE 3
int32_t XSerialCmdEngine::ParseRecv(uint8_t* recv_data_,uint8_t &err_code)
{
    if(0xFF == _recv_buf[CMD_BYTE])
	{
		//Get heartbeat packet, parse from the end of heartbeat packet

		if(_recv_buf[CMD_BYTE+HEARTBEAT_LENGTH] != _send_buf[CMD_BYTE])
		{
			err_code = XERR_ACK_ERR_CODE;
			throw XException(XERROR_CMD_ENGINE_RECV_ERRCMD);
		}
		err_code = _recv_buf[ERR_BYTE + HEARTBEAT_LENGTH];
		if(_recv_buf[ERR_BYTE+ HEARTBEAT_LENGTH] != 0)
		{
		throw XException(XERROR_CMD_ENGINE_RECV_ERRCODE);
		}
		uint8_t data_size = _recv_buf[SIZE_BYTE + HEARTBEAT_LENGTH];
		//Caculate CRC
		for(int32_t i = CMD_BYTE; i<(DATA_BYTE +data_size); i++)
		_crc_check.PutByte(_recv_buf[i + HEARTBEAT_LENGTH]);
		uint32_t crc_result = _crc_check.Done();
		//Change to big-endian
		crc_result = htonl(crc_result);
		if(crc_result != *(uint32_t*)&_recv_buf[DATA_BYTE + HEARTBEAT_LENGTH+data_size])
		{
			err_code = XERR_ACK_CRC;
			throw XException(XERROR_CMD_ENGINE_RECV_ERRCRC);
		}

		//No any fault, copy data if data_size if above 0
		if(data_size)
		memcpy(recv_data_, &_recv_buf[DATA_BYTE+ HEARTBEAT_LENGTH], data_size);

		return data_size;
		
	}
	
	
	if(_recv_buf[CMD_BYTE] != _send_buf[CMD_BYTE])
    {
		err_code = XERR_ACK_ERR_CODE;
		throw XException(XERROR_CMD_ENGINE_RECV_ERRCMD);
    }
	err_code = _recv_buf[ERR_BYTE];
    if(_recv_buf[ERR_BYTE] != 0)
    {
	throw XException(XERROR_CMD_ENGINE_RECV_ERRCODE);
    }
    uint8_t data_size = _recv_buf[SIZE_BYTE];
    //Caculate CRC
    for(int32_t i = CMD_BYTE; i<(DATA_BYTE +data_size); i++)
	_crc_check.PutByte(_recv_buf[i]);
    uint32_t crc_result = _crc_check.Done();
    //Change to big-endian
    crc_result = htonl(crc_result);
    if(crc_result != *(uint32_t*)&_recv_buf[DATA_BYTE+data_size])
	{
		err_code = XERR_ACK_CRC;
		throw XException(XERROR_CMD_ENGINE_RECV_ERRCRC);
	}

    //No any fault, copy data if data_size if above 0
    if(data_size)
	memcpy(recv_data_, &_recv_buf[DATA_BYTE], data_size);

    return data_size;
}


int32_t XSerialCmdEngine::RecvHeartbeat(uint8_t* recv_data_, int32_t len)
{
	DWORD recv_len;
	_serial_sock.Read(recv_data_, XCMD_BUF_SIZE,&recv_len, 0, CSerial::EReadTimeout::EReadTimeoutNonblocking);
	return recv_len;
}
