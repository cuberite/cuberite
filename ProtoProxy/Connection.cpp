
// Connection.cpp

// Interfaces to the cConnection class representing a single pair of connected sockets

#include "Globals.h"
#include "Connection.h"
#include "Server.h"





cConnection::cConnection(SOCKET a_ClientSocket, cServer & a_Server) :
	m_Server(a_Server),
	m_LogFile(NULL),
	m_ClientSocket(a_ClientSocket),
	m_ServerSocket(-1),
	m_BeginTick(clock())
{
	AString fnam;
	Printf(fnam, "Log_%d.log", (int)time(NULL));
	m_LogFile = fopen(fnam.c_str(), "w");
	Log("Log file created");
}





cConnection::~cConnection()
{
	fclose(m_LogFile);
}





void cConnection::Run(void)
{
	if (!ConnectToServer())
	{
		Log("Cannot connect to server; aborting");
		return;
	}
	
	while (true)
	{
		fd_set ReadFDs;
		FD_ZERO(&ReadFDs);
		FD_SET(m_ServerSocket, &ReadFDs);
		FD_SET(m_ClientSocket, &ReadFDs);
		int res = select(2, &ReadFDs, NULL, NULL, NULL);
		if (res <= 0)
		{
			printf("select() failed: %d; aborting client", WSAGetLastError());
			return;
		}
		if (FD_ISSET(m_ServerSocket, &ReadFDs))
		{
			if (!RelayFromServer())
			{
				return;
			}
		}
		if (FD_ISSET(m_ClientSocket, &ReadFDs))
		{
			if (!RelayFromClient())
			{
				return;
			}
		}
	}
}





void cConnection::Log(const char * a_Format, ...)
{
	va_list args;
	va_start(args, a_Format);
	AString msg;
	AppendVPrintf(msg, a_Format, args);
	va_end(args);
	AString FullMsg;
	Printf(FullMsg, "[%5.3f] %s\n", GetRelativeTime(), msg.c_str());
	
	cCSLock Lock(m_CSLog);
	fputs(FullMsg.c_str(), m_LogFile);
}





void cConnection::DataLog(const void * a_Data, int a_Size, const char * a_Format, ...)
{
	va_list args;
	va_start(args, a_Format);
	AString msg;
	AppendVPrintf(msg, a_Format, args);
	va_end(args);
	AString FullMsg;
	AString Hex;
	Printf(FullMsg, "[%5.3f] %s\n%s", GetRelativeTime(), msg.c_str(), CreateHexDump(Hex, a_Data, a_Size, 16).c_str());
	
	cCSLock Lock(m_CSLog);
	fputs(FullMsg.c_str(), m_LogFile);
}





bool cConnection::ConnectToServer(void)
{
	m_ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_ServerSocket == INVALID_SOCKET)
	{
		return false;
	}
	sockaddr_in localhost;
	localhost.sin_family = AF_INET;
	localhost.sin_port = htons(m_Server.GetConnectPort());
	localhost.sin_addr.s_addr = htonl(0x7f000001);  // localhost
	if (connect(m_ServerSocket, (sockaddr *)&localhost, sizeof(localhost)) != 0)
	{
		printf("connection to server failed: %d\n", WSAGetLastError());
		return false;
	}
	return true;
}





bool cConnection::RelayFromServer(void)
{
	char Buffer[1024];
	int res = recv(m_ServerSocket, Buffer, sizeof(Buffer), 0);
	if (res <= 0)
	{
		Log("Server closed the socket: %d; %d; aborting connection", res, WSAGetLastError());
		return false;
	}
	
	DataLog(Buffer, res, "Received %d bytes from the server", res);
	// TODO: Process the data
	
	res = send(m_ClientSocket, Buffer, res, 0);
	if (res <= 0)
	{
		Log("Client closed the socket: %d, %d; aborting connection", res, WSAGetLastError());
		return false;
	}
	
	return true;
}





bool cConnection::RelayFromClient(void)
{
	char Buffer[1024];
	int res = recv(m_ClientSocket, Buffer, sizeof(Buffer), 0);
	if (res <= 0)
	{
		Log("Client closed the socket: %d; %d; aborting connection", res, WSAGetLastError());
		return false;
	}
	
	DataLog(Buffer, res, "Received %d bytes from the client", res);
	// TODO: Process the data
	
	res = send(m_ServerSocket, Buffer, res, 0);
	if (res <= 0)
	{
		Log("Server closed the socket: %d, %d; aborting connection", res, WSAGetLastError());
		return false;
	}
	
	return true;
}





double cConnection::GetRelativeTime(void)
{
	return (double)(clock() - m_BeginTick) / CLOCKS_PER_SEC;
	
}




