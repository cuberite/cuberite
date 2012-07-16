#pragma once
#include "Block.h"


class cBlockGlowstoneHandler : public cBlockHandler
{
public:
	cBlockGlowstoneHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}
	
	virtual NIBBLETYPE GetDropMeta(NIBBLETYPE a_BlockMeta) override
	{
		return 0;
	}

	virtual int GetDropID() override
	{
		return E_ITEM_GLOWSTONE_DUST;
	}
};