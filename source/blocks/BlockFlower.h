#pragma once
#include "Block.h"


class cBlockFlowerHandler : public cBlockHandler
{
public:
	cBlockFlowerHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}

	virtual char GetDropMeta(char a_BlockMeta)
	{
		return 0;
	}

	virtual bool CanBeAt(cWorld *a_World, int a_X, int a_Y, int a_Z)
	{
		return IsBlockTypeOfDirt(a_World->GetBlock(a_X, a_Y - 1, a_Z));
	}

	virtual bool AllowBlockOnTop()
	{
		return false;
	}
	
	virtual bool CanBePlacedOnSide()
	{
		return false;
	}
};