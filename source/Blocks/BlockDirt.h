
#pragma once

#include "BlockHandler.h"
#include "../MersenneTwister.h"
#include "../World.h"





/// Handler used for both dirt and grass
class cBlockDirtHandler :
	public cBlockHandler
{
public:
	cBlockDirtHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}

	
	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_ITEM_DIRT, 1, 0));
	}
	
	
	void OnUpdate(cWorld * a_World, int a_X, int a_Y, int a_Z) override
	{
		if (m_BlockID != E_BLOCK_GRASS)
		{
			return;
		}
		
		// Grass becomes dirt if there is something on top of it:
		BLOCKTYPE Above = a_World->GetBlock(a_X, a_Y + 1, a_Z);
		if (!g_BlockTransparent[Above] && !g_BlockOneHitDig[Above])
		{
			a_World->FastSetBlock(a_X, a_Y, a_Z, E_BLOCK_DIRT, 0);
			return;
		}
		
		// Grass spreads to adjacent blocks:
		MTRand rand;
		for (int i = 0; i < 2; i++)  // Pick two blocks to grow to
		{
			int OfsX = rand.randInt(2) - 1;  // [-1 .. 1]
			int OfsY = rand.randInt(4) - 3;  // [-3 .. 1]
			int OfsZ = rand.randInt(2) - 1;  // [-1 .. 1]
	
			BLOCKTYPE  DestBlock;
			NIBBLETYPE DestMeta;
			a_World->GetBlockTypeMeta(a_X + OfsX, a_Y + OfsY, a_Z + OfsZ, DestBlock, DestMeta);
			if(DestBlock != E_BLOCK_DIRT)
			{
				continue;
			}

			BLOCKTYPE AboveDest;
			NIBBLETYPE AboveMeta;
			a_World->GetBlockTypeMeta(a_X + OfsX, a_Y + OfsY + 1, a_Z + OfsZ, AboveDest, AboveMeta);
			if (g_BlockOneHitDig[AboveDest] || g_BlockTransparent[AboveDest])
			{
				a_World->FastSetBlock(a_X + OfsX, a_Y + OfsY, a_Z + OfsZ, E_BLOCK_GRASS, 0);
			}
		}  // for i - repeat twice
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.gravel";
	}
} ;




