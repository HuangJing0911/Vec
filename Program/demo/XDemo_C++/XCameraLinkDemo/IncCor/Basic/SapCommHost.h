#ifndef _SAPCOMMHOST_H_
#define _SAPCOMMHOST_H_

//	SapCommHost.h : header file
//

#include "SapClassBasicDef.h"

// Constants
const int TIMEOUT_LENGTH = 60000;
const int TIMEOUT_SLICE  = 100;

//
// SapCommHost class declaration
//
class SAPCLASSBASIC_CLASS SapCommHost : public SapCommunication
{
public:
   // Creation/destruction
   SapCommHost(const char *localName, SapCommCallback pCallback, void *pContext = NULL);
   ~SapCommHost();

	BOOL Destroy();

   // Access to implementation
   void *GetUserBufAddress(SapCommServer *pServer);
   int GetUserBufSize(SapCommServer *pServer)      const;
   int GetCommBufSize()                            const { return m_commBufSize; }

   BOOL SetUserBuf( SapCommServer *pServer, void *pUserBuf, int userBufSize);
   BOOL SetCommBufSize( int commBufSize);

   // Add additional server to internal list
   SapCommServer *AddServer(SapLocation location, const char *commName, const char *commandLine = 0, void *userBufPtr= NULL, int userBufSize= 0);

   // Send internal server list to all servers
   BOOL UpdateServers();

	BOOL SendData( SapCommServer *pServer, void *pData, int nData);

protected:
   // Message callback management
   BOOL InternalCommand(int command, void *inData, int inDataSize, void *outData, int outDataSize);

	SapBuffer *    m_PcCommBuf[ SAPCOMM_MAX_SERVERS];
	SapCommBufInfo m_PcCommBufInfo[ SAPCOMM_MAX_SERVERS];
	SapCommBufInfo m_MbCommBufInfo[ SAPCOMM_MAX_SERVERS];
	int            m_commBufSize;
}; 

#endif	// _SAPCOMMHOST_H_
