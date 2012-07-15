#pragma once
#include "Block.h"


class cBlockGlowstoneHandler : public cBlockHandler
{
public:
	cBlockGlowstoneHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}
	
	virtual char GetDropMeta(char a_BlockMeta)
	{
		return 0;
	}

	virtual int GetDropID()
	{
		return E_ITEM_GLOWSTONE_DUST;
	}
};