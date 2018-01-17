
#pragma once

#include "BlockHandler.h"




class cBlockStoneHandler :
	public cBlockHandler
{
public:
	cBlockStoneHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		if (a_BlockMeta == E_META_STONE_STONE)
		{
			a_Pickups.push_back(cItem(E_BLOCK_COBBLESTONE, 1, 0));
			return;
		}
		a_Pickups.push_back(cItem(E_BLOCK_STONE, 1, a_BlockMeta));
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 11;
	}
};




