
#pragma once

#include "BlockHandler.h"





class cBlockRedstoneLampHandler :
	public cBlockHandler
{
public:
	cBlockRedstoneLampHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_BLOCK_REDSTONE_LAMP_OFF, 1, 0));
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 15;
	}
};




