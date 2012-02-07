
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_Respawn.h"





void cPacket_Respawn::Serialize(AString & a_Data) const
{
	AppendByte    (a_Data, m_PacketID);
	AppendByte    (a_Data, m_World);
	AppendByte    (a_Data, m_Difficulty);
	AppendByte    (a_Data, m_CreativeMode);
	AppendShort   (a_Data, m_WorldHeight);
	AppendLong    (a_Data, m_MapSeed);
	AppendString16(a_Data, m_LevelType);
}





int cPacket_Respawn::Parse(const char * a_Data, int a_Size)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadByte,     m_World, TotalBytes);
	HANDLE_PACKET_READ(ReadByte,     m_Difficulty, TotalBytes);
	HANDLE_PACKET_READ(ReadByte,     m_CreativeMode, TotalBytes);
	HANDLE_PACKET_READ(ReadShort,    m_WorldHeight, TotalBytes);
	HANDLE_PACKET_READ(ReadLong,     m_MapSeed, TotalBytes);
	HANDLE_PACKET_READ(ReadString16, m_LevelType, TotalBytes);
	return TotalBytes;
}




