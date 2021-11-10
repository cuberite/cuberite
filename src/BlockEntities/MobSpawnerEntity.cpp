
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MobSpawnerEntity.h"

#include "../World.h"
#include "../FastRandom.h"
#include "../MobSpawner.h"
#include "../ClientHandle.h"
#include "../Items/ItemSpawnEgg.h"





cMobSpawnerEntity::cMobSpawnerEntity(BlockState a_Block, Vector3i a_Pos, cWorld * a_World):
	Super(a_Block, a_Pos, a_World),
	m_Entity(mtPig),
	m_SpawnDelay(100),
	m_IsActive(false)
{
	ASSERT(a_Block.Type() == BlockType::Spawner);
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
	switch (a_Player->GetEquippedItem().m_ItemType)
	{
		case Item::BatSpawnEgg:
		case Item::BlazeSpawnEgg:
		case Item::CaveSpiderSpawnEgg:
		case Item::ChickenSpawnEgg:
		case Item::CowSpawnEgg:
		case Item::CreeperSpawnEgg:
		case Item::EndermanSpawnEgg:
		case Item::GhastSpawnEgg:
		case Item::GuardianSpawnEgg:
		case Item::HorseSpawnEgg:
		case Item::MagmaCubeSpawnEgg:
		case Item::MooshroomSpawnEgg:
		case Item::OcelotSpawnEgg:
		case Item::PigSpawnEgg:
		case Item::RabbitSpawnEgg:
		case Item::SheepSpawnEgg:
		case Item::SilverfishSpawnEgg:
		case Item::SkeletonSpawnEgg:
		case Item::SlimeSpawnEgg:
		case Item::SpiderSpawnEgg:
		case Item::SquidSpawnEgg:
		case Item::VillagerSpawnEgg:
		case Item::WitchSpawnEgg:
		case Item::WitherSkeletonSpawnEgg:
		case Item::WolfSpawnEgg:
		case Item::ZombieSpawnEgg:
		case Item::ZombiePigmanSpawnEgg:
		case Item::ZombieVillagerSpawnEgg:
		{
			eMonsterType MonsterType = cItemSpawnEggHandler::ItemToMonsterType(a_Player->GetEquippedItem());
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
			m_World->BroadcastBlockEntity(GetPos());
			FLOGD("Changed monster spawner at {0} to type {1}.", GetPos(), cMonster::MobTypeToString(MonsterType));
			return true;
		}
		default: return false;
	}
}





void cMobSpawnerEntity::UpdateActiveState(void)
{
	m_IsActive = (GetNearbyPlayersNum() > 0);
}





bool cMobSpawnerEntity::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	using namespace std::chrono_literals;

	// Update the active flag every 5 seconds:
	if ((m_World->GetWorldTickAge() % 5s) == 0s)
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
		m_World->BroadcastBlockEntity(GetPos());
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
}





void cMobSpawnerEntity::SpawnEntity(void)
{
	auto NearbyEntities = GetNearbyMonsterNum(m_Entity);
	if (NearbyEntities >= m_MaxNearbyEntities)
	{
		ResetTimer();
		return;
	}

	bool EntitiesSpawned = m_World->DoWithChunk(GetChunkX(), GetChunkZ(), [this, NearbyEntities](cChunk & a_Chunk)
		{
			auto & Random = GetRandomProvider();
			auto EntitySpawnTally = NearbyEntities;

			bool HaveSpawnedEntity = false;
			for (short I = 0; I < m_SpawnCount; I++)
			{
				if (EntitySpawnTally >= m_MaxNearbyEntities)
				{
					break;
				}

				auto SpawnRelPos(GetRelPos());
				SpawnRelPos += Vector3i(
					static_cast<int>((Random.RandReal<double>() - Random.RandReal<double>()) * static_cast<double>(m_SpawnRange)),
					Random.RandInt(-1, 1),
					static_cast<int>((Random.RandReal<double>() - Random.RandReal<double>()) * static_cast<double>(m_SpawnRange))
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
						EntitySpawnTally++;
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

	auto Callback = [this, &NumPlayers](cEntity & a_Entity)
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

	const cBoundingBox PlayerBoundingBox(Vector3d(m_Pos.x, m_Pos.y - m_RequiredPlayerRange, m_Pos.z), m_RequiredPlayerRange, m_RequiredPlayerRange * 2);
	m_World->ForEachEntityInBox(PlayerBoundingBox, Callback);

	return NumPlayers;
}





int cMobSpawnerEntity::GetNearbyMonsterNum(eMonsterType a_EntityType)
{
	int NumEntities = 0;

	auto Callback = [this, &NumEntities](cEntity & a_Entity)
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

	const cBoundingBox EntityBoundingBox(Vector3d(m_Pos.x, m_Pos.y - 4, m_Pos.z), m_SpawnRange, 8);
	m_World->ForEachEntityInBox(EntityBoundingBox, Callback);

	return NumEntities;
}
