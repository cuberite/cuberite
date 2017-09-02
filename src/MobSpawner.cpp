
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MobSpawner.h"
#include "Mobs/IncludeAllMonsters.h"
#include "World.h"





cMobSpawner::cMobSpawner(cMonster::eFamily a_MonsterFamily, const std::set<eMonsterType>& a_AllowedTypes) :
	m_MonsterFamily(a_MonsterFamily),
	m_NewPack(true),
	m_MobType(mtInvalidType)
{
	for (std::set<eMonsterType>::const_iterator itr = a_AllowedTypes.begin(); itr != a_AllowedTypes.end(); ++itr)
	{
		if (cMonster::FamilyFromType(*itr) == a_MonsterFamily)
		{
			m_AllowedTypes.insert(*itr);
		}
	}
}





bool cMobSpawner::CheckPackCenter(BLOCKTYPE a_BlockType)
{
	// Packs of non-water mobs can only be centered on an air block
	// Packs of water mobs can only be centered on a water block
	if (m_MonsterFamily == cMonster::mfWater)
	{
		return IsBlockWater(a_BlockType);
	}
	else
	{
		return a_BlockType == E_BLOCK_AIR;
	}
}





void cMobSpawner::addIfAllowed(eMonsterType toAdd, std::vector<eMonsterType> & toAddIn)
{
	std::set<eMonsterType>::iterator itr = m_AllowedTypes.find(toAdd);
	if (itr != m_AllowedTypes.end())
	{
		toAddIn.push_back(toAdd);
	}
}





eMonsterType cMobSpawner::ChooseMobType(EMCSBiome a_Biome)
{
	std::vector<eMonsterType> allowedMobs;

	if ((a_Biome == biMushroomIsland) || (a_Biome == biMushroomShore))
	{
		addIfAllowed(mtMooshroom, allowedMobs);
	}
	else if (a_Biome == biNether)
	{
		addIfAllowed(mtGhast, allowedMobs);
		addIfAllowed(mtZombiePigman, allowedMobs);
		addIfAllowed(mtMagmaCube, allowedMobs);
	}
	else if (a_Biome == biEnd)
	{
		addIfAllowed(mtEnderman, allowedMobs);
	}
	else
	{
		addIfAllowed(mtBat, allowedMobs);
		addIfAllowed(mtSpider, allowedMobs);
		addIfAllowed(mtZombie, allowedMobs);
		addIfAllowed(mtSkeleton, allowedMobs);
		addIfAllowed(mtCreeper, allowedMobs);
		addIfAllowed(mtSquid, allowedMobs);
		addIfAllowed(mtGuardian, allowedMobs);

		if ((a_Biome != biDesert) && (a_Biome != biBeach) && (a_Biome != biOcean))
		{
			addIfAllowed(mtSheep, allowedMobs);
			addIfAllowed(mtPig, allowedMobs);
			addIfAllowed(mtCow, allowedMobs);
			addIfAllowed(mtChicken, allowedMobs);
			addIfAllowed(mtEnderman, allowedMobs);
			addIfAllowed(mtRabbit, allowedMobs);
			addIfAllowed(mtSlime, allowedMobs);  // MG TODO : much more complicated rule

			if ((a_Biome == biForest) || (a_Biome == biForestHills) || (a_Biome == biTaiga) || (a_Biome == biTaigaHills))
			{
				addIfAllowed(mtWolf, allowedMobs);
			}
			else if ((a_Biome == biJungle) || (a_Biome == biJungleHills))
			{
				addIfAllowed(mtOcelot, allowedMobs);
			}
		}
	}

	// Pick a random mob from the options:
	size_t allowedMobsSize = allowedMobs.size();
	if (allowedMobsSize > 0)
	{
		return allowedMobs[GetRandomProvider().RandInt(allowedMobsSize - 1)];
	}
	return mtInvalidType;
}





bool cMobSpawner::CanSpawnHere(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ, eMonsterType a_MobType, EMCSBiome a_Biome)
{
	if (a_Chunk == nullptr)
	{
		return false;
	}

	if (cChunkDef::IsValidHeight(a_RelY - 1) && (a_Chunk->GetBlock(a_RelX, a_RelY - 1, a_RelZ) == E_BLOCK_BEDROCK))
	{
		return false;   // Make sure mobs do not spawn on bedrock.
	}

	auto & Random = GetRandomProvider();
	BLOCKTYPE TargetBlock = a_Chunk->GetBlock(a_RelX, a_RelY, a_RelZ);

	cPlayer * a_Closest_Player = a_Chunk->GetWorld()->FindClosestPlayer(a_Chunk->PositionToWorldPosition(a_RelX, a_RelY, a_RelZ), 24);
	if (a_Closest_Player != nullptr)  // Too close to a player, bail out
	{
		return false;
	}

	if ((a_RelY >= cChunkDef::Height - 1) || (a_RelY <= 0))
	{
		return false;
	}

	NIBBLETYPE BlockLight = a_Chunk->GetBlockLight(a_RelX, a_RelY, a_RelZ);
	NIBBLETYPE SkyLight = a_Chunk->GetSkyLight(a_RelX, a_RelY, a_RelZ);
	BLOCKTYPE BlockAbove = a_Chunk->GetBlock(a_RelX, a_RelY + 1, a_RelZ);
	BLOCKTYPE BlockBelow = a_Chunk->GetBlock(a_RelX, a_RelY - 1, a_RelZ);

	SkyLight = a_Chunk->GetTimeAlteredLight(SkyLight);

	switch (a_MobType)
	{
		case mtGuardian:
		{
			return IsBlockWater(TargetBlock) && (a_RelY >= 45) && (a_RelY <= 62);
		}

		case mtSquid:
		{
			return IsBlockWater(TargetBlock) && (a_RelY >= 45) && (a_RelY <= 62);
		}

		case mtBat:
		{
			return (a_RelY <= 63) && (BlockLight <= 4) && (SkyLight <= 4) && (TargetBlock == E_BLOCK_AIR) && !cBlockInfo::IsTransparent(BlockAbove);
		}

		case mtChicken:
		case mtCow:
		case mtPig:
		case mtHorse:
		case mtRabbit:
		case mtSheep:
		{
			return (
				(TargetBlock == E_BLOCK_AIR) &&
				(BlockAbove == E_BLOCK_AIR) &&
				(!cBlockInfo::IsTransparent(BlockBelow)) &&
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
				(a_RelY >= 62) &&
				(Random.RandBool(2.0 / 3.0))
			);
		}

		case mtEnderman:
		{
			if (a_RelY < 250)
			{
				BLOCKTYPE BlockTop = a_Chunk->GetBlock(a_RelX, a_RelY + 2, a_RelZ);
				if (BlockTop == E_BLOCK_AIR)
				{
					BlockTop = a_Chunk->GetBlock(a_RelX, a_RelY + 3, a_RelZ);
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
					CanSpawn = a_Chunk->UnboundedRelGetBlockType(a_RelX + x, a_RelY, a_RelZ + z, TargetBlock);
					CanSpawn = CanSpawn && (TargetBlock == E_BLOCK_AIR);
					if (!CanSpawn)
					{
						return false;
					}
					HasFloor = (
						HasFloor ||
						(
							a_Chunk->UnboundedRelGetBlockType(a_RelX + x, a_RelY - 1, a_RelZ + z, TargetBlock) &&
							!cBlockInfo::IsTransparent(TargetBlock)
						)
					);
				}
			}
			return CanSpawn && HasFloor && (SkyLight <= 7) && (BlockLight <= 7);
		}

		case mtCaveSpider:
		{
			return (
				(TargetBlock == E_BLOCK_AIR) &&
				(!cBlockInfo::IsTransparent(BlockBelow)) &&
				(SkyLight <= 7) &&
				(BlockLight <= 7) &&
				(Random.RandBool())
			);
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
				(Random.RandBool())
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
					(a_RelY <= 40) || (a_Biome == biSwampland)
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
				(Random.RandBool(0.05))
			);
		}

		case mtWolf:
		{
			return (
				(TargetBlock == E_BLOCK_GRASS) &&
				(BlockAbove == E_BLOCK_AIR) &&
				(
					(a_Biome == biTaiga) ||
					(a_Biome == biTaigaHills) ||
					(a_Biome == biForest) ||
					(a_Biome == biForestHills) ||
					(a_Biome == biColdTaiga) ||
					(a_Biome == biColdTaigaHills) ||
					(a_Biome == biTaigaM) ||
					(a_Biome == biMegaTaiga) ||
					(a_Biome == biMegaTaigaHills)
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
					(a_Biome == biMushroomShore) ||
					(a_Biome == biMushroomIsland)
				)
			);
		}

		default:
		{
			LOGD("MG TODO: Write spawning rule for mob type %d", a_MobType);
			return false;
		}
	}
	return false;
}





cMonster * cMobSpawner::TryToSpawnHere(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ, EMCSBiome a_Biome, int & a_MaxPackSize)
{
	if (m_NewPack)
	{
		m_MobType = ChooseMobType(a_Biome);
		if (m_MobType == mtInvalidType)
		{
			return nullptr;
		}
		if (m_MobType == mtWolf)
		{
			a_MaxPackSize = 8;
		}
		else if (m_MobType == mtGhast)
		{
			a_MaxPackSize = 1;
		}
		m_NewPack = false;
	}

	// Make sure we are looking at the right chunk to spawn in
	a_Chunk = a_Chunk->GetRelNeighborChunkAdjustCoords(a_RelX, a_RelZ);

	if ((m_AllowedTypes.find(m_MobType) != m_AllowedTypes.end()) && CanSpawnHere(a_Chunk, a_RelX, a_RelY, a_RelZ, m_MobType, a_Biome))
	{
		auto newMob = cMonster::NewMonsterFromType(m_MobType);
		auto NewMobPtr = newMob.get();
		if (newMob)
		{
			m_Spawned.insert(std::move(newMob));
		}
		return NewMobPtr;
	}

	return nullptr;
}





void cMobSpawner::NewPack()
{
	m_NewPack = true;
}





cMobSpawner::tSpawnedContainer & cMobSpawner::getSpawned(void)
{
	return m_Spawned;
}





bool cMobSpawner::CanSpawnAnything(void)
{
	return !m_AllowedTypes.empty();
}




