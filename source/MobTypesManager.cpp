
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MobTypesManager.h"
#include "MersenneTwister.h"
#include "Mobs/Monster.h"
#include "Mobs/IncludeAllMonsters.h"
#include "FastRandom.h"


cMobTypesManager::tMobTypes2Names& cMobTypesManager::m_MobsTypes2Names()
{
	static std::map<cMonster::eType,std::string>* value = new std::map<cMonster::eType,std::string>(MobTypes2NamesInitializerBeforeCx11());
	return *value;
}

cMobTypesManager::tMobTypes2Names cMobTypesManager::MobTypes2NamesInitializerBeforeCx11()
{	
	std::map<cMonster::eType,std::string> toReturn;
	typedef std::map<cMonster::eType,std::string>::value_type ValueType;
	toReturn.insert(ValueType(cMonster::mtMagmaCube,"Magmacube"));
	toReturn.insert(ValueType(cMonster::mtSlime,"Slime"));
	toReturn.insert(ValueType(cMonster::mtBat,"Bat"));
	toReturn.insert(ValueType(cMonster::mtBlaze,"Blaze"));
	toReturn.insert(ValueType(cMonster::mtCaveSpider,"Cavespider"));
	toReturn.insert(ValueType(cMonster::mtChicken,"Chicken"));
	toReturn.insert(ValueType(cMonster::mtCow,"Cow"));
	toReturn.insert(ValueType(cMonster::mtCreeper,"Creeper"));
	toReturn.insert(ValueType(cMonster::mtEnderman,"Enderman"));
	toReturn.insert(ValueType(cMonster::mtGhast,"Ghast"));
	toReturn.insert(ValueType(cMonster::mtMooshroom,"Mooshroom"));
	toReturn.insert(ValueType(cMonster::mtOcelot,"Ocelot"));
	toReturn.insert(ValueType(cMonster::mtPig,"Pig"));
	toReturn.insert(ValueType(cMonster::mtSheep,"Sheep"));
	toReturn.insert(ValueType(cMonster::mtSilverfish,"Silverfish"));
	toReturn.insert(ValueType(cMonster::mtSkeleton,"Skeleton"));
	toReturn.insert(ValueType(cMonster::mtSpider,"Spider"));
	toReturn.insert(ValueType(cMonster::mtSquid,"Squid"));
	toReturn.insert(ValueType(cMonster::mtVillager,"Villager"));
	toReturn.insert(ValueType(cMonster::mtWitch,"Witch"));
	toReturn.insert(ValueType(cMonster::mtWolf,"Wolf"));
	toReturn.insert(ValueType(cMonster::mtZombie,"Zombie"));
	toReturn.insert(ValueType(cMonster::mtZombiePigman,"Zombiepigman"));	
	return toReturn;
}

cMobTypesManager::tMobType2Family& cMobTypesManager::m_MobsType2Family()
{
	static std::map<cMonster::eType,cMonster::eFamily>* value = new std::map<cMonster::eType,cMonster::eFamily>(MobType2FamilyInitializerBeforeCx11());
	return *value;
}

cMobTypesManager::tMobType2Family cMobTypesManager::MobType2FamilyInitializerBeforeCx11()
{
	std::map<cMonster::eType,cMonster::eFamily> toReturn;
	typedef std::map<cMonster::eType,cMonster::eFamily>::value_type ValueType;
	toReturn.insert(ValueType(cMonster::mtBat,cMonster::mfAmbient));
	toReturn.insert(ValueType(cMonster::mtSquid,cMonster::mfWater));
	toReturn.insert(ValueType(cMonster::mtCow,cMonster::mfPassive));
	toReturn.insert(ValueType(cMonster::mtPig,cMonster::mfPassive));
	toReturn.insert(ValueType(cMonster::mtSheep,cMonster::mfPassive));
	toReturn.insert(ValueType(cMonster::mtChicken,cMonster::mfPassive));
	toReturn.insert(ValueType(cMonster::mtVillager,cMonster::mfPassive));
	toReturn.insert(ValueType(cMonster::mtMagmaCube,cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtSlime,cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtBlaze,cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtCaveSpider,cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtCreeper,cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtEnderman,cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtGhast,cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtMooshroom,cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtOcelot,cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtSilverfish,cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtSkeleton,cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtSpider,cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtWitch,cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtWolf,cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtZombie,cMonster::mfHostile));
	toReturn.insert(ValueType(cMonster::mtZombiePigman,cMonster::mfHostile));	

	return toReturn;
}


cFastRandom& cMobTypesManager::m_Random()
{
	static cFastRandom* value = new cFastRandom();
	return *value;
}


cMonster* cMobTypesManager::NewMonsterFromType(cMonster::eType a_MobType, int a_Size)
{
	cMonster * toReturn = NULL;

	// unspecified size get rand[1,3] for Monsters that need size
	switch (a_MobType)
	{
		case cMonster::mtMagmaCube:
		case cMonster::mtSlime:
			if (a_Size == -1)
			{
				a_Size = m_Random().NextInt(2,a_MobType)+1;
			}
			if (a_Size <= 0 || a_Size >= 4)
			{
				ASSERT(!"Random for size was supposed to pick in [1..3] and picked outside");
				a_Size = 1;
			}
			break;
		default : break;
	}

	// the big switch
	switch (a_MobType)
	{
		case cMonster::mtMagmaCube:     toReturn  = new cMagmacube(a_Size); break;
		case cMonster::mtSlime:         toReturn  = new cSlime(a_Size);     break;
		case cMonster::mtBat:           toReturn  = new cBat();           break;
		case cMonster::mtBlaze:         toReturn  = new cBlaze();         break;
		case cMonster::mtCaveSpider:    toReturn  = new cCavespider();    break;
		case cMonster::mtChicken:       toReturn  = new cChicken();       break;
		case cMonster::mtCow:           toReturn  = new cCow();           break;
		case cMonster::mtCreeper:       toReturn  = new cCreeper();       break;
		case cMonster::mtEnderman:      toReturn  = new cEnderman();      break;
		case cMonster::mtGhast:         toReturn  = new cGhast();         break;
		case cMonster::mtMooshroom:     toReturn  = new cMooshroom();     break;
		case cMonster::mtOcelot:        toReturn  = new cOcelot();        break;
		case cMonster::mtPig:           toReturn  = new cPig();           break;
		case cMonster::mtSheep:         toReturn  = new cSheep();         break;
		case cMonster::mtSilverfish:    toReturn  = new cSilverfish();    break;
		case cMonster::mtSkeleton:      toReturn  = new cSkeleton();      break;
		case cMonster::mtSpider:        toReturn  = new cSpider();        break;
		case cMonster::mtSquid:         toReturn  = new cSquid();         break;
		case cMonster::mtVillager:      toReturn  = new cVillager();      break;
		case cMonster::mtWitch:         toReturn  = new cWitch();         break;
		case cMonster::mtWolf:          toReturn  = new cWolf();          break;
		case cMonster::mtZombie:        toReturn  = new cZombie();        break;
		case cMonster::mtZombiePigman:  toReturn  = new cZombiepigman();  break;
		default:
		{
			ASSERT(!"Unhandled Mob type");
		}
	}
	return toReturn;
}


const std::string& cMobTypesManager::fromMobTypeToString(cMonster::eType a_MobType)
{
	static std::string toReturnDefault = "";
	std::string& toReturn = toReturnDefault;
	std::map<cMonster::eType,std::string>::const_iterator itr = m_MobsTypes2Names().find(a_MobType);
	if (itr != m_MobsTypes2Names().end())
	{
		toReturn = itr->second;
	}
	return toReturn;
}

cMonster::eType cMobTypesManager::fromStringToMobType(const std::string& a_Name)
{
	for(std::map<cMonster::eType,std::string>::const_iterator itr = m_MobsTypes2Names().begin(); itr != m_MobsTypes2Names().end(); itr++)
	{
		if (itr->second == a_Name)
		{
			return itr->first;			
		}
	}
	return cMonster::mtInvalidType;
}

cMonster::eFamily cMobTypesManager::getFamilyFromType(cMonster::eType a_Type)
{
	cMonster::eFamily toReturn = cMonster::mfMaxplusone;
	std::map<cMonster::eType,cMonster::eFamily>::const_iterator itr = m_MobsType2Family().find(a_Type);
	if (itr != m_MobsType2Family().end())
	{
		toReturn = itr->second;
	}
	return toReturn;
}
