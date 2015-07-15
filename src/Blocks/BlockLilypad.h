
#pragma once

#include "BlockHandler.h"
#include "Entities/Pickup.h"




class cBlockLilypadHandler :
	public cClearMetaOnDrop<cBlockHandler>
{
	typedef cClearMetaOnDrop<cBlockHandler> super;
public:

	cBlockLilypadHandler(BLOCKTYPE a_BlockType) :
		super(a_BlockType)
	{
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 7;
	}
};




