
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_InventorySlot.h"
#include "cPacket_WholeInventory.h"
#include "cPacket_ItemData.h"





void cPacket_InventorySlot::Serialize(AString & a_Data) const
{
	AppendByte (a_Data, m_PacketID);
	AppendByte (a_Data, m_WindowID);
	AppendShort(a_Data, m_SlotNum);

	cPacket_ItemData::AppendItem(a_Data, m_ItemID, m_ItemCount, m_ItemUses);
}




