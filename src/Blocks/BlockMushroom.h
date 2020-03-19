
#pragma once

#include "BlockHandler.h"





class cBlockMushroomHandler:
	public cClearMetaOnDrop<cBlockHandler>
{
	using super = cClearMetaOnDrop<cBlockHandler>;

public:

	cBlockMushroomHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}

	// TODO: Add Mushroom Spread

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk, NIBBLETYPE a_BlockMeta) override
	{
		if (a_RelY <= 0)
		{
			return false;
		}

		/** Mushrooms could be placed on any solid, non-transparent, block
		including pumpkins, workbenches, furnaces, and chests */

		// TODO: Cannot be at too much daylight
		BLOCKTYPE UnderType = a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ);
		switch (UnderType)
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
			case E_BLOCK_PUMPKIN:
			case E_BLOCK_WORKBENCH:
			case E_BLOCK_FURNACE:
			case E_BLOCK_CHEST:
			{
				return true;
			}
			default:
			{
				return (cBlockInfo::IsSolid(UnderType) && (!cBlockInfo::IsTransparent(UnderType)));
			}
		}

	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;




