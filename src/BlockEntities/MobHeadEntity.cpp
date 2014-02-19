
// MobHeadEntity.cpp

// Implements the cMobHeadEntity class representing a single skull/head in the world

#include "Globals.h"
#include "json/json.h"
#include "MobHeadEntity.h"
#include "../Entities/Player.h"





cMobHeadEntity::cMobHeadEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World) :
	super(E_BLOCK_HEAD, a_BlockX, a_BlockY, a_BlockZ, a_World),
	m_Owner("")
{
}





void cMobHeadEntity::UsedBy(cPlayer * a_Player)
{
	UNUSED(a_Player);
}





void cMobHeadEntity::SetType(const eMobHeadType & a_Type)
{
	if ((!m_Owner.empty()) && (a_Type != SKULL_TYPE_PLAYER))
	{
		m_Owner = "";
	}
	m_Type = a_Type;
}





void cMobHeadEntity::SetRotation(eMobHeadRotation a_Rotation)
{
	m_Rotation = a_Rotation;
}





void cMobHeadEntity::SetOwner(const AString & a_Owner)
{
	if ((a_Owner.length() > 16) || (m_Type != SKULL_TYPE_PLAYER))
	{
		return;
	}
	m_Owner = a_Owner;
}





void cMobHeadEntity::SendTo(cClientHandle & a_Client)
{
	a_Client.SendUpdateBlockEntity(*this);
}





bool cMobHeadEntity::LoadFromJson(const Json::Value & a_Value)
{
	m_PosX = a_Value.get("x", 0).asInt();
	m_PosY = a_Value.get("y", 0).asInt();
	m_PosZ = a_Value.get("z", 0).asInt();

	m_Type      = static_cast<eMobHeadType>(a_Value.get("Type", 0).asInt());
	m_Rotation  = static_cast<eMobHeadRotation>(a_Value.get("Rotation", 0).asInt());
	m_Owner     = a_Value.get("Owner", "").asString();

	return true;
}





void cMobHeadEntity::SaveToJson(Json::Value & a_Value)
{
	a_Value["x"] = m_PosX;
	a_Value["y"] = m_PosY;
	a_Value["z"] = m_PosZ;

	a_Value["Type"] = m_Type;
	a_Value["Rotation"]  = m_Rotation;
	a_Value["Owner"]     = m_Owner;
}




