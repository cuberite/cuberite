
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BlockType.h"
#include "PointOfInterest.h"





ePoiType cPointOfInterest::GetPointOnInterestType(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
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
			return ePoiType::poiCleric;
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
			return ePoiType::poiLetherworker;
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
			if (a_BlockMeta & E_BLOCK_BED_BED_HEAD)
			{
				return ePoiType::poiHome;
			}
			else
			{
				return ePoiType::poiNone;
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
			return ePoiType::poiNetherPortal;
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
			return ePoiType::poiNone;
		}
	}
}





unsigned char cPointOfInterest::GetMaxFreeTickets(ePoiType a_Type)
{
	// those values can be found at https://minecraft.fandom.com/wiki/Java_Edition_level_format#poi_format

	switch (a_Type)
	{
		case ePoiType::poiArmorer:
		case ePoiType::poiButcher:
		case ePoiType::poiCartographer:
		case ePoiType::poiCleric:
		case ePoiType::poiFarmer:
		case ePoiType::poiFisherman:
		case ePoiType::poiFletcher:
		case ePoiType::poiLetherworker:
		case ePoiType::poiLibrairian:
		case ePoiType::poiMason:
		case ePoiType::poiShepherd:
		case ePoiType::poiToolsmith:
		case ePoiType::poiWeaponsmith:
		case ePoiType::poiHome:
		{
			return 1;
		}

		case ePoiType::poiMeeting:
		{
			return 32;
		}

		case ePoiType::poiBeehive:
		case ePoiType::poiBeeNest:
		case ePoiType::poiNetherPortal:
		case ePoiType::poiLodestone:
		case ePoiType::poiLightningRod:
		case ePoiType::poiNone:
		{
			return 0;
		}
	}

	return 0;
}
