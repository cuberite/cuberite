
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_ArmAnim.h"





int cPacket_ArmAnim::Parse(cByteBuffer & a_Buffer)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadBEInt, m_EntityID,  TotalBytes);
	HANDLE_PACKET_READ(ReadChar,  m_Animation, TotalBytes);
	return TotalBytes;
}




