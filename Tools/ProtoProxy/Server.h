
// Server.h

// Interfaces to the cServer class encapsulating the entire "server"





#pragma once

#include "PolarSSL++/RsaPrivateKey.h"






class cServer
{
	SOCKET m_ListenSocket;
	cRsaPrivateKey m_PrivateKey;
	AString m_PublicKeyDER;
	short m_ConnectPort;
	
public:
	cServer(void);
	
	int  Init(short a_ListenPort, short a_ConnectPort);
	void Run(void);
	
	cRsaPrivateKey & GetPrivateKey(void) { return m_PrivateKey; }
	const AString & GetPublicKeyDER (void) { return m_PublicKeyDER; }
	
	short GetConnectPort(void) const { return m_ConnectPort; }
} ;




