
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

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		if (a_RelY <= 0)
		{
			return false;
		}

		return (cBlockInfo::FullyOccupiesVoxel(a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ)));
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
} ;




