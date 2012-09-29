#pragma once
#include "BlockHandler.h"
#include "../MersenneTwister.h"
#include "../World.h"

class cBlockStemsHandler : public cBlockHandler
{
public:
	cBlockStemsHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}

	virtual bool NeedsRandomTicks() override
	{
		return true;
	}

	virtual NIBBLETYPE GetDropMeta(NIBBLETYPE a_BlockMeta) override
	{
		return 0;
	}

	virtual int GetDropID() override
	{
		if(m_BlockID == E_BLOCK_MELON_STEM)
			return E_ITEM_MELON_SEEDS;
		return E_ITEM_PUMPKIN_SEEDS;
	}
	
	void OnUpdate(cWorld *a_World, int a_X, int a_Y, int a_Z) override
	{
		//TODO: Handle Growing here
	}

	virtual bool CanBeAt(cWorld *a_World, int a_X, int a_Y, int a_Z) override
	{
		return a_World->GetBlock(a_X, a_Y - 1, a_Z) == E_BLOCK_FARMLAND;
	}

	virtual AString GetStepSound(void) override
	{
		return "step.wood";
	}
	
};
