
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MobSpawner.h"
#include "Mobs/IncludeAllMonsters.h"





cMobSpawner::cMobSpawner(cMonster::eFamily a_MonsterFamily,const std::set<cMonster::eType>& a_AllowedTypes) :
	m_MonsterFamily(a_MonsterFamily),
	m_NewPack(true),
	m_MobType(cMonster::mtInvalidType)
{
	for (std::set<cMonster::eType>::const_iterator itr = a_AllowedTypes.begin(); itr != a_AllowedTypes.end(); itr++)
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
	/*else if (a_Biome == biEnder)	MG TODO : figure out what are the biomes of the ender
	{
		addIfAllowed(cMonster::mtEnderman, allowedMobs);
	}*/
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
			addIfAllowed(cMonster::mtSlime, allowedMobs);	// MG TODO : much more complicated rule
			
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

	int allowedMobsSize = allowedMobs.size();
	if (allowedMobsSize > 0)
	{
		std::set<cMonster::eType>::iterator itr = allowedMobs.begin();
		int iRandom = m_Random.NextInt(allowedMobsSize,a_Biome);

		for(int i = 0; i < iRandom; i++)
		{
			itr++;
		}

		return *itr;
	}
	return cMonster::mtInvalidType;
}





bool cMobSpawner::CanSpawnHere(cMonster::eType a_MobType, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, BLOCKTYPE a_BlockType_below, NIBBLETYPE a_BlockMeta_below, BLOCKTYPE a_BlockType_above, NIBBLETYPE a_BlockMeta_above, NIBBLETYPE a_Skylight, NIBBLETYPE a_Blocklight, EMCSBiome a_Biome, int a_Level)
{
	if (m_AllowedTypes.find(a_MobType) != m_AllowedTypes.end())
	{
		switch(a_MobType)
		{
			case cMonster::mtSquid:
				return IsBlockWater(a_BlockType) && (a_Level >= 45) && (a_Level <= 62);

			case cMonster::mtBat:
				return a_Level <= 63 && (a_Skylight == 0) && (a_Blocklight <= 4) && (a_BlockType == E_BLOCK_AIR) && (!g_BlockTransparent[a_BlockType_above]);

			case cMonster::mtChicken:
			case cMonster::mtCow:
			case cMonster::mtPig:
			case cMonster::mtHorse:
			case cMonster::mtSheep:
				return (a_BlockType == E_BLOCK_AIR) && (a_BlockType_above == E_BLOCK_AIR) && (!g_BlockTransparent[a_BlockType_below]) &&
						(a_BlockType_below == E_BLOCK_GRASS) && (a_Skylight >= 9);

			case cMonster::mtOcelot:
				return (a_BlockType == E_BLOCK_AIR) && (a_BlockType_above == E_BLOCK_AIR) &&
						((a_BlockType_below == E_BLOCK_GRASS) || (a_BlockType_below == E_BLOCK_LEAVES)) && (a_Level >= 62) && (m_Random.NextInt(3,a_Biome) != 0);

			case cMonster::mtEnderman:
			case cMonster::mtCreeper:
			case cMonster::mtZombie:
			case cMonster::mtSpider:
				return (a_BlockType == E_BLOCK_AIR) && (a_BlockType_above == E_BLOCK_AIR) && (!g_BlockTransparent[a_BlockType_below]) &&
						(a_Skylight <= 7) && (a_Blocklight <= 7) && (m_Random.NextInt(2,a_Biome) == 0);

			case cMonster::mtSlime:
				return (a_BlockType == E_BLOCK_AIR) && (a_BlockType_above == E_BLOCK_AIR) && (!g_BlockTransparent[a_BlockType_below]) &&
						(a_Level <= 40);
			case cMonster::mtGhast:
			case cMonster::mtZombiePigman:
				return (a_BlockType == E_BLOCK_AIR) && (a_BlockType_above == E_BLOCK_AIR) && (!g_BlockTransparent[a_BlockType_below]) &&
						(m_Random.NextInt(20,a_Biome) == 0);
			default:
				LOGD("MG TODO : check I've got a Rule to write for type %d",a_MobType);
				return false;
		}
	}
}





cMonster* cMobSpawner::TryToSpawnHere(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, BLOCKTYPE a_BlockType_below, NIBBLETYPE a_BlockMeta_below, BLOCKTYPE a_BlockType_above, NIBBLETYPE a_BlockMeta_above, NIBBLETYPE a_Skylight, NIBBLETYPE a_Blocklight, EMCSBiome a_Biome, int a_Level, int& a_MaxPackSize)
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

	
	if (CanSpawnHere(m_MobType, a_BlockType, a_BlockMeta, a_BlockType_below, a_BlockMeta_below, a_BlockType_above, a_BlockMeta_above, a_Skylight, a_Blocklight, a_Biome, a_Level))
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




