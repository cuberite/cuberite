
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





int cPacket_CreativeInventoryAction::Parse(const char * a_Data, int a_Size)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadShort, m_Slot, TotalBytes);

	cPacket_ItemData Item;
	int res = Item.Parse(a_Data + TotalBytes, a_Size - TotalBytes);
	if (res < 0)
	{
		return res;
	}
	TotalBytes += res;

	m_ItemID = Item.m_ItemID;
	m_Quantity = Item.m_ItemCount;
	m_Damage = Item.m_ItemUses;

	return TotalBytes;
}





void cPacket_CreativeInventoryAction::Serialize(AString & a_Data) const
{
	short ItemID = m_ItemID;
	ASSERT(ItemID >= -1);  // Check validity of packets in debug runtime
	if (ItemID <= 0)
	{
		ItemID = -1;
		// Fix, to make sure no invalid values are sent.
		// WARNING: HERE ITS -1, BUT IN NAMED ENTITY SPAWN PACKET ITS 0 !!
	}

	AppendByte	 (a_Data, m_PacketID);
	AppendShort  (a_Data, m_Slot);

	cPacket_ItemData::AppendItem(a_Data, ItemID, m_Quantity, m_Damage);
}




