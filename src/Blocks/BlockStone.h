
#pragma once

#include "BlockHandler.h"
#include "ClearMetaOnDrop.h"





class cBlockStoneHandler :
	public cClearMetaOnDrop<cBlockHandler>
{
public:
	cBlockStoneHandler(BLOCKTYPE a_BlockType)
		: cClearMetaOnDrop<cBlockHandler>(a_BlockType)
	{
	}

} ;




