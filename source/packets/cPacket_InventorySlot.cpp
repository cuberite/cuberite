#include "cPacket_InventorySlot.h"

bool cPacket_InventorySlot::Send(cSocket & a_Socket)
{
	unsigned int TotalSize = c_Size;
	if( m_ItemID > -1 ) TotalSize += 1 + 2;
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte		( (char)m_PacketID,							Message, i );
	AppendByte		( m_WindowID,							Message, i );
	AppendShort		( m_SlotNum,							Message, i );
	AppendShort		( m_ItemID,								Message, i );
	if( m_ItemID > -1 )
	{
		AppendByte	( m_ItemCount,								Message, i );
		AppendShort	( m_ItemUses,								Message, i );
	}

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}