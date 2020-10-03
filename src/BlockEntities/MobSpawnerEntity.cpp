
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MobSpawnerEntity.h"

#include "../World.h"
#include "../FastRandom.h"
#include "../MobSpawner.h"
#include "../ClientHandle.h"
#include "../Items/ItemSpawnEgg.h"





cMobSpawnerEntity::cMobSpawnerEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World):
	Super(a_BlockType, a_BlockMeta, a_Pos, a_World),
	m_Entity(mtPig),
	m_SpawnDelay(100),
	m_IsActive(false),
	m_EntityBoundingBox({(float) a_Pos.x, (float) a_Pos.y - 4,                     (float) a_Pos.z}, m_SpawnRange, 8),
	m_PlayerBoundingBox({(float) a_Pos.x, (float) a_Pos.y - m_RequiredPlayerRange, (float) a_Pos.z}, m_RequiredPlayerRange, m_RequiredPlayerRange * 2)
{
	ASSERT(a_BlockType == E_BLOCK_MOB_SPAWNER);
}





void cMobSpawnerEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = static_cast<const cMobSpawnerEntity &>(a_Src);
	m_Entity = src.m_Entity;
	m_IsActive = src.m_IsActive;
	m_SpawnDelay = src.m_SpawnDelay;
	m_SpawnCount = src.m_SpawnCount;
	m_SpawnRange = src.m_SpawnRange;
	m_MinSpawnDelay = src.m_MinSpawnDelay;
	m_MaxSpawnDelay = src.m_MaxSpawnDelay;
	m_MaxNearbyEntities = src.m_MaxNearbyEntities;
	m_RequiredPlayerRange = src.m_RequiredPlayerRange;
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
	m_IsActive = (GetNearbyPlayersNum() > 0);
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
	m_SpawnDelay = GetRandomProvider().RandInt<short>(m_MinSpawnDelay, m_MaxSpawnDelay);
	m_World->BroadcastBlockEntity(GetPos());
}





void cMobSpawnerEntity::SpawnEntity(void)
{
	auto NearbyEntities = GetNearbyMonsterNum(m_Entity);
	if (NearbyEntities >= m_MaxNearbyEntities)
	{
		ResetTimer();
		return;
	}

	bool EntitiesSpawned = m_World->DoWithChunk(GetChunkX(), GetChunkZ(), [&](cChunk & a_Chunk)
		{
			auto & Random = GetRandomProvider();

			bool HaveSpawnedEntity = false;
			for (size_t i = 0; i < m_SpawnCount; i++)
			{
				if (NearbyEntities >= m_MaxNearbyEntities)
				{
					break;
				}

				auto SpawnRelPos(GetRelPos());
				SpawnRelPos += Vector3i(
					static_cast<int>((Random.RandReal<double>() - Random.RandReal<double>()) * (float) m_SpawnRange),
					Random.RandInt(-1, 1),
					static_cast<int>((Random.RandReal<double>() - Random.RandReal<double>()) * (float) m_SpawnRange)
				);

				auto Chunk = a_Chunk.GetRelNeighborChunkAdjustCoords(SpawnRelPos);
				if ((Chunk == nullptr) || !Chunk->IsValid())
				{
					continue;
				}
				EMCSBiome Biome = Chunk->GetBiomeAt(SpawnRelPos.x, SpawnRelPos.z);

				if (cMobSpawner::CanSpawnHere(Chunk, SpawnRelPos, m_Entity, Biome, true))
				{
					auto AbsPos = Chunk->RelativeToAbsolute(SpawnRelPos);
					auto Monster = cMonster::NewMonsterFromType(m_Entity);
					if (Monster == nullptr)
					{
						continue;
					}
					Monster->SetPosition(AbsPos);
					Monster->SetYaw(Random.RandReal(360.0f));
					if (Chunk->GetWorld()->SpawnMobFinalize(std::move(Monster)) != cEntity::INVALID_ID)
					{
						HaveSpawnedEntity = true;
						m_World->BroadcastSoundParticleEffect(EffectID::PARTICLE_MOBSPAWN, AbsPos, 0);
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
	int NumPlayers = 0;

	auto Callback = [&] (cEntity & a_Entity)
	{
		if (!a_Entity.IsPlayer())
		{
			return false;
		}
		if ((m_Pos - a_Entity.GetPosition()).Length() <= m_RequiredPlayerRange)
		{
			NumPlayers++;
		}
		return false;
	};

	m_World->ForEachEntityInBox(m_PlayerBoundingBox, Callback);

	return NumPlayers;
}





int cMobSpawnerEntity::GetNearbyMonsterNum(eMonsterType a_EntityType)
{
	int NumEntities = 0;

	auto Callback = [&] (cEntity & a_Entity)
	{
		if (!a_Entity.IsMob())
		{
			return false;
		}

		auto & Mob = static_cast<cMonster &>(a_Entity);
		if (Mob.GetMobType() == m_Entity)
		{
			NumEntities++;
		}
		return false;
	};

	m_World->ForEachEntityInBox(m_EntityBoundingBox, Callback);

	return NumEntities;
}





void cMobSpawnerEntity::SetSpawnRange(short a_SpawnRange)
{
	m_SpawnRange = std::min(a_SpawnRange, short(20));
	m_EntityBoundingBox = cBoundingBox({(float) m_Pos.x, (float) m_Pos.y - 4, (float) m_Pos.z}, m_RequiredPlayerRange, 8);
}





void cMobSpawnerEntity::SetRequiredPlayerRange(short a_RequiredPlayerRange)
{
	m_RequiredPlayerRange = a_RequiredPlayerRange;
	m_PlayerBoundingBox = cBoundingBox({(float) m_Pos.x, (float) m_Pos.y - m_RequiredPlayerRange, (float) m_Pos.z}, m_RequiredPlayerRange, m_RequiredPlayerRange * 2);
}




