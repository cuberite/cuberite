
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_KeepAlive.h"





void cPacket_KeepAlive::Serialize(AString & a_Data) const
{
	AppendByte   (a_Data, m_PacketID);
	AppendInteger(a_Data, m_KeepAliveID);
}





int cPacket_KeepAlive::Parse(const char * a_Data, int a_Size)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadInteger, m_KeepAliveID, TotalBytes);
	return TotalBytes;
}




