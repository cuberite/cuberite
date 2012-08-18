
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_CreativeInventoryAction.h"
#include "cPacket_ItemData.h"





cPacket_CreativeInventoryAction::cPacket_CreativeInventoryAction( const cPacket_CreativeInventoryAction & a_Copy )
{
	m_PacketID    = E_CREATIVE_INVENTORY_ACTION;
	m_SlotNum     = a_Copy.m_SlotNum;
	m_ClickedItem = a_Copy.m_ClickedItem;
}





int cPacket_CreativeInventoryAction::Parse(cByteBuffer & a_Buffer)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadBEShort, m_SlotNum, TotalBytes);

	cPacket_ItemData Item(m_ClickedItem);
	int res = Item.Parse(a_Buffer);
	if (res < 0)
	{
		return res;
	}
	TotalBytes += res;
	return TotalBytes;
}





void cPacket_CreativeInventoryAction::Serialize(AString & a_Data) const
{
	AppendByte (a_Data, m_PacketID);
	AppendShort(a_Data, m_SlotNum);

	cPacket_ItemData::AppendItem(a_Data, m_ClickedItem);
}




