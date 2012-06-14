
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_UpdateHealth.h"





void cPacket_UpdateHealth::Serialize(AString & a_Data) const
{
	AppendByte (a_Data, m_PacketID);
	AppendShort(a_Data, m_Health);
	AppendShort(a_Data, m_Food);
	AppendFloat(a_Data, m_Saturation);
}




