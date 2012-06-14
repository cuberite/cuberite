
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_BlockChange.h"





void cPacket_BlockChange::Serialize(AString & a_Data) const
{
	AppendByte	 (a_Data, m_PacketID);
	AppendInteger(a_Data, m_PosX);
	AppendByte	 (a_Data, m_PosY);
	AppendInteger(a_Data, m_PosZ);
	AppendByte   (a_Data, m_BlockType);
	AppendByte   (a_Data, m_BlockMeta);
}




