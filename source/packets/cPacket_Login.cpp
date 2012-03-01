
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_Login.h"





const std::string cPacket_Login::LEVEL_TYPE_DEFAULT = "DEFAULT";
const std::string cPacket_Login::LEVEL_TYPE_SUPERFLAT = "SUPERFLAT";





int cPacket_Login::Parse(const char * a_Data, int a_Size)
{
	//printf("Parse: NEW Login\n");
	int TotalBytes = 0;
	m_Username.clear();
	HANDLE_PACKET_READ(ReadInteger,  m_ProtocolVersion, TotalBytes);
	HANDLE_PACKET_READ(ReadString16, m_Username,        TotalBytes);
#if (MINECRAFT_1_2_2 != 1 )
	HANDLE_PACKET_READ(ReadLong,     m_MapSeed,         TotalBytes);
#endif
	HANDLE_PACKET_READ(ReadString16, m_LevelType,       TotalBytes);
	HANDLE_PACKET_READ(ReadInteger,  m_ServerMode,      TotalBytes);
#if (MINECRAFT_1_2_2 == 1 )
	HANDLE_PACKET_READ(ReadInteger,  m_Dimension,       TotalBytes);
#else
	HANDLE_PACKET_READ(ReadByte,     m_Dimension,       TotalBytes);
#endif
	HANDLE_PACKET_READ(ReadByte,     m_Difficulty,      TotalBytes);
	HANDLE_PACKET_READ(ReadByte,     m_WorldHeight,     TotalBytes);
	HANDLE_PACKET_READ(ReadByte,     m_MaxPlayers,      TotalBytes);
	return TotalBytes;
}





void cPacket_Login::Serialize(AString & a_Data) const
{
	AppendByte    (a_Data, m_PacketID);
	AppendInteger (a_Data, m_ProtocolVersion);
	AppendString16(a_Data, m_Username);
#if (MINECRAFT_1_2_2 != 1 )
	AppendLong    (a_Data, m_MapSeed);
#endif
	AppendString16(a_Data, m_LevelType);
	AppendInteger (a_Data, m_ServerMode);
#if (MINECRAFT_1_2_2 == 1 )
	AppendInteger (a_Data, m_Dimension);
#else
	AppendByte    (a_Data, m_Dimension);
#endif
	AppendByte    (a_Data, m_Difficulty);
	AppendByte    (a_Data, m_WorldHeight);
	AppendByte    (a_Data, m_MaxPlayers);
}




