
#pragma once

#include "BlockHandler.h"





class cBlockGlowstoneHandler :
	public cBlockHandler
{
public:
	cBlockGlowstoneHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Add more than one dust
		a_Pickups.emplace_back(E_ITEM_GLOWSTONE_DUST, GetRandomProvider().RandInt<char>(2, 4), 0);
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 2;
	}
} ;




