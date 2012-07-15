#pragma once
#include "Block.h"


class cBlockWoodHandler : public cBlockHandler
{
public:
	cBlockWoodHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}
	char GetDropMeta(char a_BlockMeta)
	{
		return a_BlockMeta;
	}
	
};