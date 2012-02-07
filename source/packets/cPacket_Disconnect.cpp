
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_Disconnect.h"





int cPacket_Disconnect::Parse(const char * a_Data, int a_Size)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadString16, m_Reason, TotalBytes);
	return TotalBytes;
}





void cPacket_Disconnect::Serialize(AString & a_Data) const
{
	AppendByte    (a_Data, m_PacketID);
	AppendString16(a_Data, m_Reason);
}




