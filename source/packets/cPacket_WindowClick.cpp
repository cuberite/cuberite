
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_WindowClick.h"
#include "cPacket_WholeInventory.h"
#include "cPacket_ItemData.h"





int cPacket_WindowClick::Parse(const char * a_Data, int a_Size)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadByte,  m_WindowID, TotalBytes);
	HANDLE_PACKET_READ(ReadShort, m_SlotNum, TotalBytes);
	HANDLE_PACKET_READ(ReadByte,  m_RightMouse, TotalBytes);
	HANDLE_PACKET_READ(ReadShort, m_NumClicks, TotalBytes);
	HANDLE_PACKET_READ(ReadBool,  m_Bool, TotalBytes);

	// LOG("WindowClick: WindowID: %i; FromSlot: %i; Right/Le: %i; NumClick: %i", m_Type, m_SlotNum, m_RightMouse, m_NumClicks );

	cPacket_ItemData Item;

	int res = Item.Parse(a_Data + TotalBytes, a_Size - TotalBytes);
	if (res < 0)
	{
		return res;
	}
	TotalBytes += res;

	m_ItemID    = Item.m_ItemID;
	m_ItemCount = Item.m_ItemCount;
	m_ItemUses  = Item.m_ItemUses;

	m_EnchantNums = Item.m_EnchantNums;

	return TotalBytes;
}




