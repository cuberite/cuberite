
// Server.cpp

// Interfaces to the cServer class encapsulating the entire "server"

#include "Globals.h"
#include "Server.h"
#include "Connection.h"





cServer::cServer(void)
{
}





int cServer::Init(short a_ListenPort, short a_ConnectPort)
{
	m_ConnectPort = a_ConnectPort;
	WSAData wsa;
	int res = WSAStartup(0x0202, &wsa);
	if (res != 0)
	{
		printf("Cannot initialize WinSock: %d\n", res);
		return res;
	}
	
	printf("Generating protocol encryption keypair...\n");
	time_t CurTime = time(NULL);
	RandomPool rng;
	rng.Put((const byte *)&CurTime, sizeof(CurTime));
	m_PrivateKey.GenerateRandomWithKeySize(rng, 1024);
	RSA::PublicKey pk(m_PrivateKey);
	m_PublicKey = pk;

	m_ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in local;
	memset(&local, 0, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = 0;  // All interfaces
	local.sin_port = htons(a_ListenPort);
	bind(m_ListenSocket, (sockaddr *)&local, sizeof(local));
	listen(m_ListenSocket, 1);
	
	printf("Listening on port %d, connecting to localhost:%d\n", a_ListenPort, a_ConnectPort);
	
	return 0;
}





void cServer::Run(void)
{
	printf("Server running.\n");
	while (true)
	{
		sockaddr_in Addr;
		ZeroMemory(&Addr, sizeof(Addr));
		int AddrSize = sizeof(Addr);
		SOCKET client = accept(m_ListenSocket, (sockaddr *)&Addr, &AddrSize);
		if (client == INVALID_SOCKET)
		{
			printf("accept returned an error: %d; bailing out.\n", WSAGetLastError());
			return;
		}
		printf("Client connected, proxying...\n");
		cConnection Connection(client, *this);
		Connection.Run();
		printf("Client disconnected. Ready for another connection.\n");
	}
}




