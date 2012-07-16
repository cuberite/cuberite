#pragma once
#include "Block.h"


class cBlockClothHandler : public cBlockHandler
{
public:
	cBlockClothHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}

	virtual NIBBLETYPE GetDropMeta(NIBBLETYPE a_BlockMeta) override
	{
		return a_BlockMeta;
	}
	
};