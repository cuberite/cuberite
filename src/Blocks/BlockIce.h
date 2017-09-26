
#pragma once

#include "BlockHandler.h"





class cBlockIceHandler :
	public cBlockHandler
{
public:
	cBlockIceHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// No pickups
	}

	virtual void OnDestroyedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		if (a_Player.IsGameModeCreative() || (a_BlockY <= 0))
		{
			return;
		}

		cEnchantments Enchantments = a_Player.GetInventory().GetEquippedItem().m_Enchantments;
		if (Enchantments.GetLevel(cEnchantments::enchSilkTouch) == 0)
		{
			BLOCKTYPE BlockBelow = a_ChunkInterface.GetBlock({a_BlockX, a_BlockY - 1, a_BlockZ});
			if (!cBlockInfo::FullyOccupiesVoxel(BlockBelow) && !IsBlockLiquid(BlockBelow))
			{
				return;
			}

			a_ChunkInterface.SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_WATER, 0);
		}
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 5;
	}
} ;
