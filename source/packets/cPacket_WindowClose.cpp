
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_WindowClose.h"





int cPacket_WindowClose::Parse(const char * a_Data, int a_Size)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadByte, m_Close, TotalBytes);
	return TotalBytes;
}





void cPacket_WindowClose::Serialize(AString & a_Data) const
{
	AppendByte(a_Data, m_PacketID);
	AppendByte(a_Data, m_Close);
}




