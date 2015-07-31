
#pragma once

#include "BlockHandler.h"
#include "Entities/Pickup.h"




class cBlockLilypadHandler :
	public cClearMetaOnDrop<cBlockHandler>
{
	typedef cClearMetaOnDrop<cBlockHandler> super;
public:

	cBlockLilypadHandler(BLOCKTYPE a_BlockType) :
		super(a_BlockType)
	{
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 7;
	}
	
	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		return ((a_RelY > 0) && (a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ) == E_BLOCK_STATIONARY_WATER));
	}
};




