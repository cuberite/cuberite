// BlockID.cpp

// Implements the helper functions for converting Block ID string to int etc.

#include "Globals.h"
#include "BlockID.h"
#include "inifile/iniFile.h"
#include "Item.h"
#include "Mobs/Monster.h"





class cBlockIDMap
{
	// Making the map case-insensitive:
	struct Comparator
	{
		bool operator()(const AString & a_Item1, const AString & a_Item2) const
		{
			return (NoCaseCompare(a_Item1, a_Item2) > 0);
		}
	} ;
	
	typedef std::map<AString, std::pair<short, short>, Comparator> ItemMap;
	
public:
	cBlockIDMap(void)
	{
		cIniFile Ini;
		if (!Ini.ReadFile("items.ini"))
		{
			return;
		}
		int KeyID = Ini.FindKey("Items");
		if (KeyID == cIniFile::noID)
		{
			return;
		}
		int NumValues = Ini.GetNumValues(KeyID);
		for (int i = 0; i < NumValues; i++)
		{
			AString Name = Ini.GetValueName(KeyID, i);
			if (Name.empty())
			{
				continue;
			}
			AString Value = Ini.GetValue(KeyID, i);
			AddToMap(Name, Value);
		}  // for i - Ini.Values[]
	}
	
	
	int Resolve(const AString & a_ItemName)
	{
		ItemMap::iterator itr = m_Map.find(a_ItemName);
		if (itr == m_Map.end())
		{
			return -1;
		}
		return itr->second.first;
	}
	
	
	bool ResolveItem(const AString & a_ItemName, cItem & a_Item)
	{
		// Split into parts divided by either ':' or '^'
		AStringVector Split = StringSplitAndTrim(a_ItemName, ":^");
		if (Split.empty())
		{
			return false;
		}

		ItemMap::iterator itr = m_Map.find(Split[0]);
		if (itr != m_Map.end())
		{
			// Resolved as a string, assign the type and the default damage / count
			a_Item.m_ItemType = itr->second.first;
			a_Item.m_ItemDamage = itr->second.second;
			if (a_Item.m_ItemDamage == -1)
			{
				a_Item.m_ItemDamage = 0;
			}
		}
		else
		{
			// Not a resolvable string, try pure numbers: "45:6", "45^6" etc.
			a_Item.m_ItemType = (short)atoi(Split[0].c_str());
			if ((a_Item.m_ItemType == 0) && (Split[0] != "0"))
			{
				// Parsing the number failed
				return false;
			}
		}
		
		// Parse the damage, if present:
		if (Split.size() < 2)
		{
			// Not present, set the item as valid and return success:
			a_Item.m_ItemCount = 1;
			return true;
		}
		
		a_Item.m_ItemDamage = (short)atoi(Split[1].c_str());
		if ((a_Item.m_ItemDamage == 0) && (Split[1] != "0"))
		{
			// Parsing the number failed
			return false;
		}
		a_Item.m_ItemCount = 1;
		return true;
	}
	
	
	AString Desolve(short a_ItemType, short a_ItemDamage)
	{
		// First try an exact match, both ItemType and ItemDamage ("birchplanks=5:2"):
		for (ItemMap::iterator itr = m_Map.begin(), end = m_Map.end(); itr != end; ++itr)
		{
			if ((itr->second.first == a_ItemType) && (itr->second.second == a_ItemDamage))
			{
				return itr->first;
			}
		}  // for itr - m_Map[]
		
		// There is no exact match, try matching ItemType only ("planks=5"):
		if (a_ItemDamage == 0)
		{
			for (ItemMap::iterator itr = m_Map.begin(), end = m_Map.end(); itr != end; ++itr)
			{
				if ((itr->second.first == a_ItemType) && (itr->second.second == -1))
				{
					return itr->first;
				}
			}  // for itr - m_Map[]
		}

		// No match at all, synthesize a string ("5:1"):
		AString res;
		if (a_ItemDamage == -1)
		{
			Printf(res, "%d", a_ItemType);
		}
		else
		{
			Printf(res, "%d:%d", a_ItemType, a_ItemDamage);
		}
		return res;
	}
	
	
protected:
	ItemMap m_Map;
	
	
	void AddToMap(const AString & a_Name, const AString & a_Value)
	{
		AStringVector Split = StringSplit(a_Value, ":");
		if (Split.size() == 1)
		{
			Split = StringSplit(a_Value, "^");
		}
		if (Split.empty())
		{
			return;
		}
		short ItemType = (short)atoi(Split[0].c_str());
		short ItemDamage = (Split.size() > 1) ? (short)atoi(Split[1].c_str()) : -1;
		m_Map[a_Name] = std::make_pair(ItemType, ItemDamage);
	}
} ;





static cBlockIDMap gsBlockIDMap;





/*
// Quick self-test:
class Tester
{
public:
	Tester(void)
	{
		cItem Item;
		gsBlockIDMap.ResolveItem("charcoal", Item);
		AString Charcoal = gsBlockIDMap.Desolve(Item.m_ItemType, Item.m_ItemDamage);
		ASSERT(Charcoal == "charcoal");
	}
} test;
//*/





BLOCKTYPE BlockStringToType(const AString & a_BlockTypeString)
{
	int res = atoi(a_BlockTypeString.c_str());
	if ((res != 0) || (a_BlockTypeString.compare("0") == 0))
	{
		// It was a valid number, return that
		return res;
	}
	
	return gsBlockIDMap.Resolve(TrimString(a_BlockTypeString));
}




bool StringToItem(const AString & a_ItemTypeString, cItem & a_Item)
{
	return gsBlockIDMap.ResolveItem(TrimString(a_ItemTypeString), a_Item);
}





AString ItemToString(const cItem & a_Item)
{
	return gsBlockIDMap.Desolve(a_Item.m_ItemType, a_Item.m_ItemDamage);
}





AString ItemTypeToString(short a_ItemType)
{
	return gsBlockIDMap.Desolve(a_ItemType, -1);
}





AString ItemToFullString(const cItem & a_Item)
{
	AString res;
	Printf(res, "%s:%d * %d", ItemToString(a_Item).c_str(), a_Item.m_ItemDamage, a_Item.m_ItemCount);
	return res;
}





int StringToMobType(const AString & a_MobString)
{
	static struct
	{
		int m_MobType;
		const char * m_String;
	} MobMap [] =
	{
		{cMonster::mtCreeper,      "Creeper"},
		{cMonster::mtSkeleton,     "Skeleton"},
		{cMonster::mtSpider,       "Spider"},
		{cMonster::mtGiant,        "Giant"},
		{cMonster::mtZombie,       "Zombie"},
		{cMonster::mtSlime,        "Slime"},
		{cMonster::mtGhast,        "Ghast"},
		{cMonster::mtZombiePigman, "ZombiePigman"},
		{cMonster::mtEnderman,     "Enderman"},
		{cMonster::mtCaveSpider,   "CaveSpider"},
		{cMonster::mtSilverfish,   "SilverFish"},
		{cMonster::mtBlaze,        "Blaze"},
		{cMonster::mtMagmaCube,    "MagmaCube"},
		{cMonster::mtEnderDragon,  "EnderDragon"},
		{cMonster::mtWither,       "Wither"},
		{cMonster::mtBat,          "Bat"},
		{cMonster::mtWitch,        "Witch"},
		{cMonster::mtPig,          "Pig"},
		{cMonster::mtSheep,        "Sheep"},
		{cMonster::mtCow,          "Cow"},
		{cMonster::mtChicken,      "Chicken"},
		{cMonster::mtSquid,        "Squid"},
		{cMonster::mtWolf,         "Wolf"},
		{cMonster::mtMooshroom,    "Mooshroom"},
		{cMonster::mtSnowGolem,    "SnowGolem"},
		{cMonster::mtOcelot,       "Ocelot"},
		{cMonster::mtIronGolem,    "IronGolem"},
		{cMonster::mtVillager,     "Villager"},
	};
	for (size_t i = 0; i < ARRAYCOUNT(MobMap); i++)
	{
		if (NoCaseCompare(MobMap[i].m_String, a_MobString) == 0)
		{
			return MobMap[i].m_MobType;
		}
	}  // for i - MobMap[]
	return -1;
}





eDimension StringToDimension(const AString & a_DimensionString)
{
	// First try decoding as a number
	int res = atoi(a_DimensionString.c_str());
	if ((res != 0) || (a_DimensionString == "0"))
	{
		// It was a valid number
		return (eDimension)res;
	}
	
	// Decode using a built-in map:
	static struct
	{
		eDimension m_Dimension;
		const char * m_String;
	} DimensionMap [] =
	{
		{ dimOverworld, "Overworld"},
		{ dimOverworld, "Normal"},
		{ dimOverworld, "World"},
		{ dimNether,    "Nether"},
		{ dimNether,    "Hell"},  // Alternate name for Nether
		{ dimEnd,       "End"},
		{ dimEnd,       "Sky"},  // Old name for End
	} ;
	for (size_t i = 0; i < ARRAYCOUNT(DimensionMap); i++)
	{
		if (NoCaseCompare(DimensionMap[i].m_String, a_DimensionString) == 0)
		{
			return DimensionMap[i].m_Dimension;
		}
	}  // for i - DimensionMap[]
	
	// Not found
	LOGWARNING("Unknown dimension: \"%s\". Setting to Overworld", a_DimensionString.c_str());
	return dimOverworld;
}





/// Translates damage type constant to a string representation (built-in).
AString DamageTypeToString(eDamageType a_DamageType)
{
	// Make sure to keep this alpha-sorted.
	switch (a_DamageType)
	{
		case dtAdmin:           return "dtAdmin";
		case dtAttack:          return "dtAttack";
		case dtCactusContact:   return "dtCactusContact";
		case dtDrowning:        return "dtDrowning";
		case dtEnderPearl:      return "dtEnderPearl";
		case dtFalling:         return "dtFalling";
		case dtFireContact:     return "dtFireContact";
		case dtInVoid:          return "dtInVoid";
		case dtLavaContact:     return "dtLavaContact";
		case dtLightning:       return "dtLightning";
		case dtOnFire:          return "dtOnFire";
		case dtPoisoning:       return "dtPoisoning";
		case dtWithering:       return "dtWithering";
		case dtPotionOfHarming: return "dtPotionOfHarming";
		case dtRangedAttack:    return "dtRangedAttack";
		case dtStarving:        return "dtStarving";
		case dtSuffocating:     return "dtSuffocation";
		case dtExplosion:       return "dtExplosion";
	}
	
	// Unknown damage type:
	ASSERT(!"Unknown DamageType");
	return Printf("dtUnknown_%d", (int)a_DamageType);
}





/// Translates a damage type string to damage type. Takes either a number or a damage type alias (built-in). Returns -1 on failure
eDamageType StringToDamageType(const AString & a_DamageTypeString)
{
	// First try decoding as a number:
	int res = atoi(a_DamageTypeString.c_str());
	if ((res != 0) || (a_DamageTypeString == "0"))
	{
		// It was a valid number
		return (eDamageType)res;
	}
	
	// Decode using a built-in map:
	static struct
	{
		eDamageType  m_DamageType;
		const char * m_String;
	} DamageTypeMap [] =
	{
		// Cannonical names:
		{ dtAttack,          "dtAttack"},
		{ dtRangedAttack,    "dtRangedAttack"},
		{ dtLightning,       "dtLightning"},
		{ dtFalling,         "dtFalling"},
		{ dtDrowning,        "dtDrowning"},
		{ dtSuffocating,     "dtSuffocation"},
		{ dtStarving,        "dtStarving"},
		{ dtCactusContact,   "dtCactusContact"},
		{ dtLavaContact,     "dtLavaContact"},
		{ dtPoisoning,       "dtPoisoning"},
		{ dtWithering,       "dtWithering"},
		{ dtOnFire,          "dtOnFire"},
		{ dtFireContact,     "dtFireContact"},
		{ dtInVoid,          "dtInVoid"},
		{ dtPotionOfHarming, "dtPotionOfHarming"},
		{ dtAdmin,           "dtAdmin"},
		{ dtExplosion,       "dtExplosion"},

		// Common synonyms:
		{ dtAttack,        "dtPawnAttack"},
		{ dtAttack,        "dtEntityAttack"},
		{ dtAttack,        "dtMob"},
		{ dtAttack,        "dtMobAttack"},
		{ dtRangedAttack,  "dtArrowAttack"},
		{ dtRangedAttack,  "dtArrow"},
		{ dtRangedAttack,  "dtProjectile"},
		{ dtFalling,       "dtFall"},
		{ dtDrowning,      "dtDrown"},
		{ dtSuffocating,   "dtSuffocation"},
		{ dtStarving,      "dtStarvation"},
		{ dtStarving,      "dtHunger"},
		{ dtCactusContact, "dtCactus"},
		{ dtCactusContact, "dtCactuses"},
		{ dtCactusContact, "dtCacti"},
		{ dtLavaContact,   "dtLava"},
		{ dtPoisoning,     "dtPoison"},
		{ dtWithering,     "dtWither"},
		{ dtOnFire,        "dtBurning"},
		{ dtFireContact,   "dtInFire"},
		{ dtAdmin,         "dtPlugin"},
	} ;
	for (size_t i = 0; i < ARRAYCOUNT(DamageTypeMap); i++)
	{
		if (NoCaseCompare(DamageTypeMap[i].m_String, a_DamageTypeString) == 0)
		{
			return DamageTypeMap[i].m_DamageType;
		}
	}  // for i - DamageTypeMap[]
	
	// Not found:
	return (eDamageType)-1;
}





cItem GetIniItemSet(cIniFile & a_IniFile, const char * a_Section, const char * a_Key, const char * a_Default)
{
	AString ItemStr = a_IniFile.GetValueSet(a_Section, a_Key, a_Default);
	cItem res;
	if (!StringToItem(ItemStr, res))
	{
		res.Empty();
	}
	return res;
}






