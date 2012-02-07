
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_ArmAnim.h"





int cPacket_ArmAnim::Parse(const char * a_Data, int a_Size)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadInteger, m_EntityID,  TotalBytes);
	HANDLE_PACKET_READ(ReadByte   , m_Animation, TotalBytes);
	return TotalBytes;
}





void cPacket_ArmAnim::Serialize(AString & a_Data) const
{
	AppendByte   (a_Data, m_PacketID);
	AppendInteger(a_Data, m_EntityID);
	AppendByte   (a_Data, m_Animation);
}




