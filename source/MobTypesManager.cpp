
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MobTypesManager.h"
#include "MersenneTwister.h"
#include "Mobs/Monster.h"
#include "Mobs/IncludeAllMonsters.h"
#include "FastRandom.h"





/** Map for cMonster::eType <-> string
Needs to be alpha-sorted by the strings, because binary search is used in StringToMobType()
The strings need to be lowercase (for more efficient comparisons in StringToMobType())
*/
static const struct
{
	cMonster::eType m_Type;
	const char * m_lcName;
} g_MobTypeNames[] =
{
	{cMonster::mtBat,          "bat"},
	{cMonster::mtBlaze,        "blaze"},
	{cMonster::mtCaveSpider,   "cavespider"},
	{cMonster::mtChicken,      "chicken"},
	{cMonster::mtCow,          "cow"},
	{cMonster::mtCreeper,      "creeper"},
	{cMonster::mtEnderman,     "enderman"},
	{cMonster::mtGhast,        "ghast"},
	{cMonster::mtHorse,        "horse"},
	{cMonster::mtMagmaCube,    "magmacube"},
	{cMonster::mtMooshroom,    "mooshroom"},
	{cMonster::mtOcelot,       "ocelot"},
	{cMonster::mtPig,          "pig"},
	{cMonster::mtSheep,        "sheep"},
	{cMonster::mtSilverfish,   "silverfish"},
	{cMonster::mtSkeleton,     "skeleton"},
	{cMonster::mtSlime,        "slime"},
	{cMonster::mtSpider,       "spider"},
	{cMonster::mtSquid,        "squid"},
	{cMonster::mtVillager,     "villager"},
	{cMonster::mtWitch,        "witch"},
	{cMonster::mtWolf,         "wolf"},
	{cMonster::mtZombie,       "zombie"},
	{cMonster::mtZombiePigman, "zombiepigman"},
} ;





cMonster * cMobTypesManager::NewMonsterFromType(cMonster::eType a_MobType, int a_Size)
{
	cFastRandom Random;
	
	cMonster * toReturn = NULL;

	// unspecified size get rand[1,3] for Monsters that need size
	switch (a_MobType)
	{
		case cMonster::mtMagmaCube:
		case cMonster::mtSlime:
		{
			if (a_Size == -1)
			{
				a_Size = Random.NextInt(2, a_MobType) + 1;
			}
			if ((a_Size <= 0) || (a_Size >= 4))
			{
				ASSERT(!"Random for size was supposed to pick in [1..3] and picked outside");
				a_Size = 1;
			}
			break;
		}
		default: break;
	}  // switch (a_MobType)

	// Create the mob entity
	switch (a_MobType)
	{
		case cMonster::mtMagmaCube:     toReturn  = new cMagmaCube(a_Size);             break;
		case cMonster::mtSlime:         toReturn  = new cSlime(a_Size);                 break;
		case cMonster::mtBat:           toReturn  = new cBat();                         break;
		case cMonster::mtBlaze:         toReturn  = new cBlaze();                       break;
		case cMonster::mtCaveSpider:    toReturn  = new cCavespider();                  break;
		case cMonster::mtChicken:       toReturn  = new cChicken();                     break;
		case cMonster::mtCow:           toReturn  = new cCow();                         break;
		case cMonster::mtCreeper:       toReturn  = new cCreeper();                     break;
		case cMonster::mtEnderman:      toReturn  = new cEnderman();                    break;
		case cMonster::mtGhast:         toReturn  = new cGhast();                       break;
		// TODO: 
		// case cMonster::mtHorse:         toReturn  = new cHorse();                       break;
		case cMonster::mtMooshroom:     toReturn  = new cMooshroom();                   break;
		case cMonster::mtOcelot:        toReturn  = new cOcelot();                      break;
		case cMonster::mtPig:           toReturn  = new cPig();                         break;
		// TODO: Implement sheep color
		case cMonster::mtSheep:         toReturn  = new cSheep(0);                      break;
		case cMonster::mtSilverfish:    toReturn  = new cSilverfish();                  break;
		// TODO: Implement wither skeleton geration
		case cMonster::mtSkeleton:      toReturn  = new cSkeleton(false);               break;
		case cMonster::mtSpider:        toReturn  = new cSpider();                      break;
		case cMonster::mtSquid:         toReturn  = new cSquid();                       break;
		case cMonster::mtVillager:      toReturn  = new cVillager(cVillager::vtFarmer); break;
		case cMonster::mtWitch:         toReturn  = new cWitch();                       break;
		case cMonster::mtWolf:          toReturn  = new cWolf();                        break;
		case cMonster::mtZombie:        toReturn  = new cZombie(false);                 break;
		case cMonster::mtZombiePigman:  toReturn  = new cZombiePigman();                break;
		default:
		{
			ASSERT(!"Unhandled Mob type");
		}
	}
	return toReturn;
}





AString cMobTypesManager::MobTypeToString(cMonster::eType a_MobType)
{
	// Mob types aren't sorted, so we need to search linearly:
	for (int i = 0; i < ARRAYCOUNT(g_MobTypeNames); i++)
	{
		if (g_MobTypeNames[i].m_Type == a_MobType)
		{
			return g_MobTypeNames[i].m_lcName;
		}
	}
	
	// Not found:
	return "";
}





cMonster::eType cMobTypesManager::StringToMobType(const AString & a_Name)
{
	AString lcName(a_Name);
	StrToLower(lcName);
	
	// Binary-search for the lowercase name:
	int lo = 0, hi = ARRAYCOUNT(g_MobTypeNames);
	while (hi - lo > 1)
	{
		int mid = (lo + hi) / 2;
		int res = strcmp(g_MobTypeNames[mid].m_lcName, lcName.c_str());
		if (res == 0)
		{
			return g_MobTypeNames[mid].m_Type;
		}
		if (res < 0)
		{
			hi = mid;
		}
		else
		{
			lo = mid;
		}
	}
	// Range has collapsed to at most two elements, compare each:
	if (strcmp(g_MobTypeNames[lo].m_lcName, lcName.c_str()) == 0)
	{
		return g_MobTypeNames[lo].m_Type;
	}
	if ((lo != hi) && (strcmp(g_MobTypeNames[hi].m_lcName, lcName.c_str()) == 0))
	{
		return g_MobTypeNames[hi].m_Type;
	}
	
	// Not found:
	return cMonster::mtInvalidType;
}





cMonster::eFamily cMobTypesManager::FamilyFromType(cMonster::eType a_Type)
{
	static const struct
	{
		cMonster::eType m_Type;
		cMonster::eFamily m_Family;
	} TypeMap[] =
	{
		{cMonster::mtBat,          cMonster::mfAmbient},
		{cMonster::mtBlaze,        cMonster::mfHostile},
		{cMonster::mtCaveSpider,   cMonster::mfHostile},
		{cMonster::mtChicken,      cMonster::mfPassive},
		{cMonster::mtCow,          cMonster::mfPassive},
		{cMonster::mtCreeper,      cMonster::mfHostile},
		{cMonster::mtEnderman,     cMonster::mfHostile},
		{cMonster::mtGhast,        cMonster::mfHostile},
		{cMonster::mtHorse,        cMonster::mfPassive},
		{cMonster::mtMagmaCube,    cMonster::mfHostile},
		{cMonster::mtMooshroom,    cMonster::mfHostile},
		{cMonster::mtOcelot,       cMonster::mfHostile},
		{cMonster::mtPig,          cMonster::mfPassive},
		{cMonster::mtSheep,        cMonster::mfPassive},
		{cMonster::mtSilverfish,   cMonster::mfHostile},
		{cMonster::mtSkeleton,     cMonster::mfHostile},
		{cMonster::mtSlime,        cMonster::mfHostile},
		{cMonster::mtSpider,       cMonster::mfHostile},
		{cMonster::mtSquid,        cMonster::mfWater},
		{cMonster::mtVillager,     cMonster::mfPassive},
		{cMonster::mtWitch,        cMonster::mfHostile},
		{cMonster::mtWolf,         cMonster::mfHostile},
		{cMonster::mtZombie,       cMonster::mfHostile},
		{cMonster::mtZombiePigman, cMonster::mfHostile},
	} ;
	
	for (int i = 0; i < ARRAYCOUNT(TypeMap); i++)
	{
		if (TypeMap[i].m_Type == a_Type)
		{
			return TypeMap[i].m_Family;
		}
	}
	return cMonster::mfMaxplusone;
}




