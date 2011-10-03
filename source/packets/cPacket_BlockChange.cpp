#include "cPacket_BlockChange.h"

bool cPacket_BlockChange::Send(cSocket & a_Socket)
{
	unsigned int TotalSize = c_Size;
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte	 ( (char)m_PacketID,	Message, i );
	AppendInteger( m_PosX,		Message, i );
	AppendByte	 ( m_PosY,		Message, i );
	AppendInteger( m_PosZ,		Message, i );
	AppendByte   ( m_BlockType,	Message, i );
	AppendByte   ( m_BlockMeta,	Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}