
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MobSpawnerEntity.h"

#include "../World.h"
#include "../FastRandom.h"
#include "../MobSpawner.h"
#include "../ClientHandle.h"
#include "../Items/ItemSpawnEgg.h"





cMobSpawnerEntity::cMobSpawnerEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World):
	Super(a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ, a_World),
	m_Entity(mtPig),
	m_SpawnDelay(100),
	m_IsActive(false)
{
	ASSERT(a_BlockType == E_BLOCK_MOB_SPAWNER);
}





void cMobSpawnerEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = reinterpret_cast<const cMobSpawnerEntity &>(a_Src);
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
		LOGD("Changed monster spawner at {%d, %d, %d} to type %s.", GetPosX(), GetPosY(), GetPosZ(), cMonster::MobTypeToString(MonsterType).c_str());
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

	class cCallback : public cChunkCallback
	{
	public:
		cCallback(int a_RelX, int a_RelY, int a_RelZ, eMonsterType a_MobType, int a_NearbyEntitiesNum) :
			m_RelX(a_RelX),
			m_RelY(a_RelY),
			m_RelZ(a_RelZ),
			m_MobType(a_MobType),
			m_NearbyEntitiesNum(a_NearbyEntitiesNum)
		{
		}

		virtual bool Item(cChunk * a_Chunk)
		{
			auto & Random = GetRandomProvider();

			bool EntitiesSpawned = false;
			for (size_t i = 0; i < 4; i++)
			{
				if (m_NearbyEntitiesNum >= 6)
				{
					break;
				}

				int RelX = m_RelX + static_cast<int>((Random.RandReal<double>() - Random.RandReal<double>()) * 4.0);
				int RelY = m_RelY + Random.RandInt(-1, 1);
				int RelZ = m_RelZ + static_cast<int>((Random.RandReal<double>() - Random.RandReal<double>()) * 4.0);

				cChunk * Chunk = a_Chunk->GetRelNeighborChunkAdjustCoords(RelX, RelZ);
				if ((Chunk == nullptr) || !Chunk->IsValid())
				{
					continue;
				}
				EMCSBiome Biome = Chunk->GetBiomeAt(RelX, RelZ);

				if (cMobSpawner::CanSpawnHere(Chunk, RelX, RelY, RelZ, m_MobType, Biome))
				{
					double PosX = Chunk->GetPosX() * cChunkDef::Width + RelX;
					double PosZ = Chunk->GetPosZ() * cChunkDef::Width + RelZ;

					auto Monster = cMonster::NewMonsterFromType(m_MobType);
					if (Monster == nullptr)
					{
						continue;
					}

					Monster->SetPosition(PosX, RelY, PosZ);
					Monster->SetYaw(Random.RandReal(360.0f));
					if (Chunk->GetWorld()->SpawnMobFinalize(std::move(Monster)) != cEntity::INVALID_ID)
					{
						EntitiesSpawned = true;
						Chunk->BroadcastSoundParticleEffect(
							EffectID::PARTICLE_MOBSPAWN,
							static_cast<int>(PosX * 8.0),
							static_cast<int>(RelY * 8.0),
							static_cast<int>(PosZ * 8.0),
							0
						);
						m_NearbyEntitiesNum++;
					}
				}
			}
			return EntitiesSpawned;
		}
	protected:
		int m_RelX, m_RelY, m_RelZ;
		eMonsterType m_MobType;
		int m_NearbyEntitiesNum;
	} Callback(m_RelX, m_PosY, m_RelZ, m_Entity, NearbyEntities);

	if (m_World->DoWithChunk(GetChunkX(), GetChunkZ(), Callback))
	{
		ResetTimer();
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




