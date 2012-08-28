
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_KeepAlive.h"





int cPacket_KeepAlive::Parse(cByteBuffer & a_Buffer)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadBEInt, m_KeepAliveID, TotalBytes);
	return TotalBytes;
}




