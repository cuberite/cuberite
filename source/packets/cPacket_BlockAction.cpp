
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_BlockAction.h"





cPacket_BlockAction::cPacket_BlockAction( const cPacket_BlockAction & a_Copy )
{
	m_PacketID = E_BLOCK_ACTION;
	m_PosX     = a_Copy.m_PosX;
	m_PosY     = a_Copy.m_PosY;
	m_PosZ     = a_Copy.m_PosZ;
	m_Byte1    = a_Copy.m_Byte1;
	m_Byte2    = a_Copy.m_Byte2;
}





void cPacket_BlockAction::Serialize(AString & a_Data) const
{
	AppendByte   (a_Data, m_PacketID);
	AppendInteger(a_Data, m_PosX);
	AppendShort  (a_Data, m_PosY);
	AppendInteger(a_Data, m_PosZ);
	AppendByte   (a_Data, m_Byte1);
	AppendByte   (a_Data, m_Byte2);
}




