#pragma once
#include "Block.h"
#include "../MersenneTwister.h"
#include "../cWorld.h"

class cBlockCropsHandler : public cBlockHandler
{
public:
	cBlockCropsHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}

	virtual bool NeedsRandomTicks() override
	{
		return true;
	}

	virtual bool AllowBlockOnTop() override
	{
		return false;
	}

	virtual int GetDropID() override
	{
		return E_ITEM_EMPTY;
	}
	
	virtual void OnDestroyed(cWorld *a_World, int a_X, int a_Y, int a_Z) override
	{
		MTRand rand;
		NIBBLETYPE Meta = a_World->GetBlockMeta(a_X, a_Y, a_Z);

		cItems Drops;

		if(Meta & 0x7)	//Is Wheat
		{
			Drops.push_back(cItem(E_ITEM_WHEAT, 1, 0));
		}
		if(rand.randInt(3) == 0)
		{	//Drop an second seed
			Drops.push_back(cItem(E_ITEM_SEEDS, 1, 0));
		}
		Drops.push_back(cItem(E_ITEM_SEEDS, 1, 0));
		a_World->SpawnItemPickups(Drops, a_X, a_Y, a_Z);
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
		return "step.grass";
	}
	
};
