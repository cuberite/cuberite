#include "cPacket_AddToInventory.h"
#include "cPacket_WholeInventory.h"
#include "../cItem.h"
#include "cPacket_ItemData.h"

bool cPacket_AddToInventory::Send( cSocket & a_Socket )
{
	unsigned int TotalSize = c_Size;

	cPacket_ItemData Item;

	TotalSize += Item.GetSize((short) m_ItemType);

	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte	 ( (char) m_PacketID,	Message, i );
	
	Item.AppendItem(Message, i, (short) m_ItemType, m_Count, this->m_Life);

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}