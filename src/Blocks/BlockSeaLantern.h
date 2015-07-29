
#pragma once

#include "BlockHandler.h"





class cBlockSeaLanternHandler :
	public cBlockHandler
{
public:
	cBlockSeaLanternHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to 0
		cFastRandom Random;
		a_Pickups.push_back(cItem(E_ITEM_PRISMARINE_CRYSTALS, static_cast<char>(2 + Random.NextInt(2)), 0));
	}
} ;




