
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_WindowClose.h"





int cPacket_WindowClose::Parse(cByteBuffer & a_Buffer)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadChar, m_WindowID, TotalBytes);
	return TotalBytes;
}





void cPacket_WindowClose::Serialize(AString & a_Data) const
{
	AppendByte(a_Data, m_PacketID);
	AppendByte(a_Data, m_WindowID);
}




