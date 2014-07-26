
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MobSpawner.h"
#include "Mobs/IncludeAllMonsters.h"





cMobSpawner::cMobSpawner(cMonster::eFamily a_MonsterFamily, const std::set<cMonster::eType>& a_AllowedTypes) :
	m_MonsterFamily(a_MonsterFamily),
	m_NewPack(true),
	m_MobType(cMonster::mtInvalidType)
{
	for (std::set<cMonster::eType>::const_iterator itr = a_AllowedTypes.begin(); itr != a_AllowedTypes.end(); ++itr)
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





void cMobSpawner::addIfAllowed(cMonster::eType toAdd, std::set<cMonster::eType>& toAddIn)
{
	std::set<cMonster::eType>::iterator itr = m_AllowedTypes.find(toAdd);
	if (itr != m_AllowedTypes.end())
	{
		toAddIn.insert(toAdd);
	}
}





cMonster::eType cMobSpawner::ChooseMobType(EMCSBiome a_Biome)
{
	std::set<cMonster::eType> allowedMobs;

	if (a_Biome == biMushroomIsland || a_Biome == biMushroomShore)
	{
		addIfAllowed(cMonster::mtMooshroom, allowedMobs);
	}
	else if (a_Biome == biNether)
	{
		addIfAllowed(cMonster::mtGhast, allowedMobs);
		addIfAllowed(cMonster::mtZombiePigman, allowedMobs);
		addIfAllowed(cMonster::mtMagmaCube, allowedMobs);
	}
	else if (a_Biome == biEnd)
	{
		addIfAllowed(cMonster::mtEnderman, allowedMobs);
	}
	else
	{
		addIfAllowed(cMonster::mtBat, allowedMobs);
		addIfAllowed(cMonster::mtSpider, allowedMobs);
		addIfAllowed(cMonster::mtZombie, allowedMobs);
		addIfAllowed(cMonster::mtSkeleton, allowedMobs);
		addIfAllowed(cMonster::mtCreeper, allowedMobs);
		addIfAllowed(cMonster::mtSquid, allowedMobs);
		
		if (a_Biome != biDesert && a_Biome != biBeach && a_Biome != biOcean)
		{
			addIfAllowed(cMonster::mtSheep, allowedMobs);
			addIfAllowed(cMonster::mtPig, allowedMobs);
			addIfAllowed(cMonster::mtCow, allowedMobs);
			addIfAllowed(cMonster::mtChicken, allowedMobs);
			addIfAllowed(cMonster::mtEnderman, allowedMobs);
			addIfAllowed(cMonster::mtSlime, allowedMobs);  // MG TODO : much more complicated rule
			
			if (a_Biome == biForest || a_Biome == biForestHills || a_Biome == biTaiga  || a_Biome == biTaigaHills)
			{
				addIfAllowed(cMonster::mtWolf, allowedMobs);
			}
			else if (a_Biome == biJungle || a_Biome == biJungleHills)
			{
				addIfAllowed(cMonster::mtOcelot, allowedMobs);
			}
		}
	}

	size_t allowedMobsSize = allowedMobs.size();
	if (allowedMobsSize > 0)
	{
		std::set<cMonster::eType>::iterator itr = allowedMobs.begin();
		int iRandom = m_Random.NextInt((int)allowedMobsSize, a_Biome);

		for (int i = 0; i < iRandom; i++)
		{
			++itr;
		}

		return *itr;
	}
	return cMonster::mtInvalidType;
}





bool cMobSpawner::CanSpawnHere(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ, cMonster::eType a_MobType, EMCSBiome a_Biome)
{
	BLOCKTYPE TargetBlock = E_BLOCK_AIR;
	if (m_AllowedTypes.find(a_MobType) != m_AllowedTypes.end() && a_Chunk->UnboundedRelGetBlockType(a_RelX, a_RelY, a_RelZ, TargetBlock))
	{
		if ((a_RelY + 1 > cChunkDef::Height) || (a_RelY - 1 < 0))
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
			case cMonster::mtSquid:
			{
				return IsBlockWater(TargetBlock) && (a_RelY >= 45) && (a_RelY <= 62);
			}

			case cMonster::mtBat:
			{
				return (a_RelY <= 63) && (BlockLight <= 4) && (SkyLight <= 4) && (TargetBlock == E_BLOCK_AIR) && !cBlockInfo::IsTransparent(BlockAbove);
			}

			case cMonster::mtChicken:
			case cMonster::mtCow:
			case cMonster::mtPig:
			case cMonster::mtHorse:
			case cMonster::mtSheep:
			{
				return (
					(TargetBlock == E_BLOCK_AIR) &&
					(BlockAbove == E_BLOCK_AIR) &&
					(!cBlockInfo::IsTransparent(BlockBelow)) &&
					(BlockBelow == E_BLOCK_GRASS) &&
					(SkyLight >= 9)
				);
			}
				
			case cMonster::mtOcelot:
			{
				return (
					(TargetBlock == E_BLOCK_AIR) &&
					(BlockAbove == E_BLOCK_AIR) &&
					(
						(BlockBelow == E_BLOCK_GRASS) || (BlockBelow == E_BLOCK_LEAVES) || (BlockBelow == E_BLOCK_NEW_LEAVES)
					) &&
					(a_RelY >= 62) &&
					(m_Random.NextInt(3, a_Biome) != 0)
				);
			}
			
			case cMonster::mtEnderman:
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
			
			case cMonster::mtSpider:
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
			
			case cMonster::mtCreeper:
			case cMonster::mtSkeleton:
			case cMonster::mtZombie:
			{
				return (
					(TargetBlock == E_BLOCK_AIR) &&
					(BlockAbove == E_BLOCK_AIR) &&
					(!cBlockInfo::IsTransparent(BlockBelow)) &&
					(SkyLight <= 7) &&
					(BlockLight <= 7) &&
					(m_Random.NextInt(2, a_Biome) == 0)
				);
			}
			
			case cMonster::mtSlime:
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
			
			case cMonster::mtGhast:
			case cMonster::mtZombiePigman:
			{
				return (
					(TargetBlock == E_BLOCK_AIR) &&
					(BlockAbove == E_BLOCK_AIR) &&
					(!cBlockInfo::IsTransparent(BlockBelow)) &&
					(m_Random.NextInt(20, a_Biome) == 0)
				);
			}
			
			case cMonster::mtWolf:
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
			
			default:
			{
				LOGD("MG TODO: Write spawning rule for mob type %d", a_MobType);
				return false;
			}
		}
	}
	return false;
}





cMonster* cMobSpawner::TryToSpawnHere(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ, EMCSBiome a_Biome, int& a_MaxPackSize)
{
	cMonster* toReturn = NULL;
	if (m_NewPack)
	{
		m_MobType = ChooseMobType(a_Biome);
		if (m_MobType == cMonster::mtInvalidType)
		{
			return toReturn;
		}
		if (m_MobType == cMonster::mtWolf)
		{
			a_MaxPackSize = 8;
		}
		else if (m_MobType == cMonster::mtGhast)
		{
			a_MaxPackSize = 1;
		}
		m_NewPack = false;
	}

	// Make sure we are looking at the right chunk to spawn in
	a_Chunk = a_Chunk->GetRelNeighborChunkAdjustCoords(a_RelX, a_RelZ);
	
	if (CanSpawnHere(a_Chunk, a_RelX, a_RelY, a_RelZ, m_MobType, a_Biome))
	{
		cMonster * newMob = cMonster::NewMonsterFromType(m_MobType);
		if (newMob)
		{
			m_Spawned.insert(newMob);
		}
		toReturn = newMob;
	}
	return toReturn;
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




