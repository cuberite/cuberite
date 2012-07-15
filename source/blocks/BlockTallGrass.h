#pragma once
#include "Block.h"


class cBlockTallGrassHandler : public cBlockHandler
{
public:
	cBlockTallGrassHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}
	
	virtual bool IgnoreBuildCollision()
	{
		return true;
	}
	
	virtual int GetDropID()
	{
		return E_ITEM_SEEDS;
	}

	virtual char GetDropCount()
	{
		MTRand r1;
		if(r1.randInt(10) == 5)
			return 1;
		return 0;
	}

	virtual bool CanBeAt(cWorld *a_World, int a_X, int a_Y, int a_Z)
	{
		return a_World->GetBlock(a_X, a_Y - 1, a_Z) != E_BLOCK_AIR;
	}
};