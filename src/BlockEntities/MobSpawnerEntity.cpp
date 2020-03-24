
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MobSpawnerEntity.h"

#include "../World.h"
#include "../FastRandom.h"
#include "../MobSpawner.h"
#include "../ClientHandle.h"
#include "../Items/ItemSpawnEgg.h"





cMobSpawnerEntity::cMobSpawnerEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World):
	super(a_BlockType, a_BlockMeta, a_Pos, a_World),
	m_Entity(mtPig),
	m_SpawnDelay(100),
	m_IsActive(false)
{
	ASSERT(a_BlockType == E_BLOCK_MOB_SPAWNER);
}





void cMobSpawnerEntity::CopyFrom(const cBlockEntity & a_Src)
{
	super::CopyFrom(a_Src);
	auto & src = static_cast<const cMobSpawnerEntity &>(a_Src);
	m_Entity = src.m_Entity;
	m_IsActive = src.m_IsActive;
	m_SpawnDelay = src.m_SpawnDelay;
}





void cMobSpawnerEntity::SendTo(cClientHandle & a_Client)
{
	a_Client.SendUpdateBlockEntity(*this);
}





bool cMobSpawnerEntity::UsedBy(cPlayer * a_Player)
{
	if (a_Player->GetEquippedItem().m_ItemType == E_ITEM_SPAWN_EGG)
	{
		eMonsterType MonsterType = cItemSpawnEggHandler::ItemDamageToMonsterType(a_Player->GetEquippedItem().m_ItemDamage);
		if (MonsterType == eMonsterType::mtInvalidType)
		{
			return false;
		}

		m_Entity = MonsterType;
		ResetTimer();
		if (!a_Player->IsGameModeCreative())
		{
			a_Player->GetInventory().RemoveOneEquippedItem();
		}
		FLOGD("Changed monster spawner at {0} to type {1}.", GetPos(), cMonster::MobTypeToString(MonsterType));
		return true;
	}
	return false;
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





bool cMobSpawnerEntity::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
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
	m_SpawnDelay = GetRandomProvider().RandInt<short>(200, 800);
	m_World->BroadcastBlockEntity(GetPos());
}





void cMobSpawnerEntity::SpawnEntity(void)
{
	int NearbyEntities = GetNearbyMonsterNum(m_Entity);
	if (NearbyEntities >= 6)
	{
		ResetTimer();
		return;
	}

	auto MobType = m_Entity;
	bool EntitiesSpawned = m_World->DoWithChunk(GetChunkX(), GetChunkZ(), [&](cChunk & a_Chunk)
		{
			auto & Random = GetRandomProvider();

			bool HaveSpawnedEntity = false;
			for (size_t i = 0; i < 4; i++)
			{
				if (NearbyEntities >= 6)
				{
					break;
				}

				Vector3i spawnRelPos(GetRelPos());
				spawnRelPos += Vector3i(
					static_cast<int>((Random.RandReal<double>() - Random.RandReal<double>()) * 4.0),
					Random.RandInt(-1, 1),
					static_cast<int>((Random.RandReal<double>() - Random.RandReal<double>()) * 4.0)
				);

				auto chunk = a_Chunk.GetRelNeighborChunkAdjustCoords(spawnRelPos);
				if ((chunk == nullptr) || !chunk->IsValid())
				{
					continue;
				}
				EMCSBiome Biome = chunk->GetBiomeAt(spawnRelPos.x, spawnRelPos.z);

				if (cMobSpawner::CanSpawnHere(chunk, spawnRelPos, MobType, Biome))
				{
					auto absPos = chunk->RelativeToAbsolute(spawnRelPos);
					auto monster = cMonster::NewMonsterFromType(MobType);
					if (monster == nullptr)
					{
						continue;
					}
					monster->SetPosition(absPos);
					monster->SetYaw(Random.RandReal(360.0f));
					if (chunk->GetWorld()->SpawnMobFinalize(std::move(monster)) != cEntity::INVALID_ID)
					{
						HaveSpawnedEntity = true;
						m_World->BroadcastSoundParticleEffect(
							EffectID::PARTICLE_MOBSPAWN,
							absPos,
							0
						);
						NearbyEntities++;
					}
				}
			}
			return HaveSpawnedEntity;
		}
	);

	if (EntitiesSpawned)
	{
		ResetTimer();
	}
}





int cMobSpawnerEntity::GetNearbyPlayersNum(void)
{
	auto SpawnerPos = Vector3d(0.5, 0.5, 0.5) + m_Pos;
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
	auto SpawnerPos = Vector3d(0.5, 0.5, 0.5) + m_Pos;
	int NumEntities = 0;

	class cCallback : public cChunkDataCallback
	{
	public:
		cCallback(Vector3d a_SpawnerPos, eMonsterType a_CallbackEntityType, int & a_NumEntities) :
			m_SpawnerPos(a_SpawnerPos),
			m_EntityType(a_CallbackEntityType),
			m_NumEntities(a_NumEntities)
		{
		}

		virtual void Entity(cEntity * a_Entity) override
		{
			if (!a_Entity->IsMob())
			{
				return;
			}

			cMonster * Mob = static_cast<cMonster *>(a_Entity);
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




