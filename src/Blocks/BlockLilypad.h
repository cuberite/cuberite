
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
};




