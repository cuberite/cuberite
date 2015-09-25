
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
		if ((a_RelY < 1) || (a_RelY >= cChunkDef::Height))
		{
			return false;
		}
		BLOCKTYPE UnderType;
		NIBBLETYPE UnderMeta;
		a_Chunk.GetBlockTypeMeta(a_RelX, a_RelY - 1, a_RelZ, UnderType, UnderMeta);
		return (
			((UnderType == E_BLOCK_STATIONARY_WATER) || (UnderType == E_BLOCK_WATER)) &&  // Water is below...
			(UnderMeta == 0)                                                              // ... and it's a source
		);
	}
};




