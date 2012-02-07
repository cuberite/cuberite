
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_Thunderbolt.h"





cPacket_Thunderbolt::cPacket_Thunderbolt( const cPacket_Thunderbolt & a_Copy )
{
	m_PacketID = E_THUNDERBOLT;
	m_UniqueID = 237183; //just a random Ent ID. I don't think this matters at all.
	m_Unknown  = true;
	m_xLBPos   = a_Copy.m_xLBPos;
	m_yLBPos   = a_Copy.m_yLBPos;
	m_zLBPos   = a_Copy.m_zLBPos;
}





void cPacket_Thunderbolt::Serialize(AString & a_Data) const
{
	AppendByte    (a_Data, m_PacketID);
	AppendInteger (a_Data, m_UniqueID);
	AppendBool    (a_Data, m_Unknown);
	AppendInteger (a_Data, m_xLBPos * 32);
	AppendInteger (a_Data, m_yLBPos * 32);
	AppendInteger (a_Data, m_zLBPos * 32);
}




