
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_RelativeEntityMove.h"





void cPacket_RelativeEntityMove::Serialize(AString & a_Data) const
{
	AppendByte	 (a_Data, m_PacketID);
	AppendInteger(a_Data, m_UniqueID);
	AppendByte	 (a_Data, m_MoveX);
	AppendByte	 (a_Data, m_MoveY);
	AppendByte	 (a_Data, m_MoveZ);
}




