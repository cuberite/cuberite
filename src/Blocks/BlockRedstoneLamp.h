
#pragma once

#include "BlockHandler.h"





class cBlockRedstoneLampHandler :
	public cClearMetaOnDrop<cBlockHandler>
{
public:
	cBlockRedstoneLampHandler(BLOCKTYPE a_BlockType)
		: cClearMetaOnDrop<cBlockHandler>(a_BlockType)
	{
	}
};




