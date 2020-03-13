
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





eMonsterType cMobSpawner::ChooseMobType(EMCSBiome a_Biome)
{
	std::vector<eMonsterType> AllowedMobs;

	for (eMonsterType MobType : GetAllowedMobTypes(a_Biome))
	{
		auto itr = m_AllowedTypes.find(MobType);
		if (itr != m_AllowedTypes.end())
		{
			AllowedMobs.push_back(MobType);
		}
	}

	// Pick a random mob from the options:
	if (AllowedMobs.empty())
	{
		return mtInvalidType;
	}

	return AllowedMobs[GetRandomProvider().RandInt(AllowedMobs.size() - 1)];
}





bool cMobSpawner::CanSpawnHere(cChunk * a_Chunk, Vector3i a_RelPos, eMonsterType a_MobType, EMCSBiome a_Biome)
{
	if (a_Chunk == nullptr)
	{
		return false;
	}
	if ((a_RelPos.y >= cChunkDef::Height - 1) || (a_RelPos.y <= 0))
	{
		return false;
	}

	if (cChunkDef::IsValidHeight(a_RelPos.y - 1) && (a_Chunk->GetBlock(a_RelPos.addedY(-1)) == E_BLOCK_BEDROCK))
	{
		return false;   // Make sure mobs do not spawn on bedrock.
	}

	auto & random = GetRandomProvider();
	auto targetBlock = a_Chunk->GetBlock(a_RelPos);

	// If too close to any player, don't spawn anything
	auto absPos = a_Chunk->RelativeToAbsolute(a_RelPos);
	static const double rangeLimit = 24;
	if (a_Chunk->GetWorld()->DoWithNearestPlayer(absPos, rangeLimit, [](cPlayer & a_Player) -> bool
		{
			return true;
		})
	)
	{
		return false;
	}

	auto blockLight = a_Chunk->GetBlockLight(a_RelPos);
	auto skyLight = a_Chunk->GetSkyLight(a_RelPos);
	auto blockAbove = a_Chunk->GetBlock(a_RelPos.addedY(1));
	auto blockBelow = a_Chunk->GetBlock(a_RelPos.addedY(-1));

	skyLight = a_Chunk->GetTimeAlteredLight(skyLight);

	switch (a_MobType)
	{
		case mtGuardian:
		{
			return IsBlockWater(targetBlock) && IsBlockWater(blockBelow) && (a_RelPos.y >= 45) && (a_RelPos.y <= 62);
		}

		case mtSquid:
		{
			return IsBlockWater(targetBlock) && (a_RelPos.y >= 45) && (a_RelPos.y <= 62);
		}

		case mtBat:
		{
			return (a_RelPos.y <= 63) && (blockLight <= 4) && (skyLight <= 4) && (targetBlock == E_BLOCK_AIR) && !cBlockInfo::IsTransparent(blockAbove);
		}

		case mtChicken:
		case mtCow:
		case mtPig:
		case mtHorse:
		case mtRabbit:
		case mtSheep:
		{
			return (
				(targetBlock == E_BLOCK_AIR) &&
				(blockAbove == E_BLOCK_AIR) &&
				(!cBlockInfo::IsTransparent(blockBelow)) &&
				(blockBelow == E_BLOCK_GRASS) &&
				(skyLight >= 9)
			);
		}

		case mtOcelot:
		{
			return (
				(targetBlock == E_BLOCK_AIR) &&
				(blockAbove == E_BLOCK_AIR) &&
				(
					(blockBelow == E_BLOCK_GRASS) || (blockBelow == E_BLOCK_LEAVES) || (blockBelow == E_BLOCK_NEW_LEAVES)
				) &&
				(a_RelPos.y >= 62) &&
				(random.RandBool(2.0 / 3.0))
			);
		}

		case mtEnderman:
		{
			if (a_RelPos.y < 250)
			{
				auto blockTop = a_Chunk->GetBlock(a_RelPos.addedY(2));
				if (blockTop == E_BLOCK_AIR)
				{
					blockTop = a_Chunk->GetBlock(a_RelPos.addedY(3));
					return (
						(targetBlock == E_BLOCK_AIR) &&
						(blockAbove == E_BLOCK_AIR) &&
						(blockTop == E_BLOCK_AIR) &&
						(!cBlockInfo::IsTransparent(blockBelow)) &&
						(skyLight <= 7) &&
						(blockLight <= 7)
					);
				}
			}
			break;
		}

		case mtSpider:
		{
			bool canSpawn = true;
			bool hasFloor = false;
			for (int x = 0; x < 2; ++x)
			{
				for (int z = 0; z < 2; ++z)
				{
					canSpawn = a_Chunk->UnboundedRelGetBlockType(a_RelPos.addedXZ(x, z), targetBlock);
					canSpawn = canSpawn && (targetBlock == E_BLOCK_AIR);
					if (!canSpawn)
					{
						return false;
					}
					hasFloor = (
						hasFloor ||
						(
							a_Chunk->UnboundedRelGetBlockType(a_RelPos + Vector3i(x, -1, z), targetBlock) &&
							!cBlockInfo::IsTransparent(targetBlock)
						)
					);
				}
			}
			return canSpawn && hasFloor && (skyLight <= 7) && (blockLight <= 7);
		}

		case mtCaveSpider:
		{
			return (
				(targetBlock == E_BLOCK_AIR) &&
				(!cBlockInfo::IsTransparent(blockBelow)) &&
				(skyLight <= 7) &&
				(blockLight <= 7) &&
				(random.RandBool())
			);
		}

		case mtCreeper:
		case mtSkeleton:
		case mtZombie:
		{
			return (
				(targetBlock == E_BLOCK_AIR) &&
				(blockAbove == E_BLOCK_AIR) &&
				(!cBlockInfo::IsTransparent(blockBelow)) &&
				(skyLight <= 7) &&
				(blockLight <= 7) &&
				(random.RandBool())
			);
		}

		case mtMagmaCube:
		case mtSlime:
		{
			return (
				(targetBlock == E_BLOCK_AIR) &&
				(blockAbove == E_BLOCK_AIR) &&
				(!cBlockInfo::IsTransparent(blockBelow)) &&
				(
					(a_RelPos.y <= 40) || (a_Biome == biSwampland)
				)
			);
		}

		case mtGhast:
		case mtZombiePigman:
		{
			return (
				(targetBlock == E_BLOCK_AIR) &&
				(blockAbove == E_BLOCK_AIR) &&
				(!cBlockInfo::IsTransparent(blockBelow)) &&
				(random.RandBool(0.05))
			);
		}

		case mtWolf:
		{
			return (
				(targetBlock == E_BLOCK_GRASS) &&
				(blockAbove == E_BLOCK_AIR) &&
				(
					(a_Biome == biColdTaiga) ||
					(a_Biome == biColdTaigaHills) ||
					(a_Biome == biColdTaigaM) ||
					(a_Biome == biForest) ||
					(a_Biome == biTaiga) ||
					(a_Biome == biTaigaHills) ||
					(a_Biome == biTaigaM) ||
					(a_Biome == biMegaTaiga) ||
					(a_Biome == biMegaTaigaHills)
				)
			);
		}

		case mtMooshroom:
		{
			return (
				(targetBlock == E_BLOCK_AIR) &&
				(blockAbove == E_BLOCK_AIR) &&
				(blockBelow == E_BLOCK_MYCELIUM) &&
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





std::set<eMonsterType> cMobSpawner::GetAllowedMobTypes(EMCSBiome a_Biome)
{
	std::set<eMonsterType> ListOfSpawnables;
	// Check biomes first to get a list of animals
	switch (a_Biome)
	{
		// Nether mobs and endermen only - no other mobs in the nether
		case biNether:
		{
			ListOfSpawnables.insert(mtGhast);
			ListOfSpawnables.insert(mtMagmaCube);
			ListOfSpawnables.insert(mtZombiePigman);
			ListOfSpawnables.insert(mtEnderman);
			return ListOfSpawnables;
		}

		// Endermen only - no other mobs in the end
		case biEnd:
		{
			ListOfSpawnables.insert(mtEnderman);
			return ListOfSpawnables;
		}

		// Mooshroom only - no other mobs on mushroom islands
		case biMushroomIsland:
		case biMushroomShore:
		{
			ListOfSpawnables.insert(mtMooshroom);
			return ListOfSpawnables;
		}

		// Add Squid in ocean and river biomes
		case biOcean:
		case biFrozenOcean:
		case biFrozenRiver:
		case biRiver:
		case biDeepOcean:
		{
			ListOfSpawnables.insert(mtGuardian);
			break;
		}

		// Add ocelots in jungle biomes
		case biJungle:
		case biJungleHills:
		case biJungleEdge:
		case biJungleM:
		case biJungleEdgeM:
		{
			ListOfSpawnables.insert(mtOcelot);
			break;
		}

		// Add horses in plains-like biomes
		case biPlains:
		case biSunflowerPlains:
		case biSavanna:
		case biSavannaPlateau:
		case biSavannaM:
		case biSavannaPlateauM:
		{
			ListOfSpawnables.insert(mtHorse);
			break;
		}

		// Add wolves in forest biomes
		case biForest:
		{
			ListOfSpawnables.insert(mtWolf);
			break;
		}

		// Add wolves and rabbits in all taiga biomes
		case biColdTaiga:
		case biColdTaigaM:
		case biColdTaigaHills:
		case biTaiga:
		case biTaigaHills:
		case biTaigaM:
		case biMegaTaiga:
		case biMegaTaigaHills:
		{
			ListOfSpawnables.insert(mtWolf);
			ListOfSpawnables.insert(mtRabbit);
			break;
		}

		// Add rabbits in desert and flower forest biomes
		case biDesert:
		case biDesertHills:
		case biDesertM:
		case biFlowerForest:
		{
			ListOfSpawnables.insert(mtRabbit);
			break;
		}

		// Nothing special about this biome
		default:
		{
			break;
		}
	}

	if (
		(a_Biome != biDesertHills) &&
		(a_Biome != biDesert) &&
		(a_Biome != biDesertM) &&
		(a_Biome != biBeach) &&
		(a_Biome != biOcean) &&
		(a_Biome != biDeepOcean))
	{
		ListOfSpawnables.insert(mtSheep);
		ListOfSpawnables.insert(mtPig);
		ListOfSpawnables.insert(mtCow);
		ListOfSpawnables.insert(mtChicken);
		ListOfSpawnables.insert(mtEnderman);
		ListOfSpawnables.insert(mtSlime);
	}

	ListOfSpawnables.insert(mtBat);
	ListOfSpawnables.insert(mtSpider);
	ListOfSpawnables.insert(mtZombie);
	ListOfSpawnables.insert(mtSkeleton);
	ListOfSpawnables.insert(mtCreeper);
	ListOfSpawnables.insert(mtSquid);

	return ListOfSpawnables;
}





cMonster * cMobSpawner::TryToSpawnHere(cChunk * a_Chunk, Vector3i a_RelPos, EMCSBiome a_Biome, int & a_MaxPackSize)
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
	a_Chunk = a_Chunk->GetRelNeighborChunkAdjustCoords(a_RelPos);

	if ((m_AllowedTypes.find(m_MobType) != m_AllowedTypes.end()) && CanSpawnHere(a_Chunk, a_RelPos, m_MobType, a_Biome))
	{
		auto newMob = cMonster::NewMonsterFromType(m_MobType);
		auto NewMobPtr = newMob.get();
		if (newMob)
		{
			m_Spawned.push_back(std::move(newMob));
		}
		return NewMobPtr;
	}

	return nullptr;
}





void cMobSpawner::NewPack()
{
	m_NewPack = true;
}





bool cMobSpawner::CanSpawnAnything(void)
{
	return !m_AllowedTypes.empty();
}




