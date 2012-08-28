
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_Chat.h"





int cPacket_Chat::Parse(cByteBuffer & a_Buffer)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadBEUTF16String16, m_Message, TotalBytes);
	return TotalBytes;
}




