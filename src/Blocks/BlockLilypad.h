
#pragma once

#include "BlockHandler.h"





class cBlockLilypadHandler final :
	public cClearMetaOnDrop<cBlockHandler>
{
	using Super = cClearMetaOnDrop<cBlockHandler>;

public:

	using Super::Super;

private:

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 7;
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
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




