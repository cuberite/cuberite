
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_UseEntity.h"





int cPacket_UseEntity::Parse(cByteBuffer & a_Buffer)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadBEInt, m_UniqueID,   TotalBytes);
	HANDLE_PACKET_READ(ReadBEInt, m_TargetID,   TotalBytes);
	HANDLE_PACKET_READ(ReadBool,  m_bLeftClick, TotalBytes);
	return TotalBytes;
}




