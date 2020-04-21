
#pragma once

#include "BlockHandler.h"





/** Handler for the small (singleblock) mushrooms. */
class cBlockMushroomHandler:
	public cClearMetaOnDrop<cBlockHandler>
{
	using Super = cClearMetaOnDrop<cBlockHandler>;

public:

	cBlockMushroomHandler(BLOCKTYPE a_BlockType):
		Super(a_BlockType)
	{
	}





	// TODO: Add Mushroom Spread





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) override
	{
		if (a_RelPos.y <= 0)
		{
			return false;
		}

		// TODO: Cannot be at too much daylight

		switch (a_Chunk.GetBlock(a_RelPos.addedY(-1)))
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




