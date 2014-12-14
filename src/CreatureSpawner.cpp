
// CreatureSpawner.cpp

// This class handle the mob spawning in a world.





#include "Globals.h"
#include "CreatureSpawner.h"
#include "World.h"
#include "Entities/Player.h"
#include "Mobs/Monster.h"
#include "Root.h"





cCreatureSpawner::cCreatureSpawner(cWorld * a_World)
	: m_World(a_World)
{
	ASSERT(m_World != nullptr);
}





void cCreatureSpawner::LoadAllowedTypes(const std::set<eMonsterType> & a_AllowedTypes)
{
	for (std::set<eMonsterType>::const_iterator itr = a_AllowedTypes.begin(); itr != a_AllowedTypes.end(); ++itr)
	{
		m_AllowedTypes.insert(*itr);
	}
}





int cCreatureSpawner::SpawnRandomCreatures(bool a_SpawnMonsters, bool a_SpawnAnimals, bool a_SpawnWater)
{
	int Spawned = 0;
	for (int i = 0; i < cMonster::mfNoSpawn; i++)
	{
		cMonster::eFamily Family = static_cast<cMonster::eFamily>(i);

		if (
			((Family == cMonster::mfPassive || Family == cMonster::mfAmbient) && a_SpawnAnimals) ||
			((Family == cMonster::mfHostile && a_SpawnMonsters)) ||
			((Family == cMonster::mfWater) && a_SpawnWater)
		)
		{
			Spawned += SpawnRandomCreatures(Family);
		}
	}
	return Spawned;
}





int cCreatureSpawner::SpawnRandomCreatures(cMonster::eFamily a_Family)
{
	if ((m_World->GetWorldAge() % 100) == 0)
	{
		// Update the chunks all 5 seconds
		UpdateChunks();
	}
	if (m_Chunks.empty())
	{
		return 0;
	}

	int Limit = GetSpawnLimit(a_Family);
	int MobCount = m_World->GetMonstersNum(a_Family);
	int SpawnLimit = Limit * m_Chunks.size() / 256;
	if ((Limit == 0) || (MobCount >= SpawnLimit))
	{
		// Too many mobs
		return 0;
	}

	cFastRandom Random;
	int SpawnedMobs = 0;
	for (auto ChunkCoords : m_Chunks)
	{
		int SpawnsInThisChunk = 0;
		if (SpawnedMobs >= SpawnLimit)
		{
			return SpawnedMobs;
		}

		int RandomX = ChunkCoords.m_ChunkX * cChunkDef::Width + Random.NextInt(16);
		int RandomY = Random.NextInt(cChunkDef::Height);
		int RandomZ = ChunkCoords.m_ChunkZ * cChunkDef::Width + Random.NextInt(16);

		if (cBlockInfo::IsSolid(m_World->GetBlock(RandomX, RandomY, RandomZ)))
		{
			continue;
		}

		size_t Tries = 0;
		while (Tries++ < 3)
		{
			eMonsterType LastMonsterType = mtInvalidType;

			int BlockX = RandomX;
			int BlockY = RandomY;
			int BlockZ = RandomZ;

			for (size_t SpawnTry = 0; SpawnTry < 4; SpawnTry++)
			{
				BlockX += Random.NextInt(6) - Random.NextInt(6);
				BlockZ += Random.NextInt(6) - Random.NextInt(6);

				if (!CanSpawnFamily(BlockX, BlockY, BlockZ, a_Family))
				{
					continue;
				}

				// 576.0F  <=>  24 block radius
				if ((m_World->GetSpawn() - Vector3d(BlockX, BlockY, BlockZ)).SqrLength() <= 576.0)
				{
					// Spawn protection -> Abort
					continue;
				}

				if (IsPlayerInNear(BlockX, BlockY, BlockZ))
				{
					// A player is in near -> Abort
					continue;
				}

				if (LastMonsterType == mtInvalidType)
				{
					// Choose random mob type
					LastMonsterType = ChooseMobType(m_World->GetBiomeAt(BlockX, BlockZ), a_Family);
					if (LastMonsterType == mtInvalidType)
					{
						// Can't choose a type?
						SpawnLimit = -1;
						break;
					}
				}

				cMonster * Entity = cMonster::NewMonsterFromType(LastMonsterType);
				Entity->SetPosition(BlockX, BlockY, BlockZ);
				Entity->SetYaw(Random.NextFloat() * 360.0f);

				if (m_World->SpawnMobFinalize(Entity) != -1)
				{
					++SpawnedMobs;
					++SpawnsInThisChunk;

					if ((SpawnsInThisChunk >= Entity->GetMaxPackSize()) || (SpawnedMobs >= SpawnLimit))
					{
						Tries = 3;
						break;
					}
				}
			}
		}
	}

	return SpawnedMobs;
}





int cCreatureSpawner::GetSpawnLimit(cMonster::eFamily a_Family) const
{
	switch (a_Family)
	{
		case cMonster::mfAmbient: return 15;
		case cMonster::mfHostile: return 70;
		case cMonster::mfPassive: return 15;
		case cMonster::mfWater:   return 5;
		default: return 15;
	}
}





void cCreatureSpawner::UpdateChunks(void)
{
	class cCallback : public cPlayerListCallback
	{
		virtual bool Item(cPlayer * a_Player)
		{
			int MiddleX = a_Player->GetChunkX();
			int MiddleZ = a_Player->GetChunkZ();

			int MobSpawnRadius = 4;
			for (int x = -MobSpawnRadius; x <= MobSpawnRadius; x++)
			{
				for (int z = -MobSpawnRadius; z <= MobSpawnRadius; z++)
				{
					cChunkCoords Coords(MiddleX + x, MiddleZ + z);

					if (std::find(m_ChunkList.begin(), m_ChunkList.end(), Coords) == m_ChunkList.end())
					{
						m_ChunkList.push_back(Coords);
					}
				}
			}
			return false;
		}

		cChunkCoordsList & m_ChunkList;

	public:
		cCallback(cChunkCoordsList & a_ChunkList) :
			m_ChunkList(a_ChunkList)
		{}
	} Callback(m_Chunks);

	m_Chunks.clear();
	m_World->ForEachPlayer(Callback);
}





bool cCreatureSpawner::IsPlayerInNear(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	class cCallback : public cPlayerListCallback
	{
		virtual bool Item(cPlayer * a_Player)
		{
			// 576.0F  <=>  24 block radius
			if ((a_Player->GetPosition() - m_Pos).SqrLength() <= 576.0)
			{
				return true;
			}
			return false;
		}

		Vector3d m_Pos;

	public:
		cCallback(int a_BlockX, int a_BlockY, int a_BlockZ)
			: m_Pos(a_BlockX + 0.5, a_BlockY, a_BlockZ + 0.5)
		{}
	} Callback(a_BlockX, a_BlockY, a_BlockZ);

	return !m_World->ForEachPlayer(Callback);
}





bool cCreatureSpawner::CanSpawnFamily(int a_BlockX, int a_BlockY, int a_BlockZ, cMonster::eFamily a_Family)
{
	if ((a_BlockY <= 0) || (a_BlockY >= cChunkDef::Height))
	{
		return false;
	}

	BLOCKTYPE Block      = m_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
	BLOCKTYPE BlockAbove = m_World->GetBlock(a_BlockX, a_BlockY + 1, a_BlockZ);
	BLOCKTYPE BlockBelow = m_World->GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ);

	if (a_Family == cMonster::mfWater)
	{
		return (
			IsBlockLiquid(Block) &&
			IsBlockLiquid(BlockBelow) &&
			!cBlockInfo::FullyOccupiesVoxel(BlockAbove)
		);
	}
	else if (!cBlockInfo::IsSolid(m_World->GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ)))
	{
		return false;
	}
	else
	{
		return (
			(BlockBelow != E_BLOCK_BEDROCK) &&
			!cBlockInfo::FullyOccupiesVoxel(Block) &&
			!IsBlockLiquid(Block) &&
			!cBlockInfo::FullyOccupiesVoxel(BlockAbove)
		);
	}
}





bool cCreatureSpawner::CanSpawnHere(int a_BlockX, int a_BlockY, int a_BlockZ, eMonsterType a_MobType)
{
	cFastRandom Random;
	BLOCKTYPE TargetBlock = m_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ);

	if ((a_BlockY <= 0) || (a_BlockY >= cChunkDef::Height))
	{
		return false;
	}

	NIBBLETYPE BlockLight = m_World->GetBlockBlockLight(a_BlockX, a_BlockY, a_BlockZ);
	NIBBLETYPE SkyLight   = m_World->GetBlockSkyLight(a_BlockX, a_BlockY, a_BlockZ);
	BLOCKTYPE BlockAbove  = m_World->GetBlock(a_BlockX, a_BlockY + 1, a_BlockZ);
	BLOCKTYPE BlockBelow  = m_World->GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ);
	EMCSBiome Biome       = m_World->GetBiomeAt(a_BlockX, a_BlockZ);

	SkyLight -= m_World->GetSkyDarkness();
	if (SkyLight >= 16)
	{
		SkyLight = 0;
	}

	switch (a_MobType)
	{
		case mtSquid:
		{
			return IsBlockWater(TargetBlock) && (a_BlockY >= 45) && (a_BlockY <= 62);
		}

		case mtBat:
		{
			return (a_BlockY <= 63) && (BlockLight <= 4) && (SkyLight <= 4) && (TargetBlock == E_BLOCK_AIR) && !cBlockInfo::IsTransparent(BlockAbove);
		}

		case mtChicken:
		case mtCow:
		case mtPig:
		case mtHorse:
		case mtSheep:
		{
			return (
				(TargetBlock == E_BLOCK_AIR) &&
				(BlockAbove == E_BLOCK_AIR) &&
				(BlockBelow == E_BLOCK_GRASS) &&
				(SkyLight >= 9)
			);
		}

		case mtOcelot:
		{
			return (
				(TargetBlock == E_BLOCK_AIR) &&
				(BlockAbove == E_BLOCK_AIR) &&
				(
					(BlockBelow == E_BLOCK_GRASS) || (BlockBelow == E_BLOCK_LEAVES) || (BlockBelow == E_BLOCK_NEW_LEAVES)
				) &&
				(a_BlockY >= 62) &&
				(Random.NextInt(3) != 0)
			);
		}

		case mtEnderman:
		{
			if (a_BlockY < 250)
			{
				BLOCKTYPE BlockTop = m_World->GetBlock(a_BlockX, a_BlockY + 2, a_BlockZ);
				if (BlockTop == E_BLOCK_AIR)
				{
					BlockTop = m_World->GetBlock(a_BlockX, a_BlockY + 3, a_BlockZ);
					return (
						(TargetBlock == E_BLOCK_AIR) &&
						(BlockAbove == E_BLOCK_AIR) &&
						(BlockTop == E_BLOCK_AIR) &&
						(!cBlockInfo::IsTransparent(BlockBelow)) &&
						(SkyLight <= 7) &&
						(BlockLight <= 7)
					);
				}
			}
			break;
		}

		case mtSpider:
		{
			bool CanSpawn = true;
			bool HasFloor = false;
			for (int x = 0; x < 2; ++x)
			{
				for (int z = 0; z < 2; ++z)
				{
					if (m_World->GetBlock(a_BlockX + x, a_BlockY, a_BlockZ + z) == E_BLOCK_AIR)
					{
						return false;
					}
					HasFloor = (
						HasFloor || !cBlockInfo::IsTransparent(m_World->GetBlock(a_BlockX + x, a_BlockY - 1, a_BlockZ + z))
					);
				}
			}
			return CanSpawn && HasFloor && (SkyLight <= 7) && (BlockLight <= 7);
		}

		case mtCreeper:
		case mtSkeleton:
		case mtZombie:
		{
			return (
				(TargetBlock == E_BLOCK_AIR) &&
				(BlockAbove == E_BLOCK_AIR) &&
				(!cBlockInfo::IsTransparent(BlockBelow)) &&
				(SkyLight <= 7) &&
				(BlockLight <= 7) &&
				(Random.NextInt(2) == 0)
			);
		}

		case mtMagmaCube:
		case mtSlime:
		{
			return (
				(TargetBlock == E_BLOCK_AIR) &&
				(BlockAbove == E_BLOCK_AIR) &&
				(!cBlockInfo::IsTransparent(BlockBelow)) &&
				(
					(a_BlockY <= 40) || (Biome == biSwampland)
				)
			);
		}

		case mtGhast:
		case mtZombiePigman:
		{
			return (
				(TargetBlock == E_BLOCK_AIR) &&
				(BlockAbove == E_BLOCK_AIR) &&
				(!cBlockInfo::IsTransparent(BlockBelow)) &&
				(Random.NextInt(20) == 0)
			);
		}

		case mtWolf:
		{
			return (
				(TargetBlock == E_BLOCK_GRASS) &&
				(BlockAbove == E_BLOCK_AIR) &&
				(
					(Biome == biTaiga) ||
					(Biome == biTaigaHills) ||
					(Biome == biForest) ||
					(Biome == biForestHills) ||
					(Biome == biColdTaiga) ||
					(Biome == biColdTaigaHills) ||
					(Biome == biTaigaM) ||
					(Biome == biMegaTaiga) ||
					(Biome == biMegaTaigaHills)
				)
			);
		}

		case mtMooshroom:
		{
			return (
				(TargetBlock == E_BLOCK_AIR) &&
				(BlockAbove == E_BLOCK_AIR) &&
				(BlockBelow == E_BLOCK_MYCELIUM) &&
				(
					(Biome == biMushroomShore) ||
					(Biome == biMushroomIsland)
				)
			);
		}

		default:
		{
			LOGD("MG TODO: Write spawning rule for mob type %d", a_MobType);
			return false;
		}
	}
}





eMonsterType cCreatureSpawner::ChooseMobType(EMCSBiome a_Biome, cMonster::eFamily a_Family)
{
	std::vector<eMonsterType> AllowedMobs;

	if ((a_Biome == biMushroomIsland) || (a_Biome == biMushroomShore))
	{
		AddIfAllowed(mtMooshroom, AllowedMobs, a_Family);
	}
	else if (a_Biome == biNether)
	{
		AddIfAllowed(mtGhast, AllowedMobs, a_Family);
		AddIfAllowed(mtZombiePigman, AllowedMobs, a_Family);
		AddIfAllowed(mtMagmaCube, AllowedMobs, a_Family);
	}
	else if (a_Biome == biEnd)
	{
		AddIfAllowed(mtEnderman, AllowedMobs, a_Family);
	}
	else
	{
		AddIfAllowed(mtBat, AllowedMobs, a_Family);
		AddIfAllowed(mtSpider, AllowedMobs, a_Family);
		AddIfAllowed(mtZombie, AllowedMobs, a_Family);
		AddIfAllowed(mtSkeleton, AllowedMobs, a_Family);
		AddIfAllowed(mtCreeper, AllowedMobs, a_Family);
		AddIfAllowed(mtSquid, AllowedMobs, a_Family);
		
		if ((a_Biome != biDesert) && (a_Biome != biBeach) && (a_Biome != biOcean))
		{
			AddIfAllowed(mtSheep, AllowedMobs, a_Family);
			AddIfAllowed(mtPig, AllowedMobs, a_Family);
			AddIfAllowed(mtCow, AllowedMobs, a_Family);
			AddIfAllowed(mtChicken, AllowedMobs, a_Family);
			AddIfAllowed(mtEnderman, AllowedMobs, a_Family);
			AddIfAllowed(mtSlime, AllowedMobs, a_Family);  // MG TODO : much more complicated rule
			
			if ((a_Biome == biForest) || (a_Biome == biForestHills) || (a_Biome == biTaiga) || (a_Biome == biTaigaHills))
			{
				AddIfAllowed(mtWolf, AllowedMobs, a_Family);
			}
			else if ((a_Biome == biJungle) || (a_Biome == biJungleHills))
			{
				AddIfAllowed(mtOcelot, AllowedMobs, a_Family);
			}
		}
	}

	size_t AllowedMobsSize = AllowedMobs.size();
	if (AllowedMobsSize > 0)
	{
		cFastRandom Random;
		return AllowedMobs.at(Random.NextInt(AllowedMobsSize));
	}
	return mtInvalidType;
}





void cCreatureSpawner::AddIfAllowed(eMonsterType ToAdd, std::vector<eMonsterType> & ToAddIn, cMonster::eFamily a_Family)
{
	std::set<eMonsterType>::iterator itr = m_AllowedTypes.find(ToAdd);
	if ((itr != m_AllowedTypes.end()) && (cMonster::FamilyFromType(ToAdd) == a_Family))
	{
		ToAddIn.push_back(ToAdd);
	}
}




