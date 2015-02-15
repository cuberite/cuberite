
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
	
	#ifdef _WIN32
		WSAData wsa;
		int res = WSAStartup(0x0202, &wsa);
		if (res != 0)
		{
			printf("Cannot initialize WinSock: %d\n", res);
			return res;
		}
	#endif  // _WIN32
	
	puts("Generating protocol encryption keypair...");
	m_PrivateKey.Generate();
	m_PublicKeyDER = m_PrivateKey.GetPubKeyDER();

	m_ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_ListenSocket < 0)
	{
		#ifdef _WIN32
			int err = WSAGetLastError();
		#else
			int err = errno;
		#endif
		printf("Failed to create listener socket: %d\n", err);
		return err;
	}
	sockaddr_in local;
	memset(&local, 0, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = 130;  // INADDR_ANY;  // All interfaces
	local.sin_port = htons(a_ListenPort);
	if (!bind(m_ListenSocket, (sockaddr *)&local, sizeof(local)))
	{
		#ifdef _WIN32
			int err = WSAGetLastError();
		#else
			int err = errno;
		#endif
		printf("Failed to bind listener socket: %d\n", err);
		return err;
	}
	if (listen(m_ListenSocket, 1) != 0)
	{
		#ifdef _WIN32
			int err = WSAGetLastError();
		#else
			int err = errno;
		#endif
		printf("Failed to listen on socket: %d\n", err);
		return err;
	}
	
	printf("Listening on port %d, connecting to localhost:%d\n", a_ListenPort, a_ConnectPort);
	
	return 0;
}





void cServer::Run(void)
{
	puts("Server running.");
	while (true)
	{
		sockaddr_in Addr;
		memset(&Addr, 0, sizeof(Addr));
		socklen_t AddrSize = sizeof(Addr);
		SOCKET client = accept(m_ListenSocket, (sockaddr *)&Addr, &AddrSize);
		if (client == INVALID_SOCKET)
		{
			printf("accept returned an error: %d; bailing out.\n", SocketError);
			return;
		}
		puts("Client connected, proxying...");
		cConnection Connection(client, *this);
		Connection.Run();
		puts("Client disconnected. Ready for another connection.");
	}
}




