
// Server.h

// Interfaces to the cServer class encapsulating the entire "server"





#pragma once






class cServer
{
	SOCKET m_ListenSocket;
	RSA::PrivateKey m_PrivateKey;
	RSA::PublicKey  m_PublicKey;
	short m_ConnectPort;
	
public:
	cServer(void);
	
	int  Init(short a_ListenPort, short a_ConnectPort);
	void Run(void);
	
	RSA::PrivateKey & GetPrivateKey(void) { return m_PrivateKey; }
	RSA::PublicKey  & GetPublicKey (void) { return m_PublicKey; }
	
	short GetConnectPort(void) const { return m_ConnectPort; }
} ;




