
// Server.h

// Interfaces to the cServer class encapsulating the entire "server"





#pragma once

#include "mbedTLS++/RsaPrivateKey.h"






class cServer
{
	SOCKET m_ListenSocket;
	cRsaPrivateKey m_PrivateKey;
	AString m_PublicKeyDER;
	UInt16 m_ConnectPort;

public:
	cServer(void);

	int  Init(UInt16 a_ListenPort, UInt16 a_ConnectPort);
	void Run(void);

	cRsaPrivateKey & GetPrivateKey(void) { return m_PrivateKey; }
	const AString & GetPublicKeyDER (void) { return m_PublicKeyDER; }

	UInt16 GetConnectPort(void) const { return m_ConnectPort; }
} ;
