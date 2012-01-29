
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cBlockingTCPLink.h"
#include "packets/cPacket.h"
#include "MCSocket.h"

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

cBlockingTCPLink::cBlockingTCPLink()
	: m_Socket( 0 )
{
}

cBlockingTCPLink::~cBlockingTCPLink()
{
	CloseSocket();
}

void cBlockingTCPLink::CloseSocket()
{
	if( m_Socket )
	{
		m_Socket.CloseSocket();
		m_Socket = 0;
	}
}

bool cBlockingTCPLink::Connect( const char* a_Address, unsigned int a_Port )
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

	return true;
}

int cBlockingTCPLink::Send( char* a_Data, unsigned int a_Size, int a_Flags /* = 0 */ )
{
	if( !m_Socket )
	{
		LOGWARN("cBlockingTCPLink: Trying to send data without a valid connection!");
		return -1;
	}
	return cPacket::SendData( m_Socket, a_Data, a_Size, a_Flags );
}

int cBlockingTCPLink::SendMessage( const char* a_Message, int a_Flags /* = 0 */ )
{
	if( !m_Socket )
	{
		LOGWARN("cBlockingTCPLink: Trying to send message without a valid connection!");
		return -1;
	}
	return cPacket::SendData( m_Socket, a_Message, strlen(a_Message), a_Flags );
}

std::string cBlockingTCPLink::ReceiveData()
{
	if( !m_Socket ) return "";

	int Received = 0;
	char Buffer[256];
	std::string Data;
	while( (Received = recv(m_Socket, Buffer, 256, 0) ) > 0 )
	{
		//LOGINFO("Recv: %i", Received);
		//LOG("%s", Buffer );
		Data.append( Buffer, Received );
		memset( Buffer, 0, 256 );
	}

	//LOGINFO("Received returned: %i", Received );
	return Data;
}
