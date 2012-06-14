
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_13.h"





int cPacket_13::Parse(const char * a_Data, int a_Size)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadInteger, m_EntityID, TotalBytes);
	HANDLE_PACKET_READ(ReadByte   , m_ActionID, TotalBytes);
	return TotalBytes;
}




