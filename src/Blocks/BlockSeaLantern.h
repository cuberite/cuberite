
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
		a_Pickups.emplace_back(E_ITEM_PRISMARINE_CRYSTALS, GetRandomProvider().RandInt<char>(2, 3), 0);
	}
} ;




