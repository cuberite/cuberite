
#pragma once

#include "BlockHandler.h"
#include "../World.h"





class cBlockRedstoneHandler :
	public cBlockHandler
{
public:
	cBlockRedstoneHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}
	

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		return ((a_RelY > 0) && g_BlockFullyOccupiesVoxel[a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ)]);
	}
	
	
	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to 0
		a_Pickups.push_back(cItem(E_ITEM_REDSTONE_DUST, 1));
	}
} ;




