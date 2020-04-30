
#pragma once

#include "BlockHandler.h"





/** Handler for the small (singleblock) mushrooms. */
class cBlockMushroomHandler:
	public cClearMetaOnDrop<cBlockHandler>
{
	using Super = cClearMetaOnDrop<cBlockHandler>;

public:
	const NIBBLETYPE MAXIMUM_LIGHT_THRESHOLD = 12;


	cBlockMushroomHandler(BLOCKTYPE a_BlockType):
		Super(a_BlockType)
	{
	}





	// TODO: Add Mushroom Spread





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) override
	{
		if (!cChunkDef::IsValidHeight(a_RelPos.y)) return false;

		// Cannot be at too much light
		if (a_Chunk.GetBlockLight(a_RelPos) > MAXIMUM_LIGHT_THRESHOLD) return false;

		Vector3i BelowPos = a_RelPos.addedY(-1);
		BLOCKTYPE BelowBlock;
		if (!a_Chunk.UnboundedRelGetBlockType(BelowPos, BelowBlock)) return false;

		switch (BelowBlock)
		{
			case E_BLOCK_GLASS:
			case E_BLOCK_CACTUS:
			case E_BLOCK_ICE:
			case E_BLOCK_LEAVES:
			case E_BLOCK_NEW_LEAVES:
			case E_BLOCK_AIR:
			{
				return false;
			}
		}
		return true;
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;




