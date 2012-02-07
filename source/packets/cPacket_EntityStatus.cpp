
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_EntityStatus.h"





void cPacket_EntityStatus::Serialize(AString & a_Data) const
{
	AppendByte   (a_Data, m_PacketID);
	AppendInteger(a_Data, m_UniqueID);
	AppendByte   (a_Data, m_Status);
}




