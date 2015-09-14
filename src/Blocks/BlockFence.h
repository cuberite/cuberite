
#pragma once

#include "BlockHandler.h"




class cBlockFenceHandler :
	public cBlockHandler
{
public:
	cBlockFenceHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	inline static bool IsFenceBlockType(BLOCKTYPE a_Block)
	{
		switch (a_Block)
		{
			case E_BLOCK_FENCE:
			case E_BLOCK_ACACIA_FENCE:
			case E_BLOCK_BIRCH_FENCE:
			case E_BLOCK_DARK_OAK_FENCE:
			case E_BLOCK_JUNGLE_FENCE:
			case E_BLOCK_NETHER_BRICK_FENCE:
			case E_BLOCK_SPRUCE_FENCE:
			{
				return true;
			}
			default:
			{
				return false;
			}
		} // switch (a_Block)
	}
};
