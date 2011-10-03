#include "cTCPLink.h"
#include "cSocket.h"
#include "cEvent.h"
#include "cThread.h"

#include "cMCLogger.h"

#ifndef _WIN32
#include <cstring>
#include <errno.h>
#endif

#ifdef _WIN32
#define MSG_NOSIGNAL (0)
#endif
#ifdef __MACH__
#define MSG_NOSIGNAL (0)
#endif

cTCPLink::cTCPLink()
	: m_Socket( 0 )
	, m_StopEvent( new cEvent() )
{
}

cTCPLink::~cTCPLink()
{
	if( m_Socket )
	{
		CloseSocket();
		m_StopEvent->Wait();
	}
	delete m_StopEvent;
}

void cTCPLink::CloseSocket()
{
	if( m_Socket )
	{
		closesocket( m_Socket );
		m_Socket = 0;
	}
}

bool cTCPLink::Connect( const char* a_Address, unsigned int a_Port )
{
	if( m_Socket )
	{
		LOGWARN("WARNING: cTCPLink Connect() called while still connected. ALWAYS disconnect before re-connecting!");
	}

	struct hostent *hp;
	unsigned int addr;
	struct sockaddr_in server;

#ifdef _WIN32
	WSADATA wsaData;
	int wsaret=WSAStartup(/*0x101*/ MAKEWORD(2, 2),&wsaData);

	if(wsaret!=0)
	{
		LOGERROR("cTCPLink: WSAStartup returned error");
		return false;
	}
#endif

	m_Socket=socket(AF_INET,SOCK_STREAM,0);
#ifdef _WIN32
	if( m_Socket==INVALID_SOCKET )
#else
    if( m_Socket < 0 )
#endif
	{
		LOGERROR("cTCPLink: Invalid socket");
		m_Socket = 0;
		return false;
	}


	addr=inet_addr( a_Address );
	hp=gethostbyaddr((char*)&addr,sizeof(addr),AF_INET);
	if(hp==NULL)
	{
		//LOGWARN("cTCPLink: gethostbyaddr returned NULL");
		hp = gethostbyname( a_Address );
		if( hp == NULL )
		{
			LOGWARN("cTCPLink: Could not resolve %s", a_Address);
			CloseSocket();
			return false;
		}
	}

	server.sin_addr.s_addr=*((unsigned long*)hp->h_addr);
	server.sin_family=AF_INET;
	server.sin_port=htons( (unsigned short)a_Port );
	if( connect( m_Socket, (struct sockaddr*)&server, sizeof(server) ) )
	{
		LOGWARN("cTCPLink: No response from server (%i)", errno);
		CloseSocket();
		return false;
	}

	cThread( ReceiveThread, this );

	return true;
}

int cTCPLink::Send( char* a_Data, unsigned int a_Size, int a_Flags /* = 0 */ )
{
	//LOG("TCPLink::Send()");
	if( !m_Socket )
	{
		LOGWARN("cTCPLink: Trying to send data without a valid connection!");
		return -1;
	}
	return send( m_Socket, a_Data, a_Size, a_Flags | MSG_NOSIGNAL );
}

int cTCPLink::SendMessage( const char* a_Message, int a_Flags /* = 0 */ )
{
	//LOG("TCPLink::SendMessage()");
	if( !m_Socket )
	{
		LOGWARN("cTCPLink: Trying to send message without a valid connection!");
		return -1;
	}
	return send( m_Socket, a_Message, strlen(a_Message), a_Flags | MSG_NOSIGNAL );
}

void cTCPLink::ReceiveThread( void* a_Param)
{
	cTCPLink* self = (cTCPLink*)a_Param;
	SOCKET Socket = self->m_Socket;
	int Received = 0;
	do
	{
		char Data[256];
		Received = recv(Socket, Data, 256, 0);
		self->ReceivedData( Data, (Received>0?Received:-1) );
	} while ( Received > 0 );

	LOGINFO("cTCPLink Disconnected (%i)", Received );

	if( Socket == self->m_Socket ) self->m_StopEvent->Set();
}
