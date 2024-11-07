
// Server.h

// Interfaces to the cServer class encapsulating the entire "server"





#pragma once

#include "Globals.h"
#include "mbedTLS++/RsaPrivateKey.h"

#ifdef _WIN32
	#define SocketError WSAGetLastError()
#else
	typedef int SOCKET;
	enum
	{
		INVALID_SOCKET = -1,
	};
	#define closesocket close
	#define SocketError errno
#endif





class cServer
{
	SOCKET m_ListenSocket;
	cRsaPrivateKey m_PrivateKey;
	ContiguousByteBuffer m_PublicKeyDER;
	UInt16 m_ConnectPort;

public:
	cServer(void);

	int  Init(UInt16 a_ListenPort, UInt16 a_ConnectPort);
	void Run(void);

	cRsaPrivateKey & GetPrivateKey(void) { return m_PrivateKey; }
	ContiguousByteBufferView GetPublicKeyDER (void) { return m_PublicKeyDER; }

	UInt16 GetConnectPort(void) const { return m_ConnectPort; }
} ;
