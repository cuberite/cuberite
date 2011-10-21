#pragma once

#ifdef _WIN32
#include <WinSock.h>
#define socklen_t int
#ifdef SendMessage
#undef SendMessage
#endif
#endif	// _WIN32


class cSocket
{
#ifdef _WIN32
	typedef SOCKET xSocket;
#else
	typedef int xSocket;
#endif

public:
	cSocket() : m_Socket( 0 ) {}

	cSocket( xSocket a_Socket );
	~cSocket();

	bool IsValid();
	void CloseSocket();

	operator const xSocket() const;
	xSocket GetSocket() const;
	void SetSocket( xSocket a_Socket );

	inline static bool IsSocketError( int a_ReturnedValue )
	{
#ifdef _WIN32
		return (a_ReturnedValue == SOCKET_ERROR || a_ReturnedValue == 0);
#else
		return (a_ReturnedValue <= 0);
#endif
	}

	
private:
	xSocket m_Socket;
};