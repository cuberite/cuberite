#pragma once
#include "Block.h"


class cBlockClothHandler : public cBlockHandler
{
public:
	cBlockClothHandler(BLOCKTYPE a_BlockID);
	char GetDropMeta(char a_BlockMeta)
	{
		return a_BlockMeta;
	}
	
};