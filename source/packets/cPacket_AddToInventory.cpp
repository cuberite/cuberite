#include "cPacket_AddToInventory.h"

bool cPacket_AddToInventory::Send( cSocket & a_Socket )
{
	unsigned int TotalSize = c_Size;
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte	 ( (char)m_PacketID,	Message, i );
	AppendShort	 ( m_ItemType,	Message, i );
	AppendByte	 ( m_Count,		Message, i );
	AppendShort	 ( m_Life,		Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}