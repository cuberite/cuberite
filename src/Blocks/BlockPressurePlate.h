
#pragma once

#include "BlockHandler.h"




class cBlockPressurePlateHandler final :
	public cClearMetaOnDrop<cBlockHandler>
{
	using Super = cClearMetaOnDrop<cBlockHandler>;

public:

	using Super::Super;

private:

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		if (a_RelPos.y <= 1)
		{
			return false;
		}

		// TODO: check if the block is upside-down slab or upside-down stairs
		auto Block = a_Chunk.GetBlock(a_RelPos.addedY(-1));
		switch (Block)
		{
			case E_BLOCK_ACACIA_FENCE:
			case E_BLOCK_BIRCH_FENCE:
			case E_BLOCK_DARK_OAK_FENCE:
			case E_BLOCK_FENCE:
			case E_BLOCK_HOPPER:
			case E_BLOCK_JUNGLE_FENCE:
			case E_BLOCK_NETHER_BRICK_FENCE:
			case E_BLOCK_SPRUCE_FENCE:
			{
				return true;
			}
			default:
			{
				return (!cBlockInfo::IsTransparent(Block));
			}
		}
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		switch (m_BlockType)
		{
			case E_BLOCK_STONE_PRESSURE_PLATE: return 11;
			case E_BLOCK_WOODEN_PRESSURE_PLATE: return 13;
			case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE: return 6;
			case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE: return 30;
			default:
			{
				ASSERT(!"Unhandled blocktype in pressure plate handler!");
				return 0;
			}
		}
	}
} ;




