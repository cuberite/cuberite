
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_WindowClick.h"
#include "cPacket_WholeInventory.h"
#include "cPacket_ItemData.h"





int cPacket_WindowClick::Parse(cByteBuffer & a_Buffer)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadChar,    m_WindowID,       TotalBytes);
	HANDLE_PACKET_READ(ReadBEShort, m_SlotNum,        TotalBytes);
	HANDLE_PACKET_READ(ReadChar,    m_RightMouse,     TotalBytes);
	HANDLE_PACKET_READ(ReadBEShort, m_NumClicks,      TotalBytes);
	HANDLE_PACKET_READ(ReadBool,    m_IsShiftPressed, TotalBytes);

	cPacket_ItemData Item;
	int res = Item.Parse(a_Buffer);
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




