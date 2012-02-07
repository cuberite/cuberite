
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_RelativeEntityMoveLook.h"





void cPacket_RelativeEntityMoveLook::Serialize(AString & a_Data) const
{
	AppendByte	 (a_Data, m_PacketID);
	AppendInteger(a_Data, m_UniqueID);
	AppendByte	 (a_Data, m_MoveX);
	AppendByte	 (a_Data, m_MoveY);
	AppendByte	 (a_Data, m_MoveZ);
	AppendByte   (a_Data, m_Yaw);
	AppendByte   (a_Data, m_Pitch);
}





