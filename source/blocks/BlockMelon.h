#pragma once
#include "Block.h"


class cBlockMelonHandler : public cBlockHandler
{
public:
	cBlockMelonHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}
	
	
	virtual int GetDropID()
	{
		return E_ITEM_MELON_SLICE;
	}

	virtual char GetDropCount()
	{
		MTRand r1;
		return (char)(3 + r1.randInt(4));
	}
};