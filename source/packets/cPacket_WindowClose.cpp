#include "cPacket_WindowClose.h"

bool cPacket_WindowClose::Parse(cSocket & a_Socket)
{
	//LOG("CLOSE INVENTORY PACKET");
	m_Socket = a_Socket;

	if( !ReadByte(m_Close) ) return false;

	//LOG("Closed inventory?: %i", m_Close );

	return true;
}

bool cPacket_WindowClose::Send(cSocket & a_Socket)
{
	unsigned int TotalSize = c_Size;
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte	 ( (char)m_PacketID,	Message, i );
	AppendByte	( m_Close,		Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}