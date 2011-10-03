#include "cBlockToPickup.h"
#include "BlockID.h"

ENUM_ITEM_ID cBlockToPickup::ToPickup( unsigned char a_BlockID, ENUM_ITEM_ID a_UsedItemID )
{
	(void)a_UsedItemID;

	switch( a_BlockID )
	{
	case E_BLOCK_AIR:
		return E_ITEM_EMPTY;
	case E_BLOCK_STONE:
		return E_ITEM_COBBLESTONE;
	case E_BLOCK_GRASS:
		return E_ITEM_DIRT;
	case E_BLOCK_DIRT:
		return E_ITEM_DIRT;
	case E_BLOCK_LOG:
		return E_ITEM_LOG;
	case E_BLOCK_COAL_ORE:
		return E_ITEM_COAL;
	case E_BLOCK_DIAMOND_ORE:
		return E_ITEM_DIAMOND;
	case E_BLOCK_IRON_BLOCK:
		return E_ITEM_IRON_BLOCK;
	case E_BLOCK_DIAMOND_BLOCK:
		return E_ITEM_DIAMOND_BLOCK;
	case E_BLOCK_GOLD_BLOCK:
		return E_ITEM_GOLD_BLOCK;
	case E_BLOCK_SIGN_POST:
	case E_BLOCK_WALLSIGN:
		return E_ITEM_SIGN;
	default:
		return (ENUM_ITEM_ID)a_BlockID;
	}
	return E_ITEM_EMPTY;
}
