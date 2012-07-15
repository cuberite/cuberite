#pragma once
#include "Block.h"
#include "../MersenneTwister.h"
#include "../cWorld.h"

class cBlockRedstoneOreHandler : public cBlockHandler
{
public:
	cBlockRedstoneOreHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}

	virtual int GetDropID()
	{
		return E_ITEM_REDSTONE_DUST;
	}

	virtual char GetDropCount()
	{
		MTRand r1;
		return 4 + r1.randInt(1);
	}

	
};