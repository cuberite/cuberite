#include "cBlockToPickup.h"
#include "BlockID.h"
#include "stdlib.h"

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
	case E_BLOCK_GLASS:
		return E_ITEM_EMPTY;
	case E_BLOCK_DIRT:
		return E_ITEM_DIRT;
	case E_BLOCK_LOG:
		return E_ITEM_LOG;
	case E_BLOCK_LEAVES:
		if( a_UsedItemID == E_ITEM_SHEARS )
			return E_ITEM_LEAVES;
		else
			if(rand() % 5 == 0)
				return E_ITEM_SAPLING;
			return E_ITEM_EMPTY;
	case E_BLOCK_COAL_ORE:
		return E_ITEM_COAL;
	case E_BLOCK_LAPIS_ORE:
		return E_ITEM_DYE;
	case E_BLOCK_REDSTONE_ORE_GLOWING:
	case E_BLOCK_REDSTONE_ORE:
		return E_ITEM_REDSTONE_DUST;
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
	case E_BLOCK_REDSTONE_WIRE:
		return E_ITEM_REDSTONE_DUST;
	case E_BLOCK_REDSTONE_TORCH_OFF:
		return E_ITEM_REDSTONE_TORCH_ON;
	case E_BLOCK_MELON:
		return E_ITEM_MELON_SLICE;
	default:
		return (ENUM_ITEM_ID)a_BlockID;
	}
	//TODO: Whats that? :D
	return E_ITEM_EMPTY;
}
