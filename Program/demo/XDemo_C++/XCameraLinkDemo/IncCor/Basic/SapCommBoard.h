#ifndef _SAPCOMMBOARD_H_
#define _SAPCOMMBOARD_H_

//	SapCommBoard.h : header file
//

#include "SapClassBasicDef.h"

//
// SapCommBoard class declaration
//
class SAPCLASSBASIC_CLASS SapCommBoard : public SapCommunication
{
public:
   // Creation/destruction
   SapCommBoard(const char *localName, SapCommCallback pCallback, void *pContext = NULL, void *userBufPtr= NULL, int userBufSize= 0);
   ~SapCommBoard();

   // Access to implementation
   void *GetUserBufAddress()       { return m_MbCommBufInfo.GetUserBufPtr(); }
   int GetUserBufSize()      const { return m_MbCommBufInfo.GetUserBufSize(); }

   BOOL SetUserBuf( void *pUserBuf, int userBufSize);

   BOOL SendData( void *pData, int nData);

protected:
   // Message callback management
   BOOL InternalCommand(int command, void *inData, int inDataSize, void *outData, int outDataSize);

	SapBuffer*     m_MbCommBuf;
	SapCommBufInfo m_MbCommBufInfo;
	SapCommBufInfo m_PcCommBufInfo;

	int				m_serverIndex;
}; 

#endif	// _SAPCOMMBOARD_H_
