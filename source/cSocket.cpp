#include "cSocket.h"

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

