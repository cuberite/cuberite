
#pragma once

#include "BlockHandler.h"





class cBlockTallGrassHandler :
	public cBlockHandler
{
public:
	cBlockTallGrassHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}


	virtual bool DoesIgnoreBuildCollision(void) override
	{
		return true;
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Drop seeds, sometimes
		cFastRandom Random;
		if (Random.NextInt(8) == 0)
		{
			a_Pickups.push_back(cItem(E_ITEM_SEEDS, 1, 0));
		}
	}


	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		if (a_RelY <= 0)
		{
			return false;
		}

		BLOCKTYPE BelowBlock = a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ);
		return IsBlockTypeOfDirt(BelowBlock);
	}
} ;




