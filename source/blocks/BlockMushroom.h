#pragma once
#include "BlockHandler.h"


class cBlockMushroomHandler : public cBlockHandler
{
public:
	cBlockMushroomHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}

	virtual NIBBLETYPE GetDropMeta(NIBBLETYPE a_BlockMeta) override
	{
		return 0;
	}

	virtual bool CanBeAt(cWorld *a_World, int a_X, int a_Y, int a_Z) override
	{
		switch (a_World->GetBlock(a_X, a_Y - 1, a_Z))
		{
			case E_BLOCK_GLASS:
			case E_BLOCK_CACTUS:
			case E_BLOCK_ICE:
			case E_BLOCK_LEAVES:
			case E_BLOCK_AIR:
				return false;
		}
		return true;
	}

	virtual bool AllowBlockOnTop() override
	{
		return false;
	}

	
	virtual bool CanBePlacedOnSide() override
	{
		return false;
	}

	virtual AString GetStepSound(void) override
	{
		return "step.grass";
	}
};
