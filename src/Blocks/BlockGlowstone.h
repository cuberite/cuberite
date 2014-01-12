
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
		// Reset meta to 0
		MTRand r1;
		a_Pickups.push_back(cItem(E_ITEM_GLOWSTONE_DUST, (char)(2 + r1.randInt(2)), 0));
	}
} ;




