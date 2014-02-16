// BlockID.cpp

// Implements the helper functions for converting Block ID string to int etc.

#include "Globals.h"
#include "BlockID.h"
#include "inifile/iniFile.h"
#include "Item.h"
#include "Mobs/Monster.h"





NIBBLETYPE g_BlockLightValue[256];
NIBBLETYPE g_BlockSpreadLightFalloff[256];
bool       g_BlockTransparent[256];
float      g_BlockDigTime[256];
bool       g_BlockPistonBreakable[256];
bool       g_BlockIsSnowable[256];
bool       g_BlockRequiresSpecialTool[256];
bool       g_BlockIsSolid[256];
bool       g_BlockFullyOccupiesVoxel[256];





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
		
		a_Item.m_ItemDamage = atoi(Split[1].c_str());
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
	static struct {
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
		{ dimNether,    "Hell"},  // Alternate name for End
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
	return (eDimension)-1000;
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





// This is actually just some code that needs to run at program startup, so it is wrapped into a global var's constructor:
class cBlockPropertiesInitializer
{
public:
	cBlockPropertiesInitializer(void)
	{
		memset(g_BlockLightValue,          0x00, sizeof(g_BlockLightValue));
		memset(g_BlockSpreadLightFalloff,  0x0f, sizeof(g_BlockSpreadLightFalloff)); // 0x0f means total falloff
		memset(g_BlockTransparent,         0x00, sizeof(g_BlockTransparent));
		memset(g_BlockPistonBreakable,     0x00, sizeof(g_BlockPistonBreakable));
		memset(g_BlockFullyOccupiesVoxel,  0x00, sizeof(g_BlockFullyOccupiesVoxel));
		
		std::fill(g_BlockDigTime, g_BlockDigTime + ARRAYCOUNT(g_BlockDigTime), 0);
		std::fill(g_BlockIsSnowable, g_BlockIsSnowable + ARRAYCOUNT(g_BlockIsSnowable), true);
		std::fill(g_BlockIsSolid, g_BlockIsSolid + ARRAYCOUNT(g_BlockIsSolid), true);
		
		memset(g_BlockRequiresSpecialTool, 0x00, sizeof(g_BlockRequiresSpecialTool));  // Set all blocks to false

		// Emissive blocks
		g_BlockLightValue[E_BLOCK_FIRE]                 = 15;
		g_BlockLightValue[E_BLOCK_GLOWSTONE]            = 15;
		g_BlockLightValue[E_BLOCK_JACK_O_LANTERN]       = 15;
		g_BlockLightValue[E_BLOCK_LAVA]                 = 15;
		g_BlockLightValue[E_BLOCK_STATIONARY_LAVA]      = 15;
		g_BlockLightValue[E_BLOCK_END_PORTAL]           = 15;
		g_BlockLightValue[E_BLOCK_REDSTONE_LAMP_ON]     = 15;
		g_BlockLightValue[E_BLOCK_TORCH]                = 14;
		g_BlockLightValue[E_BLOCK_BURNING_FURNACE]      = 13;
		g_BlockLightValue[E_BLOCK_NETHER_PORTAL]        = 11;
		g_BlockLightValue[E_BLOCK_REDSTONE_ORE_GLOWING] = 9;
		g_BlockLightValue[E_BLOCK_REDSTONE_REPEATER_ON] = 9;
		g_BlockLightValue[E_BLOCK_REDSTONE_TORCH_ON]    = 7;
		g_BlockLightValue[E_BLOCK_BREWING_STAND]        = 1;
		g_BlockLightValue[E_BLOCK_BROWN_MUSHROOM]       = 1;
		g_BlockLightValue[E_BLOCK_DRAGON_EGG]           = 1;

		// Spread blocks
		g_BlockSpreadLightFalloff[E_BLOCK_AIR]              = 1;
		g_BlockSpreadLightFalloff[E_BLOCK_CAKE]             = 1;
		g_BlockSpreadLightFalloff[E_BLOCK_CHEST]            = 1;
		g_BlockSpreadLightFalloff[E_BLOCK_COBWEB]           = 1;
		g_BlockSpreadLightFalloff[E_BLOCK_CROPS]            = 1;
		g_BlockSpreadLightFalloff[E_BLOCK_FENCE]            = 1;
		g_BlockSpreadLightFalloff[E_BLOCK_FENCE_GATE]       = 1;
		g_BlockSpreadLightFalloff[E_BLOCK_FIRE]             = 1;
		g_BlockSpreadLightFalloff[E_BLOCK_GLASS]            = 1;
		g_BlockSpreadLightFalloff[E_BLOCK_GLASS_PANE]       = 1;
		g_BlockSpreadLightFalloff[E_BLOCK_GLOWSTONE]        = 1;
		g_BlockSpreadLightFalloff[E_BLOCK_IRON_BARS]        = 1;
		g_BlockSpreadLightFalloff[E_BLOCK_IRON_DOOR]        = 1;
		g_BlockSpreadLightFalloff[E_BLOCK_LEAVES]           = 1;
		g_BlockSpreadLightFalloff[E_BLOCK_SIGN_POST]        = 1;
		g_BlockSpreadLightFalloff[E_BLOCK_TORCH]            = 1;
		g_BlockSpreadLightFalloff[E_BLOCK_VINES]            = 1;
		g_BlockSpreadLightFalloff[E_BLOCK_WALLSIGN]         = 1;
		g_BlockSpreadLightFalloff[E_BLOCK_WOODEN_DOOR]      = 1;

		// Light in water and lava dissapears faster:
		g_BlockSpreadLightFalloff[E_BLOCK_LAVA]             = 3;
		g_BlockSpreadLightFalloff[E_BLOCK_STATIONARY_LAVA]  = 3;
		g_BlockSpreadLightFalloff[E_BLOCK_STATIONARY_WATER] = 3;
		g_BlockSpreadLightFalloff[E_BLOCK_WATER]            = 3;

		// Transparent blocks
		g_BlockTransparent[E_BLOCK_ACTIVATOR_RAIL]        = true;
		g_BlockTransparent[E_BLOCK_AIR]                   = true;
		g_BlockTransparent[E_BLOCK_BIG_FLOWER]            = true;
		g_BlockTransparent[E_BLOCK_BROWN_MUSHROOM]        = true;
		g_BlockTransparent[E_BLOCK_CARROTS]               = true;
		g_BlockTransparent[E_BLOCK_CHEST]                 = true;
		g_BlockTransparent[E_BLOCK_COBWEB]                = true;
		g_BlockTransparent[E_BLOCK_CROPS]                 = true;
		g_BlockTransparent[E_BLOCK_DANDELION]             = true;
		g_BlockTransparent[E_BLOCK_DETECTOR_RAIL]         = true;
		g_BlockTransparent[E_BLOCK_ENDER_CHEST]           = true;
		g_BlockTransparent[E_BLOCK_FENCE]                 = true;
		g_BlockTransparent[E_BLOCK_FENCE_GATE]            = true;
		g_BlockTransparent[E_BLOCK_FIRE]                  = true;
		g_BlockTransparent[E_BLOCK_FLOWER]                = true;
		g_BlockTransparent[E_BLOCK_FLOWER_POT]            = true;
		g_BlockTransparent[E_BLOCK_GLASS]                 = true;
		g_BlockTransparent[E_BLOCK_GLASS_PANE]            = true;
		g_BlockTransparent[E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE] = true;
		g_BlockTransparent[E_BLOCK_ICE]                   = true;
		g_BlockTransparent[E_BLOCK_IRON_DOOR]             = true;
		g_BlockTransparent[E_BLOCK_LAVA]                  = true;
		g_BlockTransparent[E_BLOCK_LEAVES]                = true;
		g_BlockTransparent[E_BLOCK_LEVER]                 = true;
		g_BlockTransparent[E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE] = true;
		g_BlockTransparent[E_BLOCK_MELON_STEM]            = true;
		g_BlockTransparent[E_BLOCK_NETHER_BRICK_FENCE]    = true;
		g_BlockTransparent[E_BLOCK_NEW_LEAVES]            = true;
		g_BlockTransparent[E_BLOCK_POTATOES]              = true;
		g_BlockTransparent[E_BLOCK_POWERED_RAIL]          = true;
		g_BlockTransparent[E_BLOCK_PISTON_EXTENSION]      = true;
		g_BlockTransparent[E_BLOCK_PUMPKIN_STEM]          = true;
		g_BlockTransparent[E_BLOCK_RAIL]                  = true;
		g_BlockTransparent[E_BLOCK_RED_MUSHROOM]          = true;
		g_BlockTransparent[E_BLOCK_SIGN_POST]             = true;
		g_BlockTransparent[E_BLOCK_SNOW]                  = true;
		g_BlockTransparent[E_BLOCK_STAINED_GLASS]         = true;
		g_BlockTransparent[E_BLOCK_STAINED_GLASS_PANE]    = true;
		g_BlockTransparent[E_BLOCK_STATIONARY_LAVA]       = true;
		g_BlockTransparent[E_BLOCK_STATIONARY_WATER]      = true;
		g_BlockTransparent[E_BLOCK_STONE_BUTTON]          = true;
		g_BlockTransparent[E_BLOCK_STONE_PRESSURE_PLATE]  = true;
		g_BlockTransparent[E_BLOCK_TALL_GRASS]            = true;
		g_BlockTransparent[E_BLOCK_TORCH]                 = true;
		g_BlockTransparent[E_BLOCK_VINES]                 = true;
		g_BlockTransparent[E_BLOCK_WALLSIGN]              = true;
		g_BlockTransparent[E_BLOCK_WATER]                 = true;
		g_BlockTransparent[E_BLOCK_WOODEN_BUTTON]         = true;
		g_BlockTransparent[E_BLOCK_WOODEN_DOOR]           = true;
		g_BlockTransparent[E_BLOCK_WOODEN_PRESSURE_PLATE] = true;

		// TODO: Any other transparent blocks?
		
		// Blocks Dig Time
		g_BlockDigTime[E_BLOCK_STONE]                   = 1.5F;
		g_BlockDigTime[E_BLOCK_GRASS]                   = 0.6F;
		g_BlockDigTime[E_BLOCK_DIRT]                    = 0.5F;
		g_BlockDigTime[E_BLOCK_COBBLESTONE]             = 2.0F;
		g_BlockDigTime[E_BLOCK_PLANKS]                  = 2.0F;
		g_BlockDigTime[E_BLOCK_SAPLING]                 = 0.0F;
		g_BlockDigTime[E_BLOCK_WATER]                   = 100.0F;
		g_BlockDigTime[E_BLOCK_STATIONARY_WATER]        = 100.0F;
		g_BlockDigTime[E_BLOCK_LAVA]                    = 100.0F;
		g_BlockDigTime[E_BLOCK_STATIONARY_LAVA]         = 100.0F;
		g_BlockDigTime[E_BLOCK_SAND]                    = 0.5F;
		g_BlockDigTime[E_BLOCK_GRAVEL]                  = 0.6F;
		g_BlockDigTime[E_BLOCK_GOLD_ORE]                = 3.0F;
		g_BlockDigTime[E_BLOCK_IRON_ORE]                = 3.0F;
		g_BlockDigTime[E_BLOCK_COAL_ORE]                = 3.0F;
		g_BlockDigTime[E_BLOCK_LOG]                     = 2.0F;
		g_BlockDigTime[E_BLOCK_LEAVES]                  = 0.2F;
		g_BlockDigTime[E_BLOCK_SPONGE]                  = 0.6F;
		g_BlockDigTime[E_BLOCK_GLASS]                   = 0.3F;
		g_BlockDigTime[E_BLOCK_LAPIS_ORE]               = 3.0F;
		g_BlockDigTime[E_BLOCK_LAPIS_BLOCK]             = 3.0F;
		g_BlockDigTime[E_BLOCK_DISPENSER]               = 3.5F;
		g_BlockDigTime[E_BLOCK_SANDSTONE]               = 0.8F;
		g_BlockDigTime[E_BLOCK_NOTE_BLOCK]              = 0.8F;
		g_BlockDigTime[E_BLOCK_BED]                     = 0.2F;
		g_BlockDigTime[E_BLOCK_POWERED_RAIL]            = 0.7F;
		g_BlockDigTime[E_BLOCK_DETECTOR_RAIL]           = 0.7F;
		g_BlockDigTime[E_BLOCK_STICKY_PISTON]           = 0.5F;
		g_BlockDigTime[E_BLOCK_COBWEB]                  = 4.0F;
		g_BlockDigTime[E_BLOCK_TALL_GRASS]              = 0.0F;
		g_BlockDigTime[E_BLOCK_DEAD_BUSH]               = 0.0F;
		g_BlockDigTime[E_BLOCK_PISTON]                  = 0.5F;
		g_BlockDigTime[E_BLOCK_PISTON_EXTENSION]        = 0.5F;
		g_BlockDigTime[E_BLOCK_WOOL]                    = 0.8F;
		g_BlockDigTime[E_BLOCK_PISTON_MOVED_BLOCK]      = 100.0F;  // No Break
		g_BlockDigTime[E_BLOCK_DANDELION]               = 0.0F;
		g_BlockDigTime[E_BLOCK_FLOWER]                  = 0.0F;
		g_BlockDigTime[E_BLOCK_BROWN_MUSHROOM]          = 0.0F;
		g_BlockDigTime[E_BLOCK_RED_MUSHROOM]            = 0.0F;
		g_BlockDigTime[E_BLOCK_GOLD_BLOCK]              = 3.0F;
		g_BlockDigTime[E_BLOCK_IRON_BLOCK]              = 5.0F;
		g_BlockDigTime[E_BLOCK_DOUBLE_STONE_SLAB]       = 2.0F;
		g_BlockDigTime[E_BLOCK_STONE_SLAB]              = 2.0F;
		g_BlockDigTime[E_BLOCK_BRICK]                   = 2.0F;
		g_BlockDigTime[E_BLOCK_TNT]                     = 0.0F;
		g_BlockDigTime[E_BLOCK_BOOKCASE]                = 1.5F;
		g_BlockDigTime[E_BLOCK_MOSSY_COBBLESTONE]       = 2.0F;
		g_BlockDigTime[E_BLOCK_OBSIDIAN]                = 50.0F;
		g_BlockDigTime[E_BLOCK_TORCH]                   = 0.0F;
		g_BlockDigTime[E_BLOCK_FIRE]                    = 0.0F;
		g_BlockDigTime[E_BLOCK_MOB_SPAWNER]             = 5.0F;
		g_BlockDigTime[E_BLOCK_WOODEN_STAIRS]           = 2.0F;
		g_BlockDigTime[E_BLOCK_CHEST]                   = 2.5F;
		g_BlockDigTime[E_BLOCK_REDSTONE_WIRE]           = 0.0F;
		g_BlockDigTime[E_BLOCK_DIAMOND_ORE]             = 3.0F;
		g_BlockDigTime[E_BLOCK_DIAMOND_BLOCK]           = 5.0F;
		g_BlockDigTime[E_BLOCK_CRAFTING_TABLE]          = 2.5F;
		g_BlockDigTime[E_BLOCK_WORKBENCH]               = 2.5F;
		g_BlockDigTime[E_BLOCK_CROPS]                   = 0.0F;
		g_BlockDigTime[E_BLOCK_FARMLAND]                = 0.6F;
		g_BlockDigTime[E_BLOCK_FURNACE]                 = 3.5F;
		g_BlockDigTime[E_BLOCK_LIT_FURNACE]             = 3.5F;
		g_BlockDigTime[E_BLOCK_BURNING_FURNACE]         = 3.5F;
		g_BlockDigTime[E_BLOCK_SIGN_POST]               = 1.0F;
		g_BlockDigTime[E_BLOCK_WOODEN_DOOR]             = 3.0F;
		g_BlockDigTime[E_BLOCK_LADDER]                  = 0.4F;
		g_BlockDigTime[E_BLOCK_RAIL]                    = 0.7F;
		// g_BlockDigTime[E_BLOCK_MINECART_TRACKS]         = ...;
		g_BlockDigTime[E_BLOCK_COBBLESTONE_STAIRS]      = 2.0F;
		g_BlockDigTime[E_BLOCK_WALLSIGN]                = 1.0F;
		g_BlockDigTime[E_BLOCK_LEVER]                   = 0.5F;
		g_BlockDigTime[E_BLOCK_STONE_PRESSURE_PLATE]    = 0.5F;
		g_BlockDigTime[E_BLOCK_IRON_DOOR]               = 5.0F;
		g_BlockDigTime[E_BLOCK_WOODEN_PRESSURE_PLATE]   = 0.5F;
		g_BlockDigTime[E_BLOCK_REDSTONE_ORE]            = 3.0F;
		g_BlockDigTime[E_BLOCK_REDSTONE_ORE_GLOWING]    = 3.0F;
		g_BlockDigTime[E_BLOCK_REDSTONE_TORCH_OFF]      = 0.0F;
		g_BlockDigTime[E_BLOCK_REDSTONE_TORCH_ON]       = 0.0F;
		g_BlockDigTime[E_BLOCK_STONE_BUTTON]            = 0.5F;
		g_BlockDigTime[E_BLOCK_SNOW]                    = 0.1F;
		g_BlockDigTime[E_BLOCK_ICE]                     = 0.5F;
		g_BlockDigTime[E_BLOCK_SNOW_BLOCK]              = 0.2F;
		g_BlockDigTime[E_BLOCK_CACTUS]                  = 0.4F;
		g_BlockDigTime[E_BLOCK_CLAY]                    = 0.6F;
		// g_BlockDigTime[E_BLOCK_SUGARCANE]               = ...;
		g_BlockDigTime[E_BLOCK_REEDS]                   = 0.0F;
		g_BlockDigTime[E_BLOCK_JUKEBOX]                 = 2.0F;
		g_BlockDigTime[E_BLOCK_FENCE]                   = 2.0F;
		g_BlockDigTime[E_BLOCK_PUMPKIN]                 = 1.0F;
		g_BlockDigTime[E_BLOCK_NETHERRACK]              = 0.4F;
		g_BlockDigTime[E_BLOCK_SOULSAND]                = 0.5F;
		g_BlockDigTime[E_BLOCK_GLOWSTONE]               = 0.3F;
		g_BlockDigTime[E_BLOCK_NETHER_PORTAL]           = 100.0F;  // No Break
		g_BlockDigTime[E_BLOCK_JACK_O_LANTERN]          = 1.0F;
		g_BlockDigTime[E_BLOCK_CAKE]                    = 0.5F;
		g_BlockDigTime[E_BLOCK_REDSTONE_REPEATER_OFF]   = 0.0F;
		g_BlockDigTime[E_BLOCK_REDSTONE_REPEATER_ON]    = 0.0F;
		g_BlockDigTime[E_BLOCK_STAINED_GLASS]           = 0.3F;
		g_BlockDigTime[E_BLOCK_TRAPDOOR]                = 3.0F;
		g_BlockDigTime[E_BLOCK_SILVERFISH_EGG]          = 0.75F;
		g_BlockDigTime[E_BLOCK_STONE_BRICKS]            = 1.5F;
		g_BlockDigTime[E_BLOCK_HUGE_BROWN_MUSHROOM]     = 0.2F;
		g_BlockDigTime[E_BLOCK_HUGE_RED_MUSHROOM]       = 0.2F;
		g_BlockDigTime[E_BLOCK_IRON_BARS]               = 5.0F;
		g_BlockDigTime[E_BLOCK_GLASS_PANE]              = 0.3F;
		g_BlockDigTime[E_BLOCK_MELON]                   = 1.0F;
		g_BlockDigTime[E_BLOCK_PUMPKIN_STEM]            = 0.0F;
		g_BlockDigTime[E_BLOCK_MELON_STEM]              = 0.0F;
		g_BlockDigTime[E_BLOCK_VINES]                   = 0.1F;
		g_BlockDigTime[E_BLOCK_FENCE_GATE]              = 2.0F;
		g_BlockDigTime[E_BLOCK_BRICK_STAIRS]            = 2.0F;
		g_BlockDigTime[E_BLOCK_STONE_BRICK_STAIRS]      = 1.5F;
		g_BlockDigTime[E_BLOCK_MYCELIUM]                = 0.6F;
		g_BlockDigTime[E_BLOCK_LILY_PAD]                = 0.0F;
		g_BlockDigTime[E_BLOCK_NETHER_BRICK]            = 2.0F;
		g_BlockDigTime[E_BLOCK_NETHER_BRICK_FENCE]      = 2.0F;
		g_BlockDigTime[E_BLOCK_NETHER_BRICK_STAIRS]     = 2.0F;
		g_BlockDigTime[E_BLOCK_NETHER_WART]             = 0.0F;
		g_BlockDigTime[E_BLOCK_ENCHANTMENT_TABLE]       = 5.0F;
		g_BlockDigTime[E_BLOCK_BREWING_STAND]           = 0.5F;
		g_BlockDigTime[E_BLOCK_CAULDRON]                = 2.0F;
		g_BlockDigTime[E_BLOCK_END_PORTAL]              = 100.0F;  // No Break
		g_BlockDigTime[E_BLOCK_END_PORTAL_FRAME]        = 100.0F;  // No Break
		g_BlockDigTime[E_BLOCK_END_STONE]               = 3.0F;
		g_BlockDigTime[E_BLOCK_DRAGON_EGG]              = 3.0F;
		g_BlockDigTime[E_BLOCK_REDSTONE_LAMP_OFF]       = 0.3F;
		g_BlockDigTime[E_BLOCK_REDSTONE_LAMP_ON]        = 0.3F;
		g_BlockDigTime[E_BLOCK_DOUBLE_WOODEN_SLAB]      = 2.0F;
		g_BlockDigTime[E_BLOCK_WOODEN_SLAB]             = 2.0F;
		g_BlockDigTime[E_BLOCK_COCOA_POD]               = 0.2F;
		g_BlockDigTime[E_BLOCK_SANDSTONE_STAIRS]        = 0.8F;
		g_BlockDigTime[E_BLOCK_EMERALD_ORE]             = 3.0F;
		g_BlockDigTime[E_BLOCK_ENDER_CHEST]             = 22.5F;
		g_BlockDigTime[E_BLOCK_TRIPWIRE_HOOK]           = 0.0F;  // Is this correct?
		g_BlockDigTime[E_BLOCK_TRIPWIRE]                = 0.0F;  // Is this correct?
		g_BlockDigTime[E_BLOCK_EMERALD_BLOCK]           = 5.0F;
		g_BlockDigTime[E_BLOCK_SPRUCE_WOOD_STAIRS]      = 2.0F;
		g_BlockDigTime[E_BLOCK_BIRCH_WOOD_STAIRS]       = 2.0F;
		g_BlockDigTime[E_BLOCK_JUNGLE_WOOD_STAIRS]      = 2.0F;
		g_BlockDigTime[E_BLOCK_COMMAND_BLOCK]           = 100.0F;  // Is this correct? No Break
		g_BlockDigTime[E_BLOCK_BEACON]                  = 3.0F;
		g_BlockDigTime[E_BLOCK_COBBLESTONE_WALL]        = 2.0F;
		g_BlockDigTime[E_BLOCK_FLOWER_POT]              = 0.0F;
		g_BlockDigTime[E_BLOCK_CARROTS]                 = 0.0F;
		g_BlockDigTime[E_BLOCK_POTATOES]                = 0.0F;
		g_BlockDigTime[E_BLOCK_WOODEN_BUTTON]           = 0.5F;
		g_BlockDigTime[E_BLOCK_HEAD]                    = 1.0F;
		g_BlockDigTime[E_BLOCK_ANVIL]                   = 5.0F;
		g_BlockDigTime[E_BLOCK_TRAPPED_CHEST]           = 2.5F;
		g_BlockDigTime[E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE] = 0.5F;
		g_BlockDigTime[E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE] = 0.5F;
		g_BlockDigTime[E_BLOCK_INACTIVE_COMPARATOR]     = 0.0F;
		g_BlockDigTime[E_BLOCK_ACTIVE_COMPARATOR]       = 0.0F;
		g_BlockDigTime[E_BLOCK_DAYLIGHT_SENSOR]         = 0.2F;
		g_BlockDigTime[E_BLOCK_BLOCK_OF_REDSTONE]       = 5.0F;
		g_BlockDigTime[E_BLOCK_NETHER_QUARTZ_ORE]       = 3.0F;
		g_BlockDigTime[E_BLOCK_HOPPER]                  = 3.0F;
		g_BlockDigTime[E_BLOCK_QUARTZ_BLOCK]            = 0.8F;
		g_BlockDigTime[E_BLOCK_QUARTZ_STAIRS]           = 0.8F;
		g_BlockDigTime[E_BLOCK_ACTIVATOR_RAIL]          = 0.7F;
		g_BlockDigTime[E_BLOCK_DROPPER]                 = 3.5F;
		g_BlockDigTime[E_BLOCK_STAINED_CLAY]            = 1.25F;
		g_BlockDigTime[E_BLOCK_STAINED_GLASS_PANE]      = 0.3F;
		g_BlockDigTime[E_BLOCK_NEW_LEAVES]              = 0.2F;
		g_BlockDigTime[E_BLOCK_NEW_LOG]                 = 2.0F;
		g_BlockDigTime[E_BLOCK_ACACIA_WOOD_STAIRS]      = 2.0F;
		g_BlockDigTime[E_BLOCK_DARK_OAK_WOOD_STAIRS]    = 2.0F;
		g_BlockDigTime[E_BLOCK_HAY_BALE]                = 0.5F;
		g_BlockDigTime[E_BLOCK_CARPET]                  = 0.1F;
		g_BlockDigTime[E_BLOCK_HARDENED_CLAY]           = 1.25F;
		g_BlockDigTime[E_BLOCK_BLOCK_OF_COAL]           = 5.0F;
		g_BlockDigTime[E_BLOCK_PACKED_ICE]              = 0.5F;
		g_BlockDigTime[E_BLOCK_BIG_FLOWER]              = 0.0F;
		g_BlockDigTime[E_BLOCK_BEDROCK]                 = 100.0F;  // Is this correct?


		// Blocks that break when pushed by piston:
		g_BlockPistonBreakable[E_BLOCK_ACTIVE_COMPARATOR]     = true;
		g_BlockPistonBreakable[E_BLOCK_AIR]                   = true;
		g_BlockPistonBreakable[E_BLOCK_BED]                   = true;
		g_BlockPistonBreakable[E_BLOCK_BIG_FLOWER]            = true;
		g_BlockPistonBreakable[E_BLOCK_BROWN_MUSHROOM]        = true;
		g_BlockPistonBreakable[E_BLOCK_COBWEB]                = true;
		g_BlockPistonBreakable[E_BLOCK_CROPS]                 = true;
		g_BlockPistonBreakable[E_BLOCK_DANDELION]             = true;
		g_BlockPistonBreakable[E_BLOCK_DEAD_BUSH]             = true;
		g_BlockPistonBreakable[E_BLOCK_FIRE]                  = true;
		g_BlockPistonBreakable[E_BLOCK_FLOWER]                = true;
		g_BlockPistonBreakable[E_BLOCK_INACTIVE_COMPARATOR]   = true;
		g_BlockPistonBreakable[E_BLOCK_IRON_DOOR]             = true;
		g_BlockPistonBreakable[E_BLOCK_JACK_O_LANTERN]        = true;
		g_BlockPistonBreakable[E_BLOCK_LADDER]                = true;
		g_BlockPistonBreakable[E_BLOCK_LAVA]                  = true;
		g_BlockPistonBreakable[E_BLOCK_LEVER]                 = true;
		g_BlockPistonBreakable[E_BLOCK_MELON]                 = true;
		g_BlockPistonBreakable[E_BLOCK_MELON_STEM]            = true;
		g_BlockPistonBreakable[E_BLOCK_PUMPKIN]               = true;
		g_BlockPistonBreakable[E_BLOCK_PUMPKIN_STEM]          = true;
		g_BlockPistonBreakable[E_BLOCK_REDSTONE_REPEATER_OFF] = true;
		g_BlockPistonBreakable[E_BLOCK_REDSTONE_REPEATER_ON]  = true;
		g_BlockPistonBreakable[E_BLOCK_REDSTONE_TORCH_OFF]    = true;
		g_BlockPistonBreakable[E_BLOCK_REDSTONE_TORCH_ON]     = true;
		g_BlockPistonBreakable[E_BLOCK_REDSTONE_WIRE]         = true;
		g_BlockPistonBreakable[E_BLOCK_RED_MUSHROOM]          = true;
		g_BlockPistonBreakable[E_BLOCK_REEDS]                 = true;
		g_BlockPistonBreakable[E_BLOCK_SNOW]                  = true;
		g_BlockPistonBreakable[E_BLOCK_STATIONARY_LAVA]       = true;
		g_BlockPistonBreakable[E_BLOCK_STATIONARY_WATER]      = true;
		g_BlockPistonBreakable[E_BLOCK_STONE_BUTTON]          = true;
		g_BlockPistonBreakable[E_BLOCK_STONE_PRESSURE_PLATE]  = true;
		g_BlockPistonBreakable[E_BLOCK_TALL_GRASS]            = true;
		g_BlockPistonBreakable[E_BLOCK_TORCH]                 = true;
		g_BlockPistonBreakable[E_BLOCK_VINES]                 = true;
		g_BlockPistonBreakable[E_BLOCK_WATER]                 = true;
		g_BlockPistonBreakable[E_BLOCK_WOODEN_BUTTON]         = true;
		g_BlockPistonBreakable[E_BLOCK_WOODEN_DOOR]           = true;
		g_BlockPistonBreakable[E_BLOCK_WOODEN_PRESSURE_PLATE] = true;


		// Blocks that cannot be snowed over:
		g_BlockIsSnowable[E_BLOCK_ACTIVE_COMPARATOR]     = false;
		g_BlockIsSnowable[E_BLOCK_AIR]                   = false;
		g_BlockIsSnowable[E_BLOCK_BIG_FLOWER]            = false;
		g_BlockIsSnowable[E_BLOCK_BROWN_MUSHROOM]        = false;
		g_BlockIsSnowable[E_BLOCK_CACTUS]                = false;
		g_BlockIsSnowable[E_BLOCK_CHEST]                 = false;
		g_BlockIsSnowable[E_BLOCK_CROPS]                 = false;
		g_BlockIsSnowable[E_BLOCK_DANDELION]             = false;
		g_BlockIsSnowable[E_BLOCK_FIRE]                  = false;
		g_BlockIsSnowable[E_BLOCK_FLOWER]                = false;
		g_BlockIsSnowable[E_BLOCK_GLASS]                 = false;
		g_BlockIsSnowable[E_BLOCK_ICE]                   = false;
		g_BlockIsSnowable[E_BLOCK_INACTIVE_COMPARATOR]   = false;
		g_BlockIsSnowable[E_BLOCK_LAVA]                  = false;
		g_BlockIsSnowable[E_BLOCK_LILY_PAD]              = false;
		g_BlockIsSnowable[E_BLOCK_REDSTONE_REPEATER_OFF] = false;
		g_BlockIsSnowable[E_BLOCK_REDSTONE_REPEATER_ON]  = false;
		g_BlockIsSnowable[E_BLOCK_REDSTONE_TORCH_OFF]    = false;
		g_BlockIsSnowable[E_BLOCK_REDSTONE_TORCH_ON]     = false;
		g_BlockIsSnowable[E_BLOCK_REDSTONE_WIRE]         = false;
		g_BlockIsSnowable[E_BLOCK_RED_MUSHROOM]          = false;
		g_BlockIsSnowable[E_BLOCK_REEDS]                 = false;
		g_BlockIsSnowable[E_BLOCK_SAPLING]               = false;
		g_BlockIsSnowable[E_BLOCK_SIGN_POST]             = false;
		g_BlockIsSnowable[E_BLOCK_SNOW]                  = false;
		g_BlockIsSnowable[E_BLOCK_STAINED_GLASS]         = false;
		g_BlockIsSnowable[E_BLOCK_STAINED_GLASS_PANE]    = false;
		g_BlockIsSnowable[E_BLOCK_STATIONARY_LAVA]       = false;
		g_BlockIsSnowable[E_BLOCK_STATIONARY_WATER]      = false;
		g_BlockIsSnowable[E_BLOCK_TALL_GRASS]            = false;
		g_BlockIsSnowable[E_BLOCK_TNT]                   = false;
		g_BlockIsSnowable[E_BLOCK_TORCH]                 = false;
		g_BlockIsSnowable[E_BLOCK_VINES]                 = false;
		g_BlockIsSnowable[E_BLOCK_WALLSIGN]              = false;
		g_BlockIsSnowable[E_BLOCK_WATER]                 = false;
		g_BlockIsSnowable[E_BLOCK_RAIL]                  = false;
		g_BlockIsSnowable[E_BLOCK_ACTIVATOR_RAIL]        = false;
		g_BlockIsSnowable[E_BLOCK_POWERED_RAIL]          = false;
		g_BlockIsSnowable[E_BLOCK_DETECTOR_RAIL]         = false;
		g_BlockIsSnowable[E_BLOCK_COBWEB]                = false;
		

		// Blocks that don't drop without a special tool:
		g_BlockRequiresSpecialTool[E_BLOCK_BRICK]                = true;
		g_BlockRequiresSpecialTool[E_BLOCK_CAULDRON]             = true;
		g_BlockRequiresSpecialTool[E_BLOCK_COAL_ORE]             = true;
		g_BlockRequiresSpecialTool[E_BLOCK_COBBLESTONE]          = true;
		g_BlockRequiresSpecialTool[E_BLOCK_COBBLESTONE_STAIRS]   = true;
		g_BlockRequiresSpecialTool[E_BLOCK_COBWEB]               = true;
		g_BlockRequiresSpecialTool[E_BLOCK_DIAMOND_BLOCK]        = true;
		g_BlockRequiresSpecialTool[E_BLOCK_DIAMOND_ORE]          = true;
		g_BlockRequiresSpecialTool[E_BLOCK_DOUBLE_STONE_SLAB]    = true;
		g_BlockRequiresSpecialTool[E_BLOCK_EMERALD_ORE]          = true;
		g_BlockRequiresSpecialTool[E_BLOCK_END_STONE]            = true;
		g_BlockRequiresSpecialTool[E_BLOCK_GOLD_BLOCK]           = true;
		g_BlockRequiresSpecialTool[E_BLOCK_GOLD_ORE]             = true;
		g_BlockRequiresSpecialTool[E_BLOCK_IRON_BLOCK]           = true;
		g_BlockRequiresSpecialTool[E_BLOCK_IRON_ORE]             = true;
		g_BlockRequiresSpecialTool[E_BLOCK_LAPIS_BLOCK]          = true;
		g_BlockRequiresSpecialTool[E_BLOCK_LAPIS_ORE]            = true;
		g_BlockRequiresSpecialTool[E_BLOCK_MOSSY_COBBLESTONE]    = true;
		g_BlockRequiresSpecialTool[E_BLOCK_NETHERRACK]           = true;
		g_BlockRequiresSpecialTool[E_BLOCK_NETHER_BRICK]         = true;
		g_BlockRequiresSpecialTool[E_BLOCK_NETHER_BRICK_STAIRS]  = true;
		g_BlockRequiresSpecialTool[E_BLOCK_OBSIDIAN]             = true;
		g_BlockRequiresSpecialTool[E_BLOCK_REDSTONE_ORE]         = true;
		g_BlockRequiresSpecialTool[E_BLOCK_REDSTONE_ORE_GLOWING] = true;
		g_BlockRequiresSpecialTool[E_BLOCK_SANDSTONE]            = true;
		g_BlockRequiresSpecialTool[E_BLOCK_SANDSTONE_STAIRS]     = true;
		g_BlockRequiresSpecialTool[E_BLOCK_SNOW]                 = true;
		g_BlockRequiresSpecialTool[E_BLOCK_STONE]                = true;
		g_BlockRequiresSpecialTool[E_BLOCK_STONE_BRICKS]         = true;
		g_BlockRequiresSpecialTool[E_BLOCK_STONE_BRICK_STAIRS]   = true;
		g_BlockRequiresSpecialTool[E_BLOCK_STONE_PRESSURE_PLATE] = true;
		g_BlockRequiresSpecialTool[E_BLOCK_STONE_SLAB]           = true;
		g_BlockRequiresSpecialTool[E_BLOCK_VINES]                = true;
		g_BlockRequiresSpecialTool[E_BLOCK_FURNACE]              = true;
		g_BlockRequiresSpecialTool[E_BLOCK_LIT_FURNACE]          = true;
		g_BlockRequiresSpecialTool[E_BLOCK_ANVIL]                = true;
		g_BlockRequiresSpecialTool[E_BLOCK_ENCHANTMENT_TABLE]    = true;

		// Nonsolid blocks:
		g_BlockIsSolid[E_BLOCK_ACTIVATOR_RAIL]        = false;
		g_BlockIsSolid[E_BLOCK_AIR]                   = false;
		g_BlockIsSolid[E_BLOCK_BIG_FLOWER]            = false;
		g_BlockIsSolid[E_BLOCK_BROWN_MUSHROOM]        = false;
		g_BlockIsSolid[E_BLOCK_CARROTS]               = false;
		g_BlockIsSolid[E_BLOCK_COBWEB]                = false;
		g_BlockIsSolid[E_BLOCK_CROPS]                 = false;
		g_BlockIsSolid[E_BLOCK_DANDELION]             = false;
		g_BlockIsSolid[E_BLOCK_DETECTOR_RAIL]         = false;
		g_BlockIsSolid[E_BLOCK_END_PORTAL]            = false;
		g_BlockIsSolid[E_BLOCK_FIRE]                  = false;
		g_BlockIsSolid[E_BLOCK_FLOWER]                = false;
		g_BlockIsSolid[E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE] = false;
		g_BlockIsSolid[E_BLOCK_LAVA]                  = false;
		g_BlockIsSolid[E_BLOCK_LEVER]                 = false;
		g_BlockIsSolid[E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE] = false;
		g_BlockIsSolid[E_BLOCK_MELON_STEM]            = false;
		g_BlockIsSolid[E_BLOCK_NETHER_PORTAL]         = false;
		g_BlockIsSolid[E_BLOCK_PISTON_EXTENSION]      = false;
		g_BlockIsSolid[E_BLOCK_POTATOES]              = false;
		g_BlockIsSolid[E_BLOCK_POWERED_RAIL]          = false;
		g_BlockIsSolid[E_BLOCK_RAIL]                  = false;
		g_BlockIsSolid[E_BLOCK_REDSTONE_TORCH_OFF]    = false;
		g_BlockIsSolid[E_BLOCK_REDSTONE_TORCH_ON]     = false;
		g_BlockIsSolid[E_BLOCK_REDSTONE_WIRE]         = false;
		g_BlockIsSolid[E_BLOCK_RED_MUSHROOM]          = false;
		g_BlockIsSolid[E_BLOCK_REEDS]                 = false;
		g_BlockIsSolid[E_BLOCK_SAPLING]               = false;
		g_BlockIsSolid[E_BLOCK_SIGN_POST]             = false;
		g_BlockIsSolid[E_BLOCK_SNOW]                  = false;
		g_BlockIsSolid[E_BLOCK_STATIONARY_LAVA]       = false;
		g_BlockIsSolid[E_BLOCK_STATIONARY_WATER]      = false;
		g_BlockIsSolid[E_BLOCK_STONE_BUTTON]          = false;
		g_BlockIsSolid[E_BLOCK_STONE_PRESSURE_PLATE]  = false;
		g_BlockIsSolid[E_BLOCK_TALL_GRASS]            = false;
		g_BlockIsSolid[E_BLOCK_TORCH]                 = false;
		g_BlockIsSolid[E_BLOCK_TRIPWIRE]              = false;
		g_BlockIsSolid[E_BLOCK_VINES]                 = false;
		g_BlockIsSolid[E_BLOCK_WALLSIGN]              = false;
		g_BlockIsSolid[E_BLOCK_WATER]                 = false;
		g_BlockIsSolid[E_BLOCK_WOODEN_BUTTON]         = false;
		g_BlockIsSolid[E_BLOCK_WOODEN_PRESSURE_PLATE] = false;
		g_BlockIsSolid[E_BLOCK_WOODEN_SLAB]           = false;

		// Torch placeable blocks:
		g_BlockFullyOccupiesVoxel[E_BLOCK_NEW_LOG]               = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_BEDROCK]               = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_BLOCK_OF_COAL]         = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_BLOCK_OF_REDSTONE]     = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_BOOKCASE]              = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_BRICK]                 = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_CLAY]                  = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_COAL_ORE]              = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_COBBLESTONE]           = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_COMMAND_BLOCK]         = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_CRAFTING_TABLE]        = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_DIAMOND_BLOCK]         = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_DIAMOND_ORE]           = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_DIRT]                  = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_DISPENSER]             = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_DOUBLE_STONE_SLAB]     = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_DOUBLE_WOODEN_SLAB]    = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_DROPPER]               = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_EMERALD_BLOCK]         = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_EMERALD_ORE]           = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_END_STONE]             = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_FURNACE]               = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_GLOWSTONE]             = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_GOLD_BLOCK]            = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_GOLD_ORE]              = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_GRASS]                 = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_GRAVEL]                = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_HARDENED_CLAY]         = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_HAY_BALE]              = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_HUGE_BROWN_MUSHROOM]   = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_HUGE_RED_MUSHROOM]     = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_IRON_BLOCK]            = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_IRON_ORE]              = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_JACK_O_LANTERN]        = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_JUKEBOX]               = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_LAPIS_BLOCK]           = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_LAPIS_ORE]             = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_LOG]                   = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_MELON]                 = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_MOSSY_COBBLESTONE]     = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_MYCELIUM]              = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_NETHERRACK]            = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_NETHER_BRICK]          = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_NETHER_QUARTZ_ORE]     = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_NOTE_BLOCK]            = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_OBSIDIAN]              = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_PACKED_ICE]            = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_PLANKS]                = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_PUMPKIN]               = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_QUARTZ_BLOCK]          = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_REDSTONE_LAMP_OFF]     = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_REDSTONE_LAMP_ON]      = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_REDSTONE_ORE]          = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_REDSTONE_ORE_GLOWING]  = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_SANDSTONE]             = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_SAND]                  = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_SILVERFISH_EGG]        = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_SPONGE]                = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_STAINED_CLAY]          = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_WOOL]                  = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_STONE]                 = true;
		g_BlockFullyOccupiesVoxel[E_BLOCK_STONE_BRICKS]          = true;
	}
} BlockPropertiesInitializer;





