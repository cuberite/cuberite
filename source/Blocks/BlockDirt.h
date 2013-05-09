
#pragma once

#include "BlockHandler.h"
#include "../MersenneTwister.h"
#include "../World.h"





/// Handler used for both dirt and grass
class cBlockDirtHandler :
	public cBlockHandler
{
public:
	cBlockDirtHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	
	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_BLOCK_DIRT, 1, 0));
	}
	
	
	virtual void OnUpdate(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		if (m_BlockType != E_BLOCK_GRASS)
		{
			return;
		}
		
		// Grass becomes dirt if there is something on top of it:
		if (a_BlockY < cChunkDef::Height - 1)
		{
			BLOCKTYPE Above = a_World->GetBlock(a_BlockX, a_BlockY + 1, a_BlockZ);
			if (!g_BlockTransparent[Above] && !g_BlockOneHitDig[Above])
			{
				a_World->FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_DIRT, 0);
				return;
			}
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
			if ((a_BlockY + OfsY < 0) || (a_BlockY + OfsY >= cChunkDef::Height - 1))
			{
				// Y Coord out of range
				continue;
			}
			bool IsValid = a_World->GetBlockTypeMeta(a_BlockX + OfsX, a_BlockY + OfsY, a_BlockZ + OfsZ, DestBlock, DestMeta);
			if (!IsValid || (DestBlock != E_BLOCK_DIRT))
			{
				continue;
			}

			BLOCKTYPE AboveDest;
			NIBBLETYPE AboveMeta;
			IsValid = a_World->GetBlockTypeMeta(a_BlockX + OfsX, a_BlockY + OfsY + 1, a_BlockZ + OfsZ, AboveDest, AboveMeta);
			ASSERT(IsValid);  // WTF - how did we get the DestBlock if AboveBlock is not valid?
			if (g_BlockOneHitDig[AboveDest] || g_BlockTransparent[AboveDest])
			{
				a_World->FastSetBlock(a_BlockX + OfsX, a_BlockY + OfsY, a_BlockZ + OfsZ, E_BLOCK_GRASS, 0);
			}
		}  // for i - repeat twice
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.gravel";
	}
} ;




