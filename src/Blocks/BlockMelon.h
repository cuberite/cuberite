
#pragma once

#include "BlockHandler.h"





class cBlockMelonHandler :
	public cBlockHandler
{
public:
	cBlockMelonHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.emplace_back(E_ITEM_MELON_SLICE, GetRandomProvider().RandInt<char>(3, 7), 0);
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 19;
	}
} ;




