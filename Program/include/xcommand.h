/*
  Copyright (c) 2014, Detection Technology Inc.
  All right reserved.
 */

#ifndef XCOMMAND_H
#define XCOMMAND_H
#include "ixcmd_sink.h"
#include "xconfigure.h"

#define XPARA_CODE                0x00000000
#define XPARA_INIT_PARA           XPARA_CODE + 1
#define XPARA_SAVE_PARA           XPARA_CODE + 2
#define XPARA_INT_TIME            XPARA_CODE + 3
#define XPARA_NON_INTTIME         XPARA_CODE + 4
#define XPARA_OPE_MODE            XPARA_CODE + 5
#define XPARA_DM_GAIN             XPARA_CODE + 6
#define XPARA_HILO_MODE           XPARA_CODE + 7
#define XPARA_CH_NUM              XPARA_CODE + 8
//#define XPARA_SENSITIVITY         XPARA_CODE + 9
#define XPARA_EN_SCAN             XPARA_CODE + 10
#define XPARA_EN_GAIN_CORRECT     XPARA_CODE + 11
#define XPARA_EN_OFFSET_CORRECT   XPARA_CODE + 12
#define XPARA_EN_BASELINE_CORRECT XPARA_CODE + 13
//#define XPARA_GAIN_CORRECT        XPARA_CODE + 14
//#define XPARA_OFFSET_CORRECT      XPARA_CODE + 15
#define XPARA_BASE_LINE           XPARA_CODE + 16
//#define XPARA_CORRECT_AREA        XPARA_CODE + 17
#define XPARA_RESET_GAIN          XPARA_CODE + 18
#define XPARA_RESET_OFFSET        XPARA_CODE + 19
#define XPARA_BINNING_MODE        XPARA_CODE + 20
#define XPARA_AVERAGE_MODE        XPARA_CODE + 21
#define XPARA_SUMMING_MODE        XPARA_CODE + 22
#define XPARA_OUTPUT_SCALE        XPARA_CODE + 23
#define XPARA_OFFSET_AVERAGE      XPARA_CODE + 24
#define XPARA_LINE_TRIGGER_MODE   XPARA_CODE + 25
#define XPARA_EN_LINE_TRIGGER     XPARA_CODE + 26
#define XPARA_LINETRG_FINE_DELAY  XPARA_CODE + 27
#define XPARA_LINETRG_RAW_DELAY   XPARA_CODE + 28
#define XPARA_FRAME_TRIGGER_MODE  XPARA_CODE + 29
#define XPARA_EN_FRAME_TRIGGER    XPARA_CODE + 30
#define XPARA_FRAME_TRIGGER_DELAY XPARA_CODE + 31
#define XPARA_SIMULATE_FRAME_TRIGGER   XPARA_CODE + 32
#define XPARA_HILO_EDGE           XPARA_CODE + 33
#define XPARA_HILO_SAMPLE_POS     XPARA_CODE + 34
#define XPARA_PIXEL_NUMBER        XPARA_CODE + 35
#define XPARA_PIXEL_SIZE          XPARA_CODE + 36
#define XPARA_PIXEL_DEPTH         XPARA_CODE + 37
#define XPARA_MAXMIN_INTTIME      XPARA_CODE + 38
#define XPARA_GCU_FIRM_VER        XPARA_CODE + 39
#define XPARA_DM_FIRM_VER         XPARA_CODE + 40
#define XPARA_GCU_TEST_MODE       XPARA_CODE + 41
#define XPARA_DM_TEST_MODE        XPARA_CODE + 42
#define XPARA_DM_PIXEL_NUM        XPARA_CODE + 43
#define XPARA_DFE_CARD_NUM        XPARA_CODE + 44
#define XPARA_CARD_TYPE           XPARA_CODE + 45
//#define XPARA_AD_CARD_NUM         XPARA_CODE + 46
//#define XPARA_SAVE_GAIN           XPARA_CODE + 47
#define XPARA_LOAD_GAIN           XPARA_CODE + 48
//#define XPARA_SAVE_OFFSET         XPARA_CODE + 49
#define XPARA_LOAD_OFFSET         XPARA_CODE + 50
#define XPARA_GCU_SERIAL          XPARA_CODE + 51
#define XPARA_DM_SERIAL           XPARA_CODE + 52
//#define XPARA_DFE_AD_NUM          XPARA_CODE + 53
//#define XPARA_AD_SPEED            XPARA_CODE + 54
#define XPARA_AD_SAMPLE_POS	  XPARA_CODE + 55
#define XPARA_INIT1_PARA          XPARA_CODE + 56
#define XPARA_EN_LED              XPARA_CODE + 57
#define XPARA_TERMINAL_RESISTOR	  XPARA_CODE + 58
#define XPARA_GCU_HEALTH          XPARA_CODE + 59
#define XPARA_DM_HEALTH           XPARA_CODE + 60
#define XPARA_GCU_TYPE		  XPARA_CODE + 61
#define XPARA_ENERGY_MODE         XPARA_CODE + 62
#define XPARA_Z_GAP               XPARA_CODE + 63
#define XPARA_PRODUCT_INFO        XPARA_CODE + 64
#define XPARA_EN_PDC			  XPARA_CODE + 65
#define XPARA_TRIGGER_STAMP_PARITY XPARA_CODE + 66
#define XPARA_POWER_UP_TIME		  XPARA_CODE + 67
#define XPARA_GCU_WORK_TIME		  XPARA_CODE + 68

#define XPARA_ENCODE_COUNT		  XPARA_CODE + 69
#define XPARA_AD_VALID_OUTPUT	  XPARA_CODE + 70
#define XPARA_AD_OFFSET_CORRECT	  XPARA_CODE + 71
#define XPARA_CONFIG_FIRMWARE	  XPARA_CODE + 72
#define XPARA_DO_CALIBRATION      XPARA_CODE + 73
#define XPARA_CALIBRATION_SAMPLES XPARA_CODE + 74
#define XPARA_DM_FIRM_HD_VER	  XPARA_CODE + 75

#define ASC_BUF_SIZE  128

class IXCmdEngine;
class XASCParse;
class XDevice;
class IXFactory;
/*
  XCommand class provides command channel interface. It parse the ASCII command
  and send the hex command to detector. It provides the wrapping function
  of sending command.
 */

 class XDLL_EXPORT XCommand
{
public:
     explicit XCommand(IXFactory* factory_, uint32_t timeout = XCMD_TIMEOUT);
	 XCommand();
     virtual ~XCommand();

     void SetTimeout(uint32_t time);
	 void SetFactory(IXFactory* factory_);
	 bool Open(XDevice* dev_);
     void Close();
     void RegisterEventSink(IXCmdSink* cmd_sink_);
     int32_t SendAscCmd(std::string asc_send, std::string& asc_recv);
	 int32_t SendAscCmd(char* asc_send_, char* asc_recv_);
     uint32_t GetLastError();
     bool GetIsOpen();
     int32_t SendCommand(uint8_t cmd_code, uint8_t operation, uint8_t dm_id,
	              uint16_t data_size, uint8_t* send_data_,
		      uint8_t* recv_data_);

     int32_t SetPara(uint32_t para, uint64_t data, uint8_t dm_id =0);
	 int32_t SetPara(uint32_t para, std::string data, uint8_t dm_id=0);
     int32_t GetPara(uint32_t para, uint64_t &data, uint8_t dm_id=0);
	 int32_t GetPara(uint32_t para, std::string &data, uint8_t dm_id=0);
	 int32_t GetPara(uint32_t para, char* recv_data_, uint8_t dm_id=0);
     virtual int32_t GetPara(uint32_t para, float &temperature, float &humidity, float &v1, float &v2, float &v3,
		     float &v4, float &v5, float &v6, float &v7, float &high_temp,  uint8_t dm_id=0);
     int32_t ExecutePara(uint32_t para, uint64_t data=0);
	 bool StartHeartbeat();
     bool StopHeartbeat();

protected:
     XCommand(const XCommand&);
     XCommand& operator=(const XCommand&);
     
     bool TestConnection();
     bool Allocate();
     void Release();
     int32_t GetData(const std::string asc_recv, uint64_t& return_val);
     static XTHREAD_CALL HeartbeatThread(void* arg);
     virtual uint32_t HeartbeatThreadMember();
	 virtual bool InitDev(XDevice* dev_);

     bool _is_open;
	 bool _is_heartbeat_running;
     uint32_t _last_err;
     uint32_t _timeout;
     

     IXFactory* _factory_;
     IXCmdEngine* _cmd_engine_;
     XASCParse* _asc_parse_;
     IXCmdSink* _cmd_sink_;
     XLock _lock;
	 XThread _heartbeat_thread;
	 uint32_t _card_type;
};
#endif //XCOMMAND_H
