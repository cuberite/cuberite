
// MobHeadEntity.cpp

// Implements the cMobHeadEntity class representing a single skull / head in the world

#include "Globals.h"
#include "MobHeadEntity.h"
#include "../Entities/Player.h"





cMobHeadEntity::cMobHeadEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World) :
	super(E_BLOCK_HEAD, a_BlockX, a_BlockY, a_BlockZ, a_World),
	m_Type(SKULL_TYPE_SKELETON),
	m_Rotation(SKULL_ROTATION_NORTH),
	m_Owner("")
{
}





bool cMobHeadEntity::UsedBy(cPlayer * a_Player)
{
	UNUSED(a_Player);
	return true;
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
	cWorld * World = a_Client.GetPlayer()->GetWorld();
	a_Client.SendBlockChange(m_PosX, m_PosY, m_PosZ, m_BlockType, World->GetBlockMeta(m_PosX, m_PosY, m_PosZ));
	a_Client.SendUpdateBlockEntity(*this);
}




