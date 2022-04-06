
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BlockType.h"
#include "PointOfInterest.h"





ePointOfInterestType cPointOfInterest::GetPointOnInterestType(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	switch (a_BlockType)
	{
		/*
		case E_BLOCK_BLAST_FURNACE:
		{
			return poiArmorer;
		}

		case E_BLOCK_SMOKER:
		{
			return poiButcher;
		}

		case E_BLOCK_CARTOGRAPHY_TABLE:
		{
			return poiCartographer;
		}
		*/

		case E_BLOCK_BREWING_STAND:
		{
			return poiCleric;
		}

		/*
		case E_BLOCK_COMPOSTER:
		{
			return poiFarmer;
		}

		case E_BLOCK_BARREL:
		{
			return poiFisherman;
		}

		case E_BLOCK_FLETCHING_TABLE:
		{
			return poiFletcher;
		}
		*/

		case E_BLOCK_CAULDRON:
		{
			return poiLetherworker;
		}

		/*
		case E_BLOCK_LECTERN:
		{
			return poiLibrairian;
		}

		case E_BLOCK_STONECUTTER:
		{
			return poiMason;
		}

		case E_BLOCK_LOOM:
		{
			return poiShepherd;
		}

		case E_BLOCK_SMITHING_TABLE:
		{
			return poiToolsmith;
		}

		case E_BLOCK_GRINDSTONE:
		{
			return poiWeaponsmith;
		}
		*/

		// The bed is a special case: only the head is a poi
		case E_BLOCK_BED:
		{
			if ((a_BlockMeta >> 3 & 1) == 1)
			{
				return poiHome;
			}
			else
			{
				return poiNone;
			}
		}

		/*
		case E_BLOCK_BELL:
		{
			return poiMeeting;
		}

		case E_BLOCK_BEEHIVE:
		{
			return poiBeehive;
		}

		case E_BLOCK_BEE_NEST:
		{
			return poiBeeNest;
		}
		*/

		case E_BLOCK_NETHER_PORTAL:
		{
			return poiNetherPortal;
		}

		/*
		case E_BLOCK_LODESTONE:
		{
			return poiLodestone;
		}

		case E_BLOCK_LIGHTNING_ROD:
		{
			return poiLightningRod;
		}
		*/

		default:
		{
			return poiNone;
		}
	}
}





int cPointOfInterest::GetMaxFreeTickets(ePointOfInterestType a_Type)
{
	switch (a_Type)
	{
		case poiArmorer:
		case poiButcher:
		case poiCartographer:
		case poiCleric:
		case poiFarmer:
		case poiFisherman:
		case poiFletcher:
		case poiLetherworker:
		case poiLibrairian:
		case poiMason:
		case poiShepherd:
		case poiToolsmith:
		case poiWeaponsmith:
		case poiHome:
		{
			return 1;
		}

		case poiMeeting:
		{
			return 32;
		}

	default:
		{
			return 0;
		}
	}
}





std::string cPointOfInterest::GetPoiTypeName(ePointOfInterestType a_Type)
{
	switch (a_Type)
	{
		case poiArmorer:        return "minecraft:armorer";
		case poiBeehive:        return "minecraft:beehive";
		case poiBeeNest:        return "minecraft:beenest";
		case poiButcher:        return "minecraft:butcher";
		case poiCartographer:   return "minecraft:cartographer";
		case poiCleric:         return "minecraft:cleric";
		case poiFarmer:         return "minecraft:farmer";
		case poiFisherman:      return "minecraft:fisherman";
		case poiFletcher:       return "minecraft:fletcher";
		case poiHome:           return "minecraft:home";
		case poiLetherworker:   return "minecraft:letherworker";
		case poiLibrairian:     return "minecraft:librairian";
		case poiLightningRod:   return "minecraft:lightning_rod";
		case poiLodestone:      return "minecraft:lodestone";
		case poiMason:          return "minecraft:mason";
		case poiMeeting:        return "minecraft:meeting";
		case poiNetherPortal:   return "minecraft:nether_portal";
		case poiShepherd:       return "minecraft:shepherd";
		case poiToolsmith:      return "minecraft:toolsmith";
		case poiWeaponsmith:    return "minecraft:weaponsmith";

		case poiNone:    return "none";
	}
}





ePointOfInterestType cPointOfInterest::GetPoiFromString(const std::string & a_PoiName)
{
	static const std::map<std::string, ePointOfInterestType> StringToPoi
	{
		{ "minecraft:armorer",          poiArmorer },
		{ "minecraft:beehive",          poiBeehive },
		{ "minecraft:beenest",          poiBeeNest },
		{ "minecraft:butcher",          poiButcher },
		{ "minecraft:cartographer",     poiCartographer },
		{ "minecraft:cleric",           poiCleric },
		{ "minecraft:farmer",           poiFarmer },
		{ "minecraft:fisherman",        poiFisherman },
		{ "minecraft:fletcher",         poiFletcher },
		{ "minecraft:home",             poiHome },
		{ "minecraft:letherworker",     poiLetherworker },
		{ "minecraft:librairian",       poiLibrairian },
		{ "minecraft:lightning_rod",    poiLightningRod },
		{ "minecraft:lodestone",        poiLodestone },
		{ "minecraft:mason",            poiMason },
		{ "minecraft:meeting",          poiMeeting },
		{ "minecraft:nether_portal",    poiNetherPortal },
		{ "minecraft:shepherd",         poiShepherd },
		{ "minecraft:toolsmith",        poiToolsmith },
		{ "minecraft:weaponsmith",      poiWeaponsmith },
	};

	const auto It = StringToPoi.find(a_PoiName);

	if (It != StringToPoi.end())
	{
		return It->second;
	}

	return poiNone;
}
