
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_BlockDig.h"





int cPacket_BlockDig::Parse(cByteBuffer & a_Buffer)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadChar,	m_Status,    TotalBytes);
	HANDLE_PACKET_READ(ReadBEInt, m_PosX,      TotalBytes);
	HANDLE_PACKET_READ(ReadChar,	m_PosY,      TotalBytes);
	HANDLE_PACKET_READ(ReadBEInt, m_PosZ,      TotalBytes);
	HANDLE_PACKET_READ(ReadChar,	m_Direction, TotalBytes);
	return TotalBytes;
}




