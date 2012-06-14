
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_Flying.h"





int cPacket_Flying::Parse(const char * a_Data, int a_Size)
{
	int TotalBytes= 0;
	HANDLE_PACKET_READ(ReadBool, m_bFlying, TotalBytes);
	return TotalBytes;
}




