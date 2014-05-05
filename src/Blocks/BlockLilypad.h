
#pragma once

#include "BlockHandler.h"
#include "Entities/Pickup.h"




class cBlockLilypadHandler :
	public cBlockHandler
{
public:
	cBlockLilypadHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}
	
	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to zero
		a_Pickups.push_back(cItem(E_BLOCK_LILY_PAD, 1, 0));
	}
};




