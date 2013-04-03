#pragma once
#include "BlockHandler.h"
#include "../MersenneTwister.h"
#include "../World.h"

class cBlockCropsHandler : public cBlockHandler
{
public:
	cBlockCropsHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}


	virtual bool DoesAllowBlockOnTop() override
	{
		return false;
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_Meta) override
	{
		MTRand rand;

		if (a_Meta == 0x7)  // Is fully grown
		{
			a_Pickups.push_back(cItem(E_ITEM_WHEAT, 1, 0));
		}
		a_Pickups.push_back(cItem(E_ITEM_SEEDS, (rand.randInt(3) == 0) ? 2 : 1, 0));
	}	
	
	
	void OnUpdate(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		NIBBLETYPE Meta = a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		if (Meta < 7)
		{
			a_World->FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_CROPS, ++Meta);
		}
	}


	virtual bool CanBeAt(int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		return ((a_RelY > 0) && (a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ) == E_BLOCK_FARMLAND));
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.grass";
	}
} ;




