
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MobSpawnerEntity.h"
#include "../World.h"
#include "json/json.h"





cMobSpawnerEntity::cMobSpawnerEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World)
	: super(E_BLOCK_MOB_SPAWNER, a_BlockX, a_BlockY, a_BlockZ, a_World)
	, m_EntityName("Pig")
	, m_SpawnDelay(20)
	, m_MinSpawnDelay(200)
	, m_MaxSpawnDelay(800)
	, m_MaxNearbyEntities(6)
	, m_ActivatingRange(16)
	, m_SpawnRange(4)
{
}





cMobSpawnerEntity::~cMobSpawnerEntity()
{
	
}





bool cMobSpawnerEntity::Tick(float a_Dt, cChunk & a_Chunk)
{
	
}





void cMobSpawnerEntity::UsedBy(cPlayer * a_Player)
{
	if (IsPlayingRecord())
	{
		EjectRecord();
	}
	else
	{
		const cItem & HeldItem = a_Player->GetEquippedItem();
		if (PlayRecord(HeldItem.m_ItemType))
		{
			a_Player->GetInventory().RemoveOneEquippedItem();
		}
	}
}





bool cMobSpawnerEntity::LoadFromJson(const Json::Value & a_Value)
{
	m_PosX = a_Value.get("x", 0).asInt();
	m_PosY = a_Value.get("y", 0).asInt();
	m_PosZ = a_Value.get("z", 0).asInt();

	m_Record = a_Value.get("Record", 0).asInt();

	return true;
}





void cMobSpawnerEntity::SaveToJson(Json::Value & a_Value)
{
	a_Value["x"] = m_PosX;
	a_Value["y"] = m_PosY;
	a_Value["z"] = m_PosZ;

	a_Value["Record"] = m_Record;
}




