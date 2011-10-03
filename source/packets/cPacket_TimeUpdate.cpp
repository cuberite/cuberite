#include "cPacket_TimeUpdate.h"

bool cPacket_TimeUpdate::Parse(cSocket & a_Socket)
{
	m_Socket = a_Socket;
	if( !ReadLong(m_Time) ) return false;
	return true;
}

bool cPacket_TimeUpdate::Send(cSocket & a_Socket)
{
	unsigned int TotalSize = c_Size;
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte	( (char)m_PacketID, Message, i );
	AppendLong	( m_Time, Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}