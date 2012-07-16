#pragma once
#include "Block.h"


class cBlockWoodHandler : public cBlockHandler
{
public:
	cBlockWoodHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}
	NIBBLETYPE GetDropMeta(NIBBLETYPE a_BlockMeta) override
	{
		return a_BlockMeta;
	}
	
};