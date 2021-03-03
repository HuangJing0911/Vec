#ifndef _SAPCOMMUNICATION_H_
#define _SAPCOMMUNICATION_H_

//	SapCommunication.h : header file
//

#include "SapClassBasicDef.h"

// Constants
const int SAPCOMM_MAX_SERVERS         = 16;
const int SAPCOMM_TIMEOUT             = 60000;
const int SAPCOMM_DEFAULT_PACKET_SIZE = (16L * 1024);

//
// SapCommCallbackInfo class declaration
//
class SAPCLASSBASIC_CLASS SapCommCallbackInfo
{
public:
   SapCommCallbackInfo(int command, void *inData= NULL, int inDataSize= 0, void *outData= NULL, int outDataSize= 0, void *context= NULL)
   {
      m_command     = command;
      m_inData      = inData;
      m_inDataSize  = inDataSize;
      m_outData     = outData;
      m_outDataSize = outDataSize;
		m_context     = context;
   }
   ~SapCommCallbackInfo() {}

   int GetCommand()     const { return m_command; }
   void * GetInData()   const { return m_inData; }
   int GetInDataSize()  const { return m_inDataSize; }
   void * GetOutData()  const { return m_outData; }
   int GetOutDataSize() const { return m_outDataSize; }
	void * GetContext()  const { return m_context; }

protected:
   int    m_command;
   void * m_inData;
   int    m_inDataSize;
   void * m_outData;
   int    m_outDataSize;
	void * m_context;
};

typedef void (*SapCommCallback)(SapCommCallbackInfo *);

//
// SapCommServer class declaration
//
class SAPCLASSBASIC_CLASS SapCommServer : public SapManager
{
public:
   // Creation/destruction
   SapCommServer(SapLocation loc, const char *commName);	// Remote server
   SapCommServer(const char *commName);						// Local server
   virtual ~SapCommServer();

   SapLocation GetLocation()     const { return m_location; }
   const char *GetCommName()     const { return m_commName; }
   BOOL IsSystem()               const { return m_isSystem; }
   BOOL IsLocal()                const { return m_isLocal; }
	const CORSERVER GetServer()   const { return m_hCommServer; }

	int  GetIndex()             const { return m_index;}
	void SetIndex( int index)   { m_index= index;}

protected:
   SapLocation m_location;							// Server location (name or index), e.g., "System" or "Mamba_1"
   char        m_commName[CORSERVER_MAX_STRLEN];    // Communications server name, relative to main server
   CORSERVER   m_hCommServer;
   BOOL        m_isSystem;
   BOOL        m_isLocal;
	int         m_index;
};

class SAPCLASSBASIC_CLASS SapCommBufInfo
{
public:

	SapCommBufInfo( )
	{
		m_serverIndex    = 0;
		m_commBufPtr     = NULL;
		m_commBufSize    = 0;
		m_commBufPhysAddr= 0;
		m_userBufPtr     = NULL;
		m_userBufSize    = 0;
	}
	
	int	   GetServerIndex()         const { return m_serverIndex; }
	void*    GetCommBufPtr()          const { return m_commBufPtr; }
	int      GetCommBufSize()         const { return m_commBufSize; }
   ULONG_PTR GetCommBufPhysAddr()    const { return m_commBufPhysAddr;}
	void*		  GetUserBufPtr()			       { return m_userBufPtr; }
	int      GetUserBufSize()         const { return m_userBufSize; }

	void SetServerIndex( int serverIndex)            
	{ 
		m_serverIndex= serverIndex;
	}

   void SetCommBuf( void *commBufPtr, int commBufSize, ULONG_PTR commBufPhysAddr)
	{ 
		m_commBufPtr     = commBufPtr;
		m_commBufSize    = commBufSize;
		m_commBufPhysAddr= commBufPhysAddr;
	}
	
	void SetCommBufPtr( void *pData)
	{
		m_commBufPtr= pData;
	}

	void SetUserBuf( void *userBufPtr, int userBufSize)
	{  
		m_userBufPtr  = userBufPtr; 
		m_userBufSize= userBufSize; 
	}

private:
	int	m_serverIndex;
	void*	m_commBufPtr;
	int   m_commBufSize;
   ULONG_PTR m_commBufPhysAddr;
	void* m_userBufPtr;
	int   m_userBufSize;
};

class SAPCLASSBASIC_CLASS SapCommPacketInfo
{
public:

	SapCommPacketInfo()
	{
		m_serverIndex = 0;
		m_offset      = 0;
		m_packetSize  = 0;
		m_totalSize   = 0;
	}

	SapCommPacketInfo( int serverIndex, int offset, int packetSize, int totalSize)
	{
		m_serverIndex = serverIndex;
		m_offset      = offset;
		m_packetSize  = packetSize;
		m_totalSize   = totalSize;
	}

	int	GetServerIndex()		const { return m_serverIndex; }
	int	GetOffset()				const { return m_offset; }
	int	GetPacketSize()		const { return m_packetSize; }
	int   GetTotalSize()       const { return m_totalSize; }

private:
	int m_serverIndex;
	int m_offset;
	int m_packetSize;
	int m_totalSize;
};

class SAPCLASSBASIC_CLASS SapCommReceivePacketInfo
{
public:

	SapCommReceivePacketInfo( SapCommPacketInfo packetInfo, SapCommBufInfo commBufInfo)
	{
		m_packetInfo = packetInfo;
		m_commBufInfo= commBufInfo;
	}

	void*	GetCommBufPtr()		      { return m_commBufInfo.GetCommBufPtr();}
	int	GetCommBufSize()		const { return m_commBufInfo.GetCommBufSize();}
	void*	GetUserBufPtr()				{ return m_commBufInfo.GetUserBufPtr();}
	int	GetUserBufSize()		const { return m_commBufInfo.GetUserBufSize();}
	int	GetServerIndex()		const { return m_packetInfo.GetServerIndex();}
	int	GetOffset()				const { return m_packetInfo.GetOffset();}
	int	GetPacketSize()		const { return m_packetInfo.GetPacketSize();}
	int	GetTotalSize()			const { return m_packetInfo.GetTotalSize();}

private:
	SapCommPacketInfo		m_packetInfo;
	SapCommBufInfo			m_commBufInfo;
};

class SAPCLASSBASIC_CLASS SapCommInitCommBufInfo
{
public:

	SapCommInitCommBufInfo( int serverIndex, SapCommBufInfo commBufInfo)
	{
		m_serverIndex= serverIndex;
		m_commBufInfo= commBufInfo;
	}

	int	         GetServerIndex()		   const { return m_serverIndex; }
	SapCommBufInfo GetCommBufInfo()			const { return m_commBufInfo;}

private:
	int            m_serverIndex;
	SapCommBufInfo m_commBufInfo;
};

enum SAPCOMM_STANDARD_CMD
{
	SAPCOMM_RECEIVE_PACKET= 0xffe0,
	SAPCOMM_RECEIVE_DONE,
};

//
// SapCommunication class declaration
//
class SAPCLASSBASIC_CLASS SapCommunication : public SapManager
{
public:
   // Creation/destruction
   SapCommunication(const char *localName, SapCommCallback pCallback, void *pContext = NULL);
   virtual ~SapCommunication();
   virtual BOOL Create();
   virtual BOOL Destroy();

	// Access to implementation
   SapCommCallback GetCallback() const { return m_pCallback; }
   void *GetContext()				const { return m_pContext; }
   int GetNumServers()           const;

   SapCommServer *GetSystemServer() const;
   SapCommServer *GetLocalServer() const;
   SapCommServer *GetServer(int index) const;
   SapCommServer *GetServer(SapLocation loc) const;
   SapCommServer *GetServer(const char *commName) const;
	SapCommServer *GetServer(SapLocation loc, const char *commName) const;

	// Copy local file on remote location
	BOOL CopyFile( char *szFilenameSrc, SapLocation loc, char *szFilenameDst);
	BOOL CopyFile( SapLocation locSrc, char *szFilenameSrc, SapLocation locDst, char *szFilenameDst);

   // Send a user command to a server
   BOOL SendCommand(SapCommServer *pServer, SapCommCallbackInfo *pInfo);

protected:
   // Message callback management
   static SAPSTATUS CCONV OnServerCommand(UINT32 command, void *inData, UINT32 inDataSize, void *outData, UINT32 outDataSize);

	// Reset server information
   void ResetServerInfo();

   // Add additional server to internal list
	virtual SapCommServer *AddServer(SapLocation loc, const char *commName, void *userBufPtr= NULL, int userBufSize= 0);

   // Message callback management
   // Note: InternalCommand must be defined in derived classes
   virtual BOOL InternalCommand(int command, void *inData, int inDataSize, void *outData, int outDataSize) = 0;

   enum SAPCOMM_CUSTOM_CMD
   {
      SAPCOMM_ADD_SERVER = 0xfff0,
      SAPCOMM_ADD_SERVER_ACK,
		SAPCOMM_INIT_SERVER,
		SAPCOMM_SEND_PACKET,
		SAPCOMM_SEND_DONE,
		SAPCOMM_INIT_COMMBUFINFO,
   };

   HANDLE m_syncEvent;
   char   m_localName[CORSERVER_MAX_STRLEN];

   static SapCommCallback	 m_pCallback;    // Application callback function
	static void					*m_pContext;
   static SapCommunication *m_pComm;      // For saving 'this' pointer

   int            m_numServers;
   SapCommServer *m_servers[SAPCOMM_MAX_SERVERS];
   int            m_systemIndex;
   int            m_localIndex;

private:
   static int m_numInstances;           // Number of class instances
   int        m_localInstance;          // Local instance number
}; 

#endif	// _SAPCOMMUNICATION_H_
