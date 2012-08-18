
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_BlockPlace.h"
#include "cPacket_ItemData.h"





int cPacket_BlockPlace::Parse(cByteBuffer & a_Buffer)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadBEInt, m_PosX,      TotalBytes);
	HANDLE_PACKET_READ(ReadByte,  m_PosY,      TotalBytes);
	HANDLE_PACKET_READ(ReadBEInt, m_PosZ,      TotalBytes);
	HANDLE_PACKET_READ(ReadChar,  m_Direction, TotalBytes);

	cPacket_ItemData Item(m_HeldItem);
	int res = Item.Parse(a_Buffer);
	if (res < 0)
	{
		return res;
	}
	TotalBytes += res;
	return TotalBytes;
}




