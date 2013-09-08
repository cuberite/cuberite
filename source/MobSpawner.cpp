
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MobSpawner.h"
#include "MobTypesManager.h"
#include "Mobs/Monster.h"
#include "Mobs/IncludeAllMonsters.h"

#include <sstream>

cMobSpawner::tMobTypes& cMobSpawner::m_MobTypes()
{
	static tMobTypes* value = new tMobTypes(initMobTypesBeforeCx11());
	return *value;
}

cMobSpawner::tMobTypes cMobSpawner::initMobTypesBeforeCx11()
{
	std::set<cMonster::eType> toReturn;
	toReturn.insert(cMonster::mtCreeper);
	toReturn.insert(cMonster::mtSkeleton);
	toReturn.insert(cMonster::mtSpider);
	toReturn.insert(cMonster::mtGiant);
	toReturn.insert(cMonster::mtZombie);
	toReturn.insert(cMonster::mtSlime);
	toReturn.insert(cMonster::mtGhast);
	toReturn.insert(cMonster::mtZombiePigman);
	toReturn.insert(cMonster::mtEnderman);
	toReturn.insert(cMonster::mtCaveSpider);
	toReturn.insert(cMonster::mtSilverfish);
	toReturn.insert(cMonster::mtBlaze);
	toReturn.insert(cMonster::mtMagmaCube);
	toReturn.insert(cMonster::mtEnderDragon);
	toReturn.insert(cMonster::mtWither);
	toReturn.insert(cMonster::mtBat);
	toReturn.insert(cMonster::mtWitch);
	toReturn.insert(cMonster::mtPig);
	toReturn.insert(cMonster::mtSheep);
	toReturn.insert(cMonster::mtCow);
	toReturn.insert(cMonster::mtChicken);
	toReturn.insert(cMonster::mtSquid);
	toReturn.insert(cMonster::mtWolf);
	toReturn.insert(cMonster::mtMooshroom);
	toReturn.insert(cMonster::mtSnowGolem);
	toReturn.insert(cMonster::mtOcelot);
	toReturn.insert(cMonster::mtIronGolem);
	toReturn.insert(cMonster::mtVillager);
	return toReturn;
}


cMobSpawner::cMobSpawner(cMonster::eFamily a_MonsterFamily,const std::set<cMonster::eType>& a_AllowedTypes) :
	m_MonsterFamily(a_MonsterFamily),
	m_NewPack(true),
	m_MobType(cMonster::mtInvalidType)
{
	for (std::set<cMonster::eType>::const_iterator itr = a_AllowedTypes.begin(); itr != a_AllowedTypes.end(); itr++)
	{
		if (cMobTypesManager::getFamilyFromType(*itr) == a_MonsterFamily)
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


bool cMobSpawner::CanSpawnHere(cMonster::eType a_MobType, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, BLOCKTYPE a_BlockType_below, NIBBLETYPE a_BlockMeta_below, BLOCKTYPE a_BlockType_above, NIBBLETYPE a_BlockMeta_above, EMCSBiome a_Biome, int a_Level)
{
	bool toReturn = false;
	std::set<cMonster::eType>::iterator itr = m_AllowedTypes.find(a_MobType);
	if (itr != m_AllowedTypes.end())
	{
		// MG TODO : find a nicer paging
		if (a_MobType == cMonster::mtSquid)
		{
			toReturn = (
				IsBlockLiquid(a_BlockType) &&
				a_Level >= 45 &&
				a_Level <= 62
			);
		}
		else if (a_MobType == cMonster::mtBat)
		{
			toReturn = a_Level <= 60; // MG TODO  : find a real rule
		}
		else
		{
			if (
				a_BlockType == E_BLOCK_AIR &&
				a_BlockType_above == E_BLOCK_AIR &&
				! (g_BlockTransparent[a_BlockType_below])
				)
			{
				if (a_MobType == cMonster::mtChicken || a_MobType == cMonster::mtPig || a_MobType == cMonster::mtCow || a_MobType == cMonster::mtSheep)
				{
					LOGD(oss.str().c_str());
					toReturn = (
						a_BlockType_below == E_BLOCK_GRASS /*&& // MG TODO
						a_LightLevel >= 9 */
						);
				}
				else if (a_MobType == cMonster::mtOcelot)
				{
					toReturn = (
						a_Level >= 62 &&
						(
							a_BlockType_below == E_BLOCK_GRASS ||
							a_BlockType_below == E_BLOCK_LEAVES
						) &&
						m_Random.NextInt(3,a_Biome) != 0
						);
				}
				else if (a_MobType == cMonster::mtCreeper || a_MobType == cMonster::mtSkeleton || a_MobType == cMonster::mtZombie || a_MobType == cMonster::mtSpider || a_MobType == cMonster::mtEnderman || a_MobType == cMonster::mtZombiePigman)
				{
					toReturn = true /*a_LightLevel <= 7 MG TODO*/;
					/*if (a_SunLight) MG TODO 
					{
						if (m_Random.NextInt(2,a_Biome) != 0)
						{
							toReturn = false;
						}
					}*/
				}
				else if (a_MobType == cMonster::mtSlime)
				{
					toReturn = a_Level <= 40;
					// MG TODO : much more complicated rules		
				}
				else if (a_MobType == cMonster::mtGhast)
				{
					toReturn = m_Random.NextInt(20,a_Biome) == 0;
				}
				else
				{
					LOGD("MG TODO : check I've got a Rule to write for type %d",a_MobType);
					toReturn = true;
				}
			}
		}
	}
	return toReturn;
}


cMonster* cMobSpawner::TryToSpawnHere(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, BLOCKTYPE a_BlockType_below, NIBBLETYPE a_BlockMeta_below, BLOCKTYPE a_BlockType_above, NIBBLETYPE a_BlockMeta_above, EMCSBiome a_Biome, int a_Level, int& a_MaxPackSize)
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

	
	if (CanSpawnHere(m_MobType, a_BlockType, a_BlockMeta, a_BlockType_below, a_BlockMeta_below, a_BlockType_above, a_BlockMeta_above, a_Biome, a_Level))
	{
		cMonster* newMob = cMobTypesManager::NewMonsterFromType(m_MobType);
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

cMobSpawner::tSpawnedContainer& cMobSpawner::getSpawned()
{
	return m_Spawned;
}

bool cMobSpawner::CanSpawnSomething()
{
	return m_AllowedTypes.size() > 0;
}
