
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_DestroyEntity.h"
#include "../cEntity.h"





cPacket_DestroyEntity::cPacket_DestroyEntity(cEntity* a_Entity)
{
	m_PacketID = E_DESTROY_ENT;

	m_UniqueID = a_Entity->GetUniqueID();
}





void cPacket_DestroyEntity::Serialize(AString & a_Data) const
{
	AppendByte	 (a_Data, m_PacketID);
	AppendInteger(a_Data, m_UniqueID);
}




