
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_TeleportEntity.h"

#include "../cEntity.h"





cPacket_TeleportEntity::cPacket_TeleportEntity(const cEntity & a_Entity)
{
	m_PacketID = E_ENT_TELEPORT;

	m_UniqueID = a_Entity.GetUniqueID();
	m_PosX = (int)(a_Entity.GetPosX() * 32);
	m_PosY = (int)(a_Entity.GetPosY() * 32);
	m_PosZ = (int)(a_Entity.GetPosZ() * 32);
	m_Rotation = (char)((a_Entity.GetRotation() / 360.f) * 256);
	m_Pitch    = (char)((a_Entity.GetPitch() / 360.f) * 256);
}





void cPacket_TeleportEntity::Serialize(AString & a_Data) const
{
	AppendByte   (a_Data, m_PacketID);
	AppendInteger(a_Data, m_UniqueID);
	AppendInteger(a_Data, m_PosX);
	AppendInteger(a_Data, m_PosY);
	AppendInteger(a_Data, m_PosZ);
	AppendByte   (a_Data, m_Rotation);
	AppendByte   (a_Data, m_Pitch);
}




