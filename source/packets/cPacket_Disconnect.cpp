
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_Disconnect.h"





int cPacket_Disconnect::Parse(cByteBuffer & a_Buffer)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadBEUTF16String16, m_Reason, TotalBytes);
	return TotalBytes;
}





