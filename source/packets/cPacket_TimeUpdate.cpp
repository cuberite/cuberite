
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_TimeUpdate.h"





int cPacket_TimeUpdate::Parse(const char * a_Data, int a_Size)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadLong, m_Time, TotalBytes);
	return TotalBytes;
}





void cPacket_TimeUpdate::Serialize(AString & a_Data) const
{
	AppendByte(a_Data, m_PacketID);
	AppendLong(a_Data, m_Time);
}




