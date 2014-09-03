
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Socket.h"

#ifndef _WIN32
	#include <netdb.h>
	#include <unistd.h>
	#include <arpa/inet.h>  // inet_ntoa()
	#include <sys/ioctl.h>  // ioctl()
#else
	#define socklen_t int
#endif





cSocket::cSocket(xSocket a_Socket)
	: m_Socket(a_Socket)
{
}





cSocket::operator cSocket::xSocket() const
{
	return m_Socket;
}





cSocket::xSocket cSocket::GetSocket() const
{
	return m_Socket;
}





bool cSocket::IsValidSocket(cSocket::xSocket a_Socket)
{
	#ifdef _WIN32
	return (a_Socket != INVALID_SOCKET);
	#else  // _WIN32
	return (a_Socket >= 0);
	#endif  // else _WIN32
}





void cSocket::CloseSocket()
{
	#ifdef _WIN32
	
	closesocket(m_Socket);
	
	#else  // _WIN32
	
	if (close(m_Socket) != 0)
	{
		LOGWARN("Error closing socket %d (%s): %s", m_Socket, m_IPString.c_str(), GetLastErrorString().c_str());
	}
	
	#endif  // else _WIN32

	// Invalidate the socket so that this object can be re-used for another connection
	m_Socket = INVALID_SOCKET;
}





void cSocket::ShutdownReadWrite(void)
{
	#ifdef _WIN32
		int res = shutdown(m_Socket, SD_BOTH);
	#else
		int res = shutdown(m_Socket, SHUT_RDWR);
	#endif
	if (res != 0)
	{
		LOGWARN("%s: Error shutting down socket %d (%s): %d (%s)",
			__FUNCTION__, m_Socket, m_IPString.c_str(), this->GetLastError(), GetLastErrorString().c_str()
		);
	}
}




int cSocket::GetLastError()
{
#ifdef _WIN32
	return WSAGetLastError();
#else
	return errno;
#endif
}





bool cSocket::SetReuseAddress(void)
{
	#if defined(_WIN32) || defined(ANDROID_NDK)
		char yes = 1;
	#else
		int yes = 1;
	#endif
	return (setsockopt(m_Socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == 0);
}





int cSocket::WSAStartup(void)
{
#ifdef _WIN32
	WSADATA wsaData;
	memset(&wsaData, 0, sizeof(wsaData));
	return ::WSAStartup(MAKEWORD(2, 2), &wsaData);
#else
	return 0;
#endif
}





cSocket cSocket::CreateSocket(eFamily a_Family)
{
	return socket((int)a_Family, SOCK_STREAM, 0);
}





bool cSocket::BindToAnyIPv4(unsigned short a_Port)
{
	sockaddr_in local;
	memset(&local, 0, sizeof(local));

	local.sin_family = AF_INET;
	local.sin_port = htons((u_short)a_Port);

	return (bind(m_Socket, (sockaddr *)&local, sizeof(local)) == 0);
}





bool cSocket::BindToAnyIPv6(unsigned short a_Port)
{
	sockaddr_in6 local;
	memset(&local, 0, sizeof(local));

	local.sin6_family = AF_INET6;
	local.sin6_port = htons((u_short)a_Port);

	return (bind(m_Socket, (sockaddr *)&local, sizeof(local)) == 0);
}





bool cSocket::BindToLocalhostIPv4(unsigned short a_Port)
{
	sockaddr_in local;
	memset(&local, 0, sizeof(local));

	local.sin_family = AF_INET;;
	local.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	local.sin_port = htons((u_short)a_Port);

	return (bind(m_Socket, (sockaddr*)&local, sizeof(local)) == 0);
}





bool cSocket::Listen(int a_Backlog)
{
	return (listen(m_Socket, a_Backlog) == 0);
}





cSocket cSocket::AcceptIPv4(void)
{
	sockaddr_in from;
	socklen_t fromlen = sizeof(from);

	cSocket SClient = accept(m_Socket, (sockaddr *)&from, &fromlen);

	if (SClient.IsValid() && (from.sin_addr.s_addr != 0))  // Get IP in string form
	{
		SClient.m_IPString = inet_ntoa(from.sin_addr);
	}
	return SClient;
}





cSocket cSocket::AcceptIPv6(void)
{
	sockaddr_in6 from;
	socklen_t fromlen = sizeof(from);

	cSocket SClient = accept(m_Socket, (sockaddr *)&from, &fromlen);

	// Get IP in string form:
	if (SClient.IsValid())
	{
		#if defined(_WIN32)
			// Windows XP doesn't have inet_ntop, so we need to improvise. And MSVC has different headers than GCC
			#ifdef _MSC_VER
				// MSVC version
				Printf(SClient.m_IPString, "%x:%x:%x:%x:%x:%x:%x:%x",
					from.sin6_addr.u.Word[0],
					from.sin6_addr.u.Word[1],
					from.sin6_addr.u.Word[2],
					from.sin6_addr.u.Word[3],
					from.sin6_addr.u.Word[4],
					from.sin6_addr.u.Word[5],
					from.sin6_addr.u.Word[6],
					from.sin6_addr.u.Word[7]
				);
			#else  // _MSC_VER
				// MinGW
				Printf(SClient.m_IPString, "%x:%x:%x:%x:%x:%x:%x:%x",
					from.sin6_addr.s6_addr16[0],
					from.sin6_addr.s6_addr16[1],
					from.sin6_addr.s6_addr16[2],
					from.sin6_addr.s6_addr16[3],
					from.sin6_addr.s6_addr16[4],
					from.sin6_addr.s6_addr16[5],
					from.sin6_addr.s6_addr16[6],
					from.sin6_addr.s6_addr16[7]
				);
			#endif  // else _MSC_VER
		#else
			char buffer[INET6_ADDRSTRLEN];
			inet_ntop(AF_INET6, &(from.sin6_addr), buffer, sizeof(buffer));
			SClient.m_IPString.assign(buffer);
		#endif  // _WIN32
	}
	return SClient;
}





bool cSocket::ConnectToLocalhostIPv4(unsigned short a_Port)
{
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	server.sin_port = htons(a_Port);
	return (connect(m_Socket, (sockaddr *)&server, sizeof(server)) == 0);
}





bool cSocket::ConnectIPv4(const AString & a_HostNameOrAddr, unsigned short a_Port)
{
	// First try IP Address string to hostent conversion, because it's faster and local:
	unsigned long addr = inet_addr(a_HostNameOrAddr.c_str());
	if (addr == INADDR_NONE)
	{
		// It is not an IP Address string, but rather a regular hostname, resolve:
		hostent * hp = gethostbyname(a_HostNameOrAddr.c_str());
		if (hp == NULL)
		{
			LOGWARNING("%s: Could not resolve hostname \"%s\"", __FUNCTION__, a_HostNameOrAddr.c_str());
			CloseSocket();
			return false;
		}
		memcpy(&addr, hp->h_addr, hp->h_length);
	}

	// If the socket is not created yet, create one:
	if (!IsValid())
	{
		m_Socket = socket((int)IPv4, SOCK_STREAM, 0);
	}
	
	// Connect the socket:
	sockaddr_in server;
	server.sin_addr.s_addr = addr;
	server.sin_family = AF_INET;
	server.sin_port = htons((unsigned short)a_Port);
	return (connect(m_Socket, (sockaddr *)&server, sizeof(server)) == 0);
}





int cSocket::Receive(char * a_Buffer, size_t a_Length, unsigned int a_Flags)
{
	return recv(m_Socket, a_Buffer, (int)a_Length, a_Flags);
}





int cSocket::Send(const char * a_Buffer, size_t a_Length)
{
	return send(m_Socket, a_Buffer, (int)a_Length, MSG_NOSIGNAL);
}





unsigned short cSocket::GetPort(void) const
{
	ASSERT(IsValid());
	
	sockaddr_in Addr;
	socklen_t AddrSize = sizeof(Addr);
	if (getsockname(m_Socket, (sockaddr *)&Addr, &AddrSize) != 0)
	{
		return 0;
	}
	return ntohs(Addr.sin_port);
}





void cSocket::SetNonBlocking(void)
{
	#ifdef _WIN32
		u_long NonBlocking = 1;
		int res = ioctlsocket(m_Socket, FIONBIO, &NonBlocking);
	#else
		int NonBlocking = 1;
		int res = ioctl(m_Socket, FIONBIO, (char *)&NonBlocking);
	#endif
	if (res != 0)
	{
		LOGERROR("Cannot set socket to non-blocking. This would make the server deadlock later on, aborting.\nErr: %d, %d, %s",
			res, GetLastError(), GetLastErrorString().c_str()
		);
		abort();
	}
}




