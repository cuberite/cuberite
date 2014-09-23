
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
		cFastRandom Random;
		a_Pickups.push_back(cItem(E_ITEM_MELON_SLICE, (char)(3 + Random.NextInt(5)), 0));
	}
} ;




