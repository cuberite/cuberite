#include "cPacket_InventorySlot.h"
#include "cPacket_WholeInventory.h"
#include "cPacket_ItemData.h"

bool cPacket_InventorySlot::Send(cSocket & a_Socket)
{
	unsigned int TotalSize = c_Size;

	cPacket_ItemData Item;

	TotalSize += Item.GetSize(m_ItemID);

	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte		( (char)m_PacketID,						Message, i );
	AppendByte		( m_WindowID,							Message, i );
	AppendShort		( m_SlotNum,							Message, i );


	Item.AppendItem(Message, i, m_ItemID, m_ItemCount, m_ItemUses);


	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}