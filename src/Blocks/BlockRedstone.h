
#pragma once

#include "BlockHandler.h"
#include "../World.h"





class cBlockRedstoneHandler :
	public cClearMetaOnDrop<cBlockHandler>
{
public:
	cBlockRedstoneHandler(BLOCKTYPE a_BlockType)
		: cClearMetaOnDrop<cBlockHandler>(a_BlockType)
	{
	}
	

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		return ((a_RelY > 0) && cBlockInfo::FullyOccupiesVoxel(a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ)));
	}
} ;




