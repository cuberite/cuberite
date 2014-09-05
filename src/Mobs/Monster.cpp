
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "Monster.h"
#include "IncludeAllMonsters.h"
#include "../ClientHandle.h"
#include "../MersenneTwister.h"
#include "../FastRandom.h"





/** Map for eType <-> string
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
	{cMonster::mtEnderDragon,  "enderdragon"},
	{cMonster::mtGhast,        "ghast"},
	{cMonster::mtHorse,        "horse"},
	{cMonster::mtIronGolem,    "irongolem"},
	{cMonster::mtMagmaCube,    "magmacube"},
	{cMonster::mtMooshroom,    "mooshroom"},
	{cMonster::mtOcelot,       "ocelot"},
	{cMonster::mtPig,          "pig"},
	{cMonster::mtSheep,        "sheep"},
	{cMonster::mtSilverfish,   "silverfish"},
	{cMonster::mtSkeleton,     "skeleton"},
	{cMonster::mtSlime,        "slime"},
	{cMonster::mtSnowGolem,    "snowgolem"},
	{cMonster::mtSpider,       "spider"},
	{cMonster::mtSquid,        "squid"},
	{cMonster::mtVillager,     "villager"},
	{cMonster::mtWitch,        "witch"},
	{cMonster::mtWither,       "wither"},
	{cMonster::mtWolf,         "wolf"},
	{cMonster::mtZombie,       "zombie"},
	{cMonster::mtZombiePigman, "zombiepigman"},
} ;





cMonster::cMonster(const AString & a_ConfigName, eType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height)
	: super(etMonster, a_Width, a_Height)
	, m_MobType(a_MobType)
	, m_SoundHurt(a_SoundHurt)
	, m_SoundDeath(a_SoundDeath)
	, m_DestroyTimer(0.0f)
{
	m_AI = new cAIAggressiveComponent(this);
	m_Attack = new cAttackComponent(this);
	m_Environment = new cEnvironmentComponent(this, 16);
	m_Movement = new cMovementComponent(this);

	// Temporary placement till I figure out where to put it
	m_DropChanceWeapon = 0.0f;
	m_DropChanceHelmet = 0.0f;
	m_DropChanceChestplate = 0.0f;
	m_DropChanceLeggings = 0.0f;
	m_DropChanceBoots = 0.0f;
}

void cMonster::Tick(float a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);

	if (m_Health <= 0)
	{
		// The mob is dead, but we're still animating the "puff" they leave when they die
		m_DestroyTimer += a_Dt / 1000;
		if (m_DestroyTimer > 1)
		{
			Destroy(true);
		}
		return;
	}

	LOG("Monster Tick...");
	m_AI->Tick(a_Dt, a_Chunk);
	m_Attack->Tick(a_Dt, a_Chunk);
	m_Environment->Tick(a_Dt, a_Chunk);
	m_Movement->Tick(a_Dt, a_Chunk);
}





void cMonster::SpawnOn(cClientHandle & a_Client)
{
	a_Client.SendSpawnMob(*this);
}




void cMonster::AddRandomDropItem(cItems & a_Drops, unsigned int a_Min, unsigned int a_Max, short a_Item, short a_ItemHealth)
{
	MTRand r1;
	int Count = r1.randInt() % (a_Max + 1 - a_Min) + a_Min;
	if (Count > 0)
	{
		a_Drops.push_back(cItem(a_Item, Count, a_ItemHealth));
	}
}





void cMonster::AddRandomUncommonDropItem(cItems & a_Drops, float a_Chance, short a_Item, short a_ItemHealth)
{
	MTRand r1;
	int Count = r1.randInt() % 1000;
	if (Count < (a_Chance * 10))
	{
		a_Drops.push_back(cItem(a_Item, 1, a_ItemHealth));
	}
}


void cMonster::AddRandomRareDropItem(cItems & a_Drops, cItems & a_Items, short a_LootingLevel)
{
	MTRand r1;
	int Count = r1.randInt() % 200;
	if (Count < (5 + a_LootingLevel))
	{
		int Rare = r1.randInt() % a_Items.Size();
		a_Drops.push_back(a_Items.at(Rare));
	}
}





void cMonster::AddRandomArmorDropItem(cItems & a_Drops, short a_LootingLevel)
{
	MTRand r1;
	if (r1.randInt() % 200 < ((m_DropChanceHelmet * 200) + (a_LootingLevel * 2)))
	{
		if (!GetEquippedHelmet().IsEmpty()) a_Drops.push_back(GetEquippedHelmet());
	}
	
	if (r1.randInt() % 200 < ((m_DropChanceChestplate * 200) + (a_LootingLevel * 2)))
	{
		if (!GetEquippedChestplate().IsEmpty()) a_Drops.push_back(GetEquippedChestplate());
	}
	
	if (r1.randInt() % 200 < ((m_DropChanceLeggings * 200) + (a_LootingLevel * 2)))
	{
		if (!GetEquippedLeggings().IsEmpty()) a_Drops.push_back(GetEquippedLeggings());
	}
	
	if (r1.randInt() % 200 < ((m_DropChanceBoots * 200) + (a_LootingLevel * 2)))
	{
		if (!GetEquippedBoots().IsEmpty()) a_Drops.push_back(GetEquippedBoots());
	}
}



void cMonster::AddRandomWeaponDropItem(cItems & a_Drops, short a_LootingLevel)
{
	MTRand r1;
	if (r1.randInt() % 200 < ((m_DropChanceWeapon * 200) + (a_LootingLevel * 2)))
	{
		if (!GetEquippedWeapon().IsEmpty()) a_Drops.push_back(GetEquippedWeapon());
	}
}






cMonster::eFamily cMonster::FamilyFromType(eType a_Type)
{
	// Passive-agressive mobs are counted in mob spawning code as passive

	switch (a_Type)
	{
		case mtBat:          return mfAmbient;
		case mtBlaze:        return mfHostile;
		case mtCaveSpider:   return mfHostile;
		case mtChicken:      return mfPassive;
		case mtCow:          return mfPassive;
		case mtCreeper:      return mfHostile;
		case mtEnderDragon:  return mfNoSpawn;
		case mtEnderman:     return mfHostile;
		case mtGhast:        return mfHostile;
		case mtGiant:        return mfNoSpawn;
		case mtHorse:        return mfPassive;
		case mtIronGolem:    return mfPassive;
		case mtMagmaCube:    return mfHostile;
		case mtMooshroom:    return mfHostile;
		case mtOcelot:       return mfPassive;
		case mtPig:          return mfPassive;
		case mtSheep:        return mfPassive;
		case mtSilverfish:   return mfHostile;
		case mtSkeleton:     return mfHostile;
		case mtSlime:        return mfHostile;
		case mtSnowGolem:    return mfNoSpawn;
		case mtSpider:       return mfHostile;
		case mtSquid:        return mfWater;
		case mtVillager:     return mfPassive;
		case mtWitch:        return mfHostile;
		case mtWither:       return mfNoSpawn;
		case mtWolf:         return mfHostile;
		case mtZombie:       return mfHostile;
		case mtZombiePigman: return mfHostile;
			
		case mtInvalidType:  break;
	}
	ASSERT(!"Unhandled mob type");
	return mfUnhandled;
}





cMonster * cMonster::NewMonsterFromType(cMonster::eType a_MobType)
{
	cFastRandom Random;
	cMonster * toReturn = NULL;

	// Create the mob entity
	switch (a_MobType)
	{
		case mtMagmaCube:
		{
			toReturn = new cMagmaCube(Random.NextInt(2) + 1);
			break;
		}
		case mtSlime:
		{
			toReturn = new cSlime(1 << Random.NextInt(3));  // Size 1, 2 or 4
			break;
		}
		case mtSkeleton:
		{
			// TODO: Actual detection of spawning in Nether
			toReturn = new cSkeleton((Random.NextInt(1) == 0) ? false : true);
			break;
		}
		case mtVillager:
		{
			int VillagerType = Random.NextInt(6);
			if (VillagerType == 6)
			{
				// Give farmers a better chance of spawning
				VillagerType = 0;
			}

			toReturn = new cVillager((cVillager::eVillagerType)VillagerType);
			break;
		}
		case mtHorse:
		{
			// Horses take a type (species), a colour, and a style (dots, stripes, etc.)
			int HorseType = Random.NextInt(7);
			int HorseColor = Random.NextInt(6);
			int HorseStyle = Random.NextInt(6);
			int HorseTameTimes = Random.NextInt(6) + 1;

			if ((HorseType == 5) || (HorseType == 6) || (HorseType == 7))
			{
				// Increase chances of normal horse (zero)
				HorseType = 0;
			}

			toReturn = new cHorse(HorseType, HorseColor, HorseStyle, HorseTameTimes);
			break;
		}

		case mtBat:           toReturn = new cBat();                      break;
		case mtBlaze:         toReturn = new cBlaze();                    break;
		case mtCaveSpider:    toReturn = new cCaveSpider();               break;
		case mtChicken:       toReturn = new cChicken();                  break;
		case mtCow:           toReturn = new cCow();                      break;
		case mtCreeper:       toReturn = new cCreeper();                  break;
		case mtEnderDragon:   toReturn = new cEnderDragon();              break;
		case mtEnderman:      toReturn = new cEnderman();                 break;
		case mtGhast:         toReturn = new cGhast();                    break;
		case mtGiant:         toReturn = new cGiant();                    break;
		case mtIronGolem:     toReturn = new cIronGolem();                break;
		case mtMooshroom:     toReturn = new cMooshroom();                break;
		case mtOcelot:        toReturn = new cOcelot();                   break;
		case mtPig:           toReturn = new cPig();                      break;
		case mtSheep:         toReturn = new cSheep();                    break;
		case mtSilverfish:    toReturn = new cSilverfish();               break;
		case mtSnowGolem:     toReturn = new cSnowGolem();                break;
		case mtSpider:        toReturn = new cSpider();                   break;
		case mtSquid:         toReturn = new cSquid();                    break;
		case mtWitch:         toReturn = new cWitch();                    break;
		case mtWither:	      toReturn = new cWither();                   break;
		case mtWolf:          toReturn = new cWolf();                     break;
		case mtZombie:        toReturn = new cZombie(false);              break;  // TODO: Infected zombie parameter
		case mtZombiePigman:  toReturn = new cZombiePigman();             break;
		default:
		{
			ASSERT(!"Unhandled mob type whilst trying to spawn mob!");
		}
	}
	return toReturn;
}





int cMonster::GetSpawnDelay(cMonster::eFamily a_MobFamily)
{
	switch (a_MobFamily)
	{
		case mfHostile:   return 40;
		case mfPassive:   return 40;
		case mfAmbient:   return 40;
		case mfWater:     return 400;
		case mfNoSpawn:   return -1;
		case mfUnhandled: break;
	}
	ASSERT(!"Unhandled mob family");
	return -1;
}





AString cMonster::MobTypeToString(cMonster::eType a_MobType)
{
	// Mob types aren't sorted, so we need to search linearly:
	for (size_t i = 0; i < ARRAYCOUNT(g_MobTypeNames); i++)
	{
		if (g_MobTypeNames[i].m_Type == a_MobType)
		{
			return g_MobTypeNames[i].m_lcName;
		}
	}
	
	// Not found:
	return "";
}





cMonster::eType cMonster::StringToMobType(const AString & a_Name)
{
	AString lcName = StrToLower(a_Name);
	
	// Binary-search for the lowercase name:
	int lo = 0, hi = ARRAYCOUNT(g_MobTypeNames) - 1;
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
			lo = mid;
		}
		else
		{
			hi = mid;
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
	return mtInvalidType;
}

