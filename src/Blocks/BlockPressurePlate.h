
#pragma once

#include "BlockHandler.h"




class cBlockPressurePlateHandler :
	public cBlockHandler
{
public:
	cBlockPressurePlateHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to zero
		a_Pickups.push_back(cItem(m_BlockType, 1, 0));
	}

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk, NIBBLETYPE a_BlockMeta) override
	{
		if (a_RelY - 1 <= 0)
		{
			return false;
		}

		BLOCKTYPE Block;
		NIBBLETYPE BlockMeta;
		a_Chunk.GetBlockTypeMeta(a_RelX, a_RelY - 1, a_RelZ, Block, BlockMeta);

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
				// On the top of an upside-down slab
				if (cBlockSlabHandler::IsAnySlabType(Block))
				{
					// Check if the slab is turned up side down
					return (cBlockSlabHandler::IsUpsideDown(BlockMeta));
				}

				// On the top of an upside-down stairs
				if (cBlockStairsHandler::IsAnyStairType(Block))
				{
					return (cBlockStairsHandler::IsUpsideDown(BlockMeta));
				}

				return cBlockInfo::IsFullSolidOpaqueBlock(Block);
			}
		}
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
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

	/** Returns true if the specified block type is one of the pressure plate handled by this handler */
	static bool IsAnyPressurePlateType(BLOCKTYPE a_BlockType)
	{
		return (
			(a_BlockType == E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE) ||
			(a_BlockType == E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE) ||
			(a_BlockType == E_BLOCK_STONE_PRESSURE_PLATE) ||
			(a_BlockType == E_BLOCK_WOODEN_PRESSURE_PLATE)
		);
	}
} ;




