
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_ItemSwitch.h"





int cPacket_ItemSwitch::Parse(const char * a_Data, int a_Size)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadShort, m_SlotNum, TotalBytes);
	return TotalBytes;
}





void cPacket_ItemSwitch::Serialize(AString & a_Data) const
{
	AppendByte (a_Data, m_PacketID);
	AppendShort(a_Data, m_SlotNum);
}




