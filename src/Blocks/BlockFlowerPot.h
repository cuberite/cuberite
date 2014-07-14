
#pragma once

#include "BlockHandler.h"
#include "BlockEntity.h"





class cBlockFlowerPotHandler :
	public cClearMetaOnDrop<cBlockEntityHandler>
{
public:
	cBlockFlowerPotHandler(BLOCKTYPE a_BlockType) :
		cClearMetaOnDrop<cBlockEntityHandler>(a_BlockType)
	{
	}
} ;




