
#pragma once

#include "BlockHandler.h"





class cBlockLilypadHandler:
	public cClearMetaOnDrop<cBlockHandler>
{
	using Super = cClearMetaOnDrop<cBlockHandler>;

public:

	cBlockLilypadHandler(BLOCKTYPE a_BlockType):
		Super(a_BlockType)
	{
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 7;
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) override
	{
		auto UnderPos = a_RelPos.addedY(-1);
		if (!cChunkDef::IsValidHeight(UnderPos.y))
		{
			return false;
		}
		BLOCKTYPE UnderType;
		NIBBLETYPE UnderMeta;
		a_Chunk.GetBlockTypeMeta(UnderPos, UnderType, UnderMeta);
		return (
			(((UnderType == E_BLOCK_STATIONARY_WATER) || (UnderType == E_BLOCK_WATER)) && (UnderMeta == 0)) ||  // A water source is below
			(UnderType == E_BLOCK_ICE) || (UnderType == E_BLOCK_FROSTED_ICE)                                    // Or (frosted) ice
		);
	}
};




