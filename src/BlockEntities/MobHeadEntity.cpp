
// MobHeadEntity.cpp

// Implements the cMobHeadEntity class representing a single skull / head in the world

#include "Globals.h"
#include "MobHeadEntity.h"
#include "json/json.h"
#include "../Entities/Player.h"
#include "../ClientHandle.h"





cMobHeadEntity::cMobHeadEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World):
	Super(a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ, a_World),
	m_Type(SKULL_TYPE_SKELETON),
	m_Rotation(SKULL_ROTATION_NORTH)
{
	ASSERT(a_BlockType == E_BLOCK_HEAD);
}





void cMobHeadEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = reinterpret_cast<const cMobHeadEntity &>(a_Src);
	m_OwnerName = src.m_OwnerName;
	m_OwnerTexture = src.m_OwnerTexture;
	m_OwnerTextureSignature = src.m_OwnerTextureSignature;
	m_OwnerUUID = src.m_OwnerUUID;
	m_Rotation = src.m_Rotation;
	m_Type = src.m_Type;
}




bool cMobHeadEntity::UsedBy(cPlayer * a_Player)
{
	UNUSED(a_Player);
	return true;
}





void cMobHeadEntity::SetType(const eMobHeadType & a_Type)
{
	if ((!m_OwnerName.empty()) && (a_Type != SKULL_TYPE_PLAYER))
	{
		m_OwnerName = m_OwnerTexture = m_OwnerTextureSignature = "";
		m_OwnerUUID = cUUID{};
	}
	m_Type = a_Type;
	m_World->BroadcastBlockEntity(m_PosX, m_PosY, m_PosZ);
}





void cMobHeadEntity::SetRotation(eMobHeadRotation a_Rotation)
{
	m_Rotation = a_Rotation;
	m_World->BroadcastBlockEntity(m_PosX, m_PosY, m_PosZ);
}





void cMobHeadEntity::SetOwner(const cPlayer & a_Owner)
{
	if (m_Type != SKULL_TYPE_PLAYER)
	{
		return;
	}

	m_OwnerName = a_Owner.GetName();
	m_OwnerUUID = a_Owner.GetUUID();

	const Json::Value & Properties = a_Owner.GetClientHandle()->GetProperties();
	for (auto & Node : Properties)
	{
		if (Node.get("name", "").asString() == "textures")
		{
			m_OwnerTexture = Node.get("value", "").asString();
			m_OwnerTextureSignature = Node.get("signature", "").asString();
			break;
		}
	}

	m_World->BroadcastBlockEntity(m_PosX, m_PosY, m_PosZ);
}





void cMobHeadEntity::SetOwner(const cUUID & a_OwnerUUID, const AString & a_OwnerName, const AString & a_OwnerTexture, const AString & a_OwnerTextureSignature)
{
	if (m_Type != SKULL_TYPE_PLAYER)
	{
		return;
	}

	m_OwnerUUID = a_OwnerUUID;
	m_OwnerName = a_OwnerName;
	m_OwnerTexture = a_OwnerTexture;
	m_OwnerTextureSignature = a_OwnerTextureSignature;
	m_World->BroadcastBlockEntity(m_PosX, m_PosY, m_PosZ);
}





void cMobHeadEntity::SendTo(cClientHandle & a_Client)
{
	cWorld * World = a_Client.GetPlayer()->GetWorld();
	a_Client.SendBlockChange(m_PosX, m_PosY, m_PosZ, m_BlockType, World->GetBlockMeta(m_PosX, m_PosY, m_PosZ));
	a_Client.SendUpdateBlockEntity(*this);
}




