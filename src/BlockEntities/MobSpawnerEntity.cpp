
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MobSpawnerEntity.h"

#include "../World.h"
#include "../FastRandom.h"
#include "../Items/ItemSpawnEgg.h"





cMobSpawnerEntity::cMobSpawnerEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World)
	: super(E_BLOCK_MOB_SPAWNER, a_BlockX, a_BlockY, a_BlockZ, a_World)
	, m_Entity(mtPig)
	, m_SpawnDelay(100)
	, m_IsActive(false)
{
}





void cMobSpawnerEntity::SendTo(cClientHandle & a_Client)
{
	a_Client.SendUpdateBlockEntity(*this);
}





void cMobSpawnerEntity::UsedBy(cPlayer * a_Player)
{
	if (a_Player->GetEquippedItem().m_ItemType == E_ITEM_SPAWN_EGG)
	{
		eMonsterType MonsterType = cItemSpawnEggHandler::ItemDamageToMonsterType(a_Player->GetEquippedItem().m_ItemDamage);
		if (MonsterType == eMonsterType::mtInvalidType)
		{
			return;
		}

		m_Entity = MonsterType;
		ResetTimer();
		if (!a_Player->IsGameModeCreative())
		{
			a_Player->GetInventory().RemoveOneEquippedItem();
		}
		LOGD("Changed monster spawner at {%d, %d, %d} to type %s.", GetPosX(), GetPosY(), GetPosZ(), cMonster::MobTypeToString(MonsterType).c_str());
	}
}





void cMobSpawnerEntity::UpdateActiveState(void)
{
	if (GetNearbyPlayersNum() > 0)
	{
		m_IsActive = true;
	}
	else
	{
		m_IsActive = false;
	}
}





bool cMobSpawnerEntity::Tick(float a_Dt, cChunk & a_Chunk)
{
	// Update the active flag every 5 seconds
	if ((m_World->GetWorldAge() % 100) == 0)
	{
		UpdateActiveState();
	}

	if (!m_IsActive)
	{
		return false;
	}

	if (m_SpawnDelay <= 0)
	{
		SpawnEntity();
		return true;
	}
	else
	{
		m_SpawnDelay--;
	}
	return false;
}





void cMobSpawnerEntity::ResetTimer(void)
{
	m_SpawnDelay = static_cast<short>(200 + m_World->GetTickRandomNumber(600));
	m_World->BroadcastBlockEntity(m_PosX, m_PosY, m_PosZ);
}





void cMobSpawnerEntity::SpawnEntity(void)
{
	int NearbyEntities = GetNearbyMonsterNum(m_Entity);
	if (NearbyEntities >= 6)
	{
		ResetTimer();
		return;
	}

	cFastRandom Random;

	for (size_t i = 0; i < 4; i++)
	{
		if (NearbyEntities >= 6)
		{
			break;
		}

		int PosX = (int) (m_PosX + (double)(Random.NextFloat() - Random.NextFloat()) * 4.0);
		int PosY = m_PosY + Random.NextInt(3) - 1;
		int PosZ = (int) (m_PosZ + (double)(Random.NextFloat() - Random.NextFloat()) * 4.0);

		if (m_World->GetCreatureSpawner().CanSpawnHere(PosX, PosY, PosZ, m_Entity))
		{
			cMonster * Monster = cMonster::NewMonsterFromType(m_Entity);
			if (Monster == NULL)
			{
				continue;
			}

			Monster->SetPosition(PosX, PosY, PosZ);
			Monster->SetYaw(Random.NextFloat() * 360.0f);
			if (m_World->SpawnMobFinalize(Monster) != mtInvalidType)
			{
				m_World->BroadcastSoundParticleEffect(2004, (int)(PosX * 8.0), (int)(PosY * 8.0), (int)(PosZ * 8.0), 0);
				NearbyEntities++;
				ResetTimer();
			}
		}
	}
}





int cMobSpawnerEntity::GetNearbyPlayersNum(void)
{
	Vector3d SpawnerPos(m_PosX + 0.5, m_PosY + 0.5, m_PosZ + 0.5);
	int NumPlayers = 0;

	class cCallback : public cChunkDataCallback
	{
	public:
		cCallback(Vector3d a_SpawnerPos, int & a_NumPlayers) :
			m_SpawnerPos(a_SpawnerPos),
			m_NumPlayers(a_NumPlayers)
		{
		}

		virtual void Entity(cEntity * a_Entity) override
		{
			if (!a_Entity->IsPlayer())
			{
				return;
			}

			if ((m_SpawnerPos - a_Entity->GetPosition()).Length() <= 16)
			{
				m_NumPlayers++;
			}
		}

	protected:
		Vector3d m_SpawnerPos;
		int & m_NumPlayers;
	} Callback(SpawnerPos, NumPlayers);

	int ChunkX = GetChunkX();
	int ChunkZ = GetChunkZ();
	m_World->ForEachChunkInRect(ChunkX - 1, ChunkX + 1, ChunkZ - 1, ChunkZ + 1, Callback);

	return NumPlayers;
}





int cMobSpawnerEntity::GetNearbyMonsterNum(eMonsterType a_EntityType)
{
	Vector3d SpawnerPos(m_PosX + 0.5, m_PosY + 0.5, m_PosZ + 0.5);
	int NumEntities = 0;

	class cCallback : public cChunkDataCallback
	{
	public:
		cCallback(Vector3d a_SpawnerPos, eMonsterType a_EntityType, int & a_NumEntities) :
			m_SpawnerPos(a_SpawnerPos),
			m_EntityType(a_EntityType),
			m_NumEntities(a_NumEntities)
		{
		}

		virtual void Entity(cEntity * a_Entity) override
		{
			if (!a_Entity->IsMob())
			{
				return;
			}

			cMonster * Mob = (cMonster *)a_Entity;
			if (Mob->GetMobType() != m_EntityType)
			{
				return;
			}

			if ((Diff(m_SpawnerPos.x, a_Entity->GetPosX()) <= 8.0) && (Diff(m_SpawnerPos.y, a_Entity->GetPosY()) <= 4.0) && (Diff(m_SpawnerPos.z, a_Entity->GetPosZ()) <= 8.0))
			{
				m_NumEntities++;
			}
		}

	protected:
		Vector3d m_SpawnerPos;
		eMonsterType m_EntityType;
		int & m_NumEntities;
	} Callback(SpawnerPos, a_EntityType, NumEntities);

	int ChunkX = GetChunkX();
	int ChunkZ = GetChunkZ();
	m_World->ForEachChunkInRect(ChunkX - 1, ChunkX + 1, ChunkZ - 1, ChunkZ + 1, Callback);

	return NumEntities;
}




