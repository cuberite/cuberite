
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BlockingTCPLink.h"
#include "Errors.h"




cBlockingTCPLink::cBlockingTCPLink(void)
{
}





cBlockingTCPLink::~cBlockingTCPLink()
{
	CloseSocket();
}





void cBlockingTCPLink::CloseSocket()
{
	if (!m_Socket.IsValid())
	{
		m_Socket.CloseSocket();
	}
}





bool cBlockingTCPLink::Connect(const char * iAddress, unsigned int iPort)
{
	ASSERT(!m_Socket.IsValid());
	if (m_Socket.IsValid())
	{
		LOGWARN("WARNING: cTCPLink Connect() called while still connected.");
		m_Socket.CloseSocket();
	}

	struct hostent *hp;
	unsigned int addr;
	struct sockaddr_in server;

	m_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (!m_Socket.IsValid())
	{
		LOGERROR("cTCPLink: Cannot create a socket");
		return false;
	}

	addr = inet_addr(iAddress);
	hp = gethostbyaddr((char *)&addr, sizeof(addr), AF_INET);
	if (hp == NULL)
	{
		//LOGWARN("cTCPLink: gethostbyaddr returned NULL");
		hp = gethostbyname(iAddress);
		if (hp == NULL)
		{
			LOGWARN("cTCPLink: Could not resolve %s", iAddress);
			CloseSocket();
			return false;
		}
	}

	server.sin_addr.s_addr = *((unsigned long *)hp->h_addr);
	server.sin_family = AF_INET;
	server.sin_port = htons( (unsigned short)iPort);
	if (connect(m_Socket, (struct sockaddr *)&server, sizeof(server)))
	{
		LOGWARN("cTCPLink: Connection to \"%s:%d\" failed (%s)", iAddress, iPort,GetOSErrorString( cSocket::GetLastError() ).c_str() );
		CloseSocket();
		return false;
	}

	return true;
}





int cBlockingTCPLink::Send(char * a_Data, unsigned int a_Size, int a_Flags /* = 0 */ )
{
	ASSERT(m_Socket.IsValid());
	if (!m_Socket.IsValid())
	{
		LOGERROR("cBlockingTCPLink: Trying to send data without a valid connection!");
		return -1;
	}
	return m_Socket.Send(a_Data, a_Size);
}





int cBlockingTCPLink::SendMessage( const char* a_Message, int a_Flags /* = 0 */ )
{
	ASSERT(m_Socket.IsValid());
	if (!m_Socket.IsValid())
	{
		LOGWARN("cBlockingTCPLink: Trying to send message without a valid connection!");
		return -1;
	}
	return m_Socket.Send(a_Message, strlen(a_Message));
}





void cBlockingTCPLink::ReceiveData(AString & oData)
{
	ASSERT(m_Socket.IsValid());
	if (!m_Socket.IsValid())
	{
		return;
	}

	int Received = 0;
	char Buffer[256];
	while ((Received = recv(m_Socket, Buffer, sizeof(Buffer), 0)) > 0)
	{
		oData.append(Buffer, Received);
	}
}




