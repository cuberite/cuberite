
#pragma once

#include "BlockEntity.h"





class cBlockCommandBlockHandler :
	public cBlockEntityHandler
{
public:
	cBlockCommandBlockHandler(BLOCKTYPE a_BlockType)
		: cBlockEntityHandler(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_BLOCK_AIR, 8, 0));
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 10;
	}
} ;




