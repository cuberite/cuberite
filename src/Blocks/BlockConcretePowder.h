
#pragma once

#include "BlockHandler.h"





class cBlockConcretePowderHandler :
	public cBlockHandler
{
public:
	cBlockConcretePowderHandler(BLOCKTYPE a_BlockType):
		cBlockHandler(a_BlockType)
	{
	}





	virtual void Check(cChunkInterface & a_ChunkInterface, cBlockPluginInterface & a_PluginInterface, int a_RelX, int a_RelY, int a_RelZ, cChunk & a_Chunk) override
	{
		if (GetSoaked(Vector3i(a_RelX, a_RelY, a_RelZ), a_Chunk))
		{
			return;
		}
		cBlockHandler::Check(a_ChunkInterface, a_PluginInterface, a_RelX, a_RelY, a_RelZ, a_Chunk);
	}





	/** Check blocks above and around to see if they are water. If one is, convert this into concrete block.
		Returns TRUE if the block was changed. */
	bool GetSoaked(Vector3i a_Rel, cChunk & a_Chunk)
	{
		static const std::array<Vector3i, 5> WaterCheck
		{
			{
				{ 1, 0,  0},
				{-1, 0,  0},
				{ 0, 0,  1},
				{ 0, 0, -1},
				{ 0, 1,  0},
			}
		};

		bool ShouldSoak = std::any_of(WaterCheck.cbegin(), WaterCheck.cend(), [a_Rel, & a_Chunk](Vector3i a_Offset)
			{
				BLOCKTYPE NeighborType;
				return (
					a_Chunk.UnboundedRelGetBlockType(a_Rel.x + a_Offset.x, a_Rel.y + a_Offset.y, a_Rel.z + a_Offset.z, NeighborType)
					&& IsBlockWater(NeighborType)
				);
			}
		);

		if (ShouldSoak)
		{
			NIBBLETYPE BlockMeta;
			BlockMeta = a_Chunk.GetMeta(a_Rel.x, a_Rel.y, a_Rel.z);
			a_Chunk.SetBlock(a_Rel.x, a_Rel.y, a_Rel.z, E_BLOCK_CONCRETE, BlockMeta);
			return true;
		}
		return false;
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		switch (a_Meta)
		{
			case E_META_CONCRETE_POWDER_WHITE: return 8;
			case E_META_CONCRETE_POWDER_ORANGE: return 15;
			case E_META_CONCRETE_POWDER_MAGENTA: return 16;
			case E_META_CONCRETE_POWDER_LIGHTBLUE: return 17;
			case E_META_CONCRETE_POWDER_YELLOW: return 18;
			case E_META_CONCRETE_POWDER_LIGHTGREEN: return 19;
			case E_META_CONCRETE_POWDER_PINK: return 20;
			case E_META_CONCRETE_POWDER_GRAY: return 21;
			case E_META_CONCRETE_POWDER_LIGHTGRAY: return 22;
			case E_META_CONCRETE_POWDER_CYAN: return 23;
			case E_META_CONCRETE_POWDER_PURPLE: return 24;
			case E_META_CONCRETE_POWDER_BLUE: return 25;
			case E_META_CONCRETE_POWDER_BROWN: return 26;
			case E_META_CONCRETE_POWDER_GREEN: return 27;
			case E_META_CONCRETE_POWDER_RED: return 28;
			case E_META_CONCRETE_POWDER_BLACK: return 29;
			default:
			{
				ASSERT(!"Unhandled meta in concrete powder handler!");
				return 0;
			}
		}
	}
};




