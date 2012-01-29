
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_CreativeInventoryAction.h"
#include "cPacket_ItemData.h"





cPacket_CreativeInventoryAction::cPacket_CreativeInventoryAction( const cPacket_CreativeInventoryAction & a_Copy )
{
	m_PacketID = E_CREATIVE_INVENTORY_ACTION;
	m_Slot	 = a_Copy.m_Slot;
	m_ItemID = a_Copy.m_ItemID;
	m_Quantity = a_Copy.m_Quantity;
	m_Damage = a_Copy.m_Damage;
}

bool cPacket_CreativeInventoryAction::Parse(cSocket & a_Socket)
{
	m_Socket = a_Socket;
	if( !ReadShort	( m_Slot ) ) return false;

	cPacket_ItemData Item;

	Item.Parse(m_Socket);

	m_ItemID = Item.m_ItemID;
	m_Quantity = Item.m_ItemCount;
	m_Damage = Item.m_ItemUses;


	return true;
}

bool cPacket_CreativeInventoryAction::Send(cSocket & a_Socket)
{
	//LOG("InventoryChange:");
	unsigned int TotalSize = c_Size;

	cPacket_ItemData Item;

	TotalSize += Item.GetSize(m_ItemID);

	char* Message = new char[TotalSize];

	if( m_ItemID <= 0 ) m_ItemID = -1; // Fix, to make sure no invalid values are sent.
                        // WARNING: HERE ITS -1, BUT IN NAMED ENTITY SPAWN PACKET ITS 0 !!
	//LOG("cPacket_CreateInventoryAction: Sending Creative item ID: %i", m_ItemID );

	unsigned int i = 0;
	AppendByte	 ( (char)m_PacketID,	Message, i );
	AppendShort  ( m_Slot,		Message, i );

	Item.AppendItem(Message, i, m_ItemID, m_Quantity, m_Damage);

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}
