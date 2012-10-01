
#pragma once

#include "BlockHandler.h"





class cBlockGlowstoneHandler :
	public cBlockHandler
{
public:
	cBlockGlowstoneHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}
	

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to 0
		// TODO: More drops?
		a_Pickups.push_back(cItem(E_ITEM_GLOWSTONE_DUST, 1, 0));
	}
} ;




