
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "TCPLink.h"





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
		m_Socket.CloseSocket();
		m_Socket = 0;
	}
}

bool cTCPLink::Connect( const AString & a_Address, unsigned int a_Port )
{
	if( m_Socket )
	{
		LOGWARN("WARNING: cTCPLink Connect() called while still connected. ALWAYS disconnect before re-connecting!");
	}

	m_Socket = cSocket::CreateSocket();
	if( !m_Socket.IsValid() )
	{
		LOGERROR("cTCPLink: Failed to create socket");
		return false;
	}

	if (m_Socket.Connect(a_Address, a_Port) != 0)
	{
		LOGWARN("cTCPLink: Cannot connect to server \"%s\" (%s)", m_Socket.GetLastErrorString().c_str());
		m_Socket.CloseSocket();
		return false;
	}

	cThread( ReceiveThread, this );

	return true;
}





int cTCPLink::Send(const char * a_Data, unsigned int a_Size, int a_Flags /* = 0 */ )
{
	(void)a_Flags;
	if (!m_Socket.IsValid())
	{
		LOGWARN("cTCPLink: Trying to send data without a valid connection!");
		return -1;
	}
	return m_Socket.Send(a_Data, a_Size);
}





int cTCPLink::SendMessage(const char * a_Message, int a_Flags /* = 0 */ )
{
	(void)a_Flags;
	if (!m_Socket.IsValid())
	{
		LOGWARN("cTCPLink: Trying to send message without a valid connection!");
		return -1;
	}
	return m_Socket.Send(a_Message, strlen(a_Message));
}





void cTCPLink::ReceiveThread( void* a_Param)
{
	cTCPLink* self = (cTCPLink*)a_Param;
	cSocket Socket = self->m_Socket;
	int Received = 0;
	do
	{
		char Data[256];
		Received = Socket.Receive(Data, sizeof(Data), 0);
		self->ReceivedData( Data, ((Received > 0) ? Received : -1) );
	} while ( Received > 0 );

	LOGINFO("cTCPLink Disconnected (%i)", Received );

	if (Socket == self->m_Socket)
	{
		self->m_StopEvent->Set();
	}
}




