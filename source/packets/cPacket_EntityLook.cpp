
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_EntityLook.h"

#include "../cEntity.h"





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cPacket_EntityLook:

cPacket_EntityLook::cPacket_EntityLook(const cEntity & a_Entity)
{
	m_PacketID = E_ENT_LOOK;

	m_UniqueID = a_Entity.GetUniqueID();
	m_Rotation = (char)((a_Entity.GetRotation() / 360.f) * 256);
	m_Pitch    = (char)((a_Entity.GetPitch()    / 360.f) * 256);
}





void cPacket_EntityLook::Serialize(AString & a_Data) const
{
	AppendByte   (a_Data, m_PacketID);
	AppendInteger(a_Data, m_UniqueID);
	AppendByte   (a_Data, m_Rotation);
	AppendByte   (a_Data, m_Pitch);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cPacket_EntityHeadLook:

cPacket_EntityHeadLook::cPacket_EntityHeadLook(const cEntity & a_Entity)
{
	m_PacketID = E_ENT_HEAD_LOOK;

	m_UniqueID = a_Entity.GetUniqueID();
	m_HeadYaw  = (char)((a_Entity.GetRotation() / 360.f) * 256);
}





void cPacket_EntityHeadLook::Serialize(AString & a_Data) const
{
	AppendByte   (a_Data, m_PacketID);
	AppendInteger(a_Data, m_UniqueID);
	AppendByte   (a_Data, m_HeadYaw);
}




