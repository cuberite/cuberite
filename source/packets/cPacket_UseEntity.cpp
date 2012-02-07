
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_UseEntity.h"





int cPacket_UseEntity::Parse(const char * a_Data, int a_Size)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadInteger, m_UniqueID,   TotalBytes);
	HANDLE_PACKET_READ(ReadInteger, m_TargetID,   TotalBytes);
	HANDLE_PACKET_READ(ReadBool,    m_bLeftClick, TotalBytes);
	return TotalBytes;
}




