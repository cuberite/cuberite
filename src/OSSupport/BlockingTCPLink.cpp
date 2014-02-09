
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
		LOGWARN("WARNING: cBlockingTCPLink Connect() called while still connected.");
		m_Socket.CloseSocket();
	}
	
	m_Socket = cSocket::CreateSocket(cSocket::IPv4);

	if (!m_Socket.IsValid())
	{
		LOGERROR("cBlockingTCPLink: Cannot create a socket");
		return false;
	}
	if (!m_Socket.ConnectIPv4(iAddress,iPort))
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




