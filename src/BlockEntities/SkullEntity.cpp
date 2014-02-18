
// SkullEntity.cpp

// Implements the cSkullEntity class representing a single skull/head in the world

#include "Globals.h"
#include "json/json.h"
#include "SkullEntity.h"
#include "../Entities/Player.h"





cSkullEntity::cSkullEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World) :
	super(E_BLOCK_HEAD, a_BlockX, a_BlockY, a_BlockZ, a_World),
	m_Owner("")
{
}





void cSkullEntity::UsedBy(cPlayer * a_Player)
{
	UNUSED(a_Player);
}





void cSkullEntity::SetSkullType(const eSkullType & a_SkullType)
{
	if ((!m_Owner.empty()) && (a_SkullType != SKULL_TYPE_PLAYER))
	{
		m_Owner = "";
	}
	m_SkullType = a_SkullType;
}





void cSkullEntity::SetRotation(eSkullRotation a_Rotation)
{
	m_Rotation = a_Rotation;
}





void cSkullEntity::SetOwner(const AString & a_Owner)
{
	if ((a_Owner.length() > 16) || (m_SkullType != SKULL_TYPE_PLAYER))
	{
		return;
	}
	m_Owner = a_Owner;
}





void cSkullEntity::SendTo(cClientHandle & a_Client)
{
	a_Client.SendUpdateBlockEntity(*this);
}





bool cSkullEntity::LoadFromJson(const Json::Value & a_Value)
{
	m_PosX = a_Value.get("x", 0).asInt();
	m_PosY = a_Value.get("y", 0).asInt();
	m_PosZ = a_Value.get("z", 0).asInt();

	m_SkullType = static_cast<eSkullType>(a_Value.get("SkullType", 0).asInt());
	m_Rotation  = static_cast<eSkullRotation>(a_Value.get("Rotation", 0).asInt());
	m_Owner     = a_Value.get("Owner", "").asString();

	return true;
}





void cSkullEntity::SaveToJson(Json::Value & a_Value)
{
	a_Value["x"] = m_PosX;
	a_Value["y"] = m_PosY;
	a_Value["z"] = m_PosZ;

	a_Value["SkullType"] = m_SkullType;
	a_Value["Rotation"]  = m_Rotation;
	a_Value["Owner"]     = m_Owner;
}




