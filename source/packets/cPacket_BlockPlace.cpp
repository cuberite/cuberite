
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_BlockPlace.h"
#include "cPacket_ItemData.h"





int cPacket_BlockPlace::Parse(const char * a_Data, int a_Size)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadInteger, m_PosX,      TotalBytes);
	HANDLE_PACKET_READ(ReadByte,    m_PosY,      TotalBytes);
	HANDLE_PACKET_READ(ReadInteger, m_PosZ,      TotalBytes);
	HANDLE_PACKET_READ(ReadByte,    m_Direction, TotalBytes);

	cPacket_ItemData Item;
	int res = Item.Parse(a_Data + TotalBytes, a_Size - TotalBytes);
	if (res < 0)
	{
		return res;
	}
	TotalBytes += res;

	m_ItemType = Item.m_ItemID;
	m_Count    = Item.m_ItemCount;
	m_Uses     = Item.m_ItemUses;

	return TotalBytes;
}




