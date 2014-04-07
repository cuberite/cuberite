#pragma once
#include "BlockHandler.h"
#include "BlockLeaves.h"
#include "../World.h"






class cBlockNewLeavesHandler :
	public cBlockLeavesHandler
{
public:
	cBlockNewLeavesHandler(BLOCKTYPE a_BlockType)
		: cBlockLeavesHandler(a_BlockType)
	{
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		MTRand rand;

		// Only the first 2 bits contain the display information, the others are for growing
		if (rand.randInt(5) == 0)
		{
			a_Pickups.push_back(cItem(E_BLOCK_SAPLING, 1, (a_BlockMeta & 3) + 4));
		}
	}


	void OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		cBlockHandler::OnDestroyed(a_ChunkInterface, a_WorldInterface, a_BlockX, a_BlockY, a_BlockZ);
	}
} ;





