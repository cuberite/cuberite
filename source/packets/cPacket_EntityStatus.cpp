#include "cPacket_EntityStatus.h"

bool cPacket_EntityStatus::Send(cSocket & a_Socket)
{
	unsigned int TotalSize = c_Size;

	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte		( (char)m_PacketID,	Message, i );
	AppendInteger	( m_UniqueID,	Message, i );
	AppendByte		( m_Status,		Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}