
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_WindowClick.h"
#include "cPacket_WholeInventory.h"
#include "cPacket_ItemData.h"





int cPacket_WindowClick::Parse(cByteBuffer & a_Buffer)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadChar,    m_WindowID,       TotalBytes);
	HANDLE_PACKET_READ(ReadBEShort, m_SlotNum,        TotalBytes);
	HANDLE_PACKET_READ(ReadBool,    m_IsRightClick,   TotalBytes);
	HANDLE_PACKET_READ(ReadBEShort, m_TransactionID,  TotalBytes);
	HANDLE_PACKET_READ(ReadBool,    m_IsShiftPressed, TotalBytes);

	cPacket_ItemData Item(m_HeldItem);
	int res = Item.Parse(a_Buffer);
	if (res < 0)
	{
		return res;
	}
	TotalBytes += res;

	return TotalBytes;
}




