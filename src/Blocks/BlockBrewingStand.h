
#pragma once

#include "BlockHandler.h"
#include "ClearMetaOnDrop.h"





class cBlockBrewingStandHandler :
	public cClearMetaOnDrop<cBlockHandler>
{
public:
	cBlockBrewingStandHandler(BLOCKTYPE a_BlockType)
		: cClearMetaOnDrop<cBlockHandler>(a_BlockType)
	{
	}

	virtual bool IsUseable() override
	{
		return true;
	}
} ;




