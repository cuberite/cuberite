#include "cPacket_Chat.h"


bool cPacket_Chat::Parse( cSocket & a_Socket )
{
	m_Socket = a_Socket;
	if( !ReadString16( m_Message ) ) return false;
	return true;
}

bool cPacket_Chat::Send( cSocket & a_Socket )
{
	unsigned int TotalSize = c_Size + m_Message.size() * sizeof( short );
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte	 ( (char)m_PacketID,	Message, i );
	AppendString16 ( m_Message,	Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}