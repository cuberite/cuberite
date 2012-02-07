
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_AddToInventory.h"
#include "cPacket_WholeInventory.h"
#include "cPacket_ItemData.h"





void cPacket_AddToInventory::Serialize(AString & a_Data) const
{
	AppendByte (a_Data, m_PacketID);
	cPacket_ItemData::AppendItem(a_Data, m_ItemType, m_Count, m_Life);
}




