
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MobTypesManager.h"
#include "MersenneTwister.h"
#include "Mobs/Monster.h"
#include "Mobs/IncludeAllMonsters.h"
#include "FastRandom.h"





cMobTypesManager::tMobTypes2Names & cMobTypesManager::m_MobsTypes2Names(void)
{
	// TODO: This memory leaks
	static std::map<cMonster::eType, AString> * value = new std::map<cMonster::eType, AString>(MobTypes2NamesInitializerBeforeCx11());
	return *value;
}





cMobTypesManager::tMobTypes2Names cMobTypesManager::MobTypes2NamesInitializerBeforeCx11()
{	
	std::map<cMonster::eType, AString> toReturn;
	typedef std::map<cMonster::eType, AString>::value_type ValueType;
	// The strings need to be lowercase (for more efficient comparisons in StringToMobType())
	toReturn.insert(ValueType(cMonster::mtBat,          "bat"));
	toReturn.insert(ValueType(cMonster::mtBlaze,        "blaze"));
	toReturn.insert(ValueType(cMonster::mtCaveSpider,   "cavespider"));
	toReturn.insert(ValueType(cMonster::mtChicken,      "chicken"));
	toReturn.insert(ValueType(cMonster::mtCow,          "cow"));
	toReturn.insert(ValueType(cMonster::mtCreeper,      "creeper"));
	toReturn.insert(ValueType(cMonster::mtEnderman,     "enderman"));
	toReturn.insert(ValueType(cMonster::mtGhast,        "ghast"));
	toReturn.insert(ValueType(cMonster::mtHorse,        "horse"));
	toReturn.insert(ValueType(cMonster::mtMagmaCube,    "magmacube"));
	toReturn.insert(ValueType(cMonster::mtMooshroom,    "mooshroom"));
	toReturn.insert(ValueType(cMonster::mtOcelot,       "ocelot"));
	toReturn.insert(ValueType(cMonster::mtPig,          "pig"));
	toReturn.insert(ValueType(cMonster::mtSheep,        "sheep"));
	toReturn.insert(ValueType(cMonster::mtSilverfish,   "silverfish"));
	toReturn.insert(ValueType(cMonster::mtSkeleton,     "skeleton"));
	toReturn.insert(ValueType(cMonster::mtSlime,        "slime"));
	toReturn.insert(ValueType(cMonster::mtSpider,       "spider"));
	toReturn.insert(ValueType(cMonster::mtSquid,        "squid"));
	toReturn.insert(ValueType(cMonster::mtVillager,     "villager"));
	toReturn.insert(ValueType(cMonster::mtWitch,        "witch"));
	toReturn.insert(ValueType(cMonster::mtWolf,         "wolf"));
	toReturn.insert(ValueType(cMonster::mtZombie,       "zombie"));
	toReturn.insert(ValueType(cMonster::mtZombiePigman, "zombiepigman"));
	return toReturn;
}





cMobTypesManager::tMobType2Family & cMobTypesManager::m_MobsType2Family(void)
{
	// TODO: This memory is leaked:
	static std::map<cMonster::eType,cMonster::eFamily> * value = new std::map<cMonster::eType,cMonster::eFamily>(MobType2FamilyInitializerBeforeCx11());
	return *value;
}





cMobTypesManager::tMobType2Family cMobTypesManager::MobType2FamilyInitializerBeforeCx11()
{
	std::map<cMonster::eType,cMonster::eFamily> toReturn;
	typedef std::map<cMonster::eType,cMonster::eFamily>::value_type ValueType;
	toReturn.insert(ValueType(cMonster::mtBat,          cMonster::mfAmbient));
	toReturn.insert(ValueType(cMonster::mtBlaze,        cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtCaveSpider,   cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtChicken,      cMonster::mfPassive));
	toReturn.insert(ValueType(cMonster::mtCow,          cMonster::mfPassive));
	toReturn.insert(ValueType(cMonster::mtCreeper,      cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtEnderman,     cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtGhast,        cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtHorse,        cMonster::mfPassive));
	toReturn.insert(ValueType(cMonster::mtMagmaCube,    cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtMooshroom,    cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtOcelot,       cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtPig,          cMonster::mfPassive));
	toReturn.insert(ValueType(cMonster::mtSheep,        cMonster::mfPassive));
	toReturn.insert(ValueType(cMonster::mtSilverfish,   cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtSkeleton,     cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtSlime,        cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtSpider,       cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtSquid,        cMonster::mfWater));
	toReturn.insert(ValueType(cMonster::mtVillager,     cMonster::mfPassive));
	toReturn.insert(ValueType(cMonster::mtWitch,        cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtWolf,         cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtZombie,       cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtZombiePigman, cMonster::mfHostile));

	return toReturn;
}





cFastRandom & cMobTypesManager::m_Random(void)
{
	// TODO: This memory is leaked:
	static cFastRandom * value = new cFastRandom();
	return *value;
}





cMonster * cMobTypesManager::NewMonsterFromType(cMonster::eType a_MobType, int a_Size)
{
	cMonster * toReturn = NULL;

	// unspecified size get rand[1,3] for Monsters that need size
	switch (a_MobType)
	{
		case cMonster::mtMagmaCube:
		case cMonster::mtSlime:
		{
			if (a_Size == -1)
			{
				a_Size = m_Random().NextInt(2, a_MobType) + 1;
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
	std::map<cMonster::eType, AString>::const_iterator itr = m_MobsTypes2Names().find(a_MobType);
	if (itr != m_MobsTypes2Names().end())
	{
		return itr->second;
	}
	return "";
}





cMonster::eType cMobTypesManager::StringToMobType(const AString & a_Name)
{
	AString lcName(a_Name);
	StrToLower(lcName);
	for (std::map<cMonster::eType, AString>::const_iterator itr = m_MobsTypes2Names().begin(); itr != m_MobsTypes2Names().end(); itr++)
	{
		if (itr->second == a_Name)
		{
			return itr->first;			
		}
	}
	return cMonster::mtInvalidType;
}





cMonster::eFamily cMobTypesManager::FamilyFromType(cMonster::eType a_Type)
{
	cMonster::eFamily toReturn = cMonster::mfMaxplusone;
	std::map<cMonster::eType, cMonster::eFamily>::const_iterator itr = m_MobsType2Family().find(a_Type);
	if (itr != m_MobsType2Family().end())
	{
		toReturn = itr->second;
	}
	return toReturn;
}




