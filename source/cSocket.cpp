#include "cSocket.h"

#ifndef _WIN32
#include <netdb.h>
#include <unistd.h>
#endif

cSocket::cSocket( xSocket a_Socket )
	: m_Socket( a_Socket )
{
}

cSocket::~cSocket()
{
}

cSocket::operator const cSocket::xSocket() const
{
	return m_Socket;
}

cSocket::xSocket cSocket::GetSocket() const
{
	return m_Socket;
}

bool cSocket::IsValid()
{
#ifdef _WIN32
	return ( m_Socket != INVALID_SOCKET);
#else
	return ( m_Socket >= 0);
#endif
}

void cSocket::CloseSocket()
{
#ifdef _WIN32
	closesocket(m_Socket);
#else
	shutdown(m_Socket, SHUT_RDWR);//SD_BOTH);
	close(m_Socket);
#endif
}