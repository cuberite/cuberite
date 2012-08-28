
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_Respawn.h"





int cPacket_Respawn::Parse(cByteBuffer & a_Buffer)
{
	int TotalBytes = 0;

	HANDLE_PACKET_READ(ReadBEInt,           m_Dimension,    TotalBytes);
	HANDLE_PACKET_READ(ReadChar,            m_Difficulty,   TotalBytes);
	HANDLE_PACKET_READ(ReadChar,            m_CreativeMode, TotalBytes);
	HANDLE_PACKET_READ(ReadBEShort,         m_WorldHeight,  TotalBytes);
	HANDLE_PACKET_READ(ReadBEUTF16String16, m_LevelType,    TotalBytes);
	return TotalBytes;
}




