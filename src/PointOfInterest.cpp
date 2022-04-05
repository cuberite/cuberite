
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
