
#pragma once

#include "BlockHandler.h"





class cBlockConcretePowderHandler :
	public cBlockHandler
{
public:
	cBlockConcretePowderHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}





	virtual void OnPlaced(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override
	{
		GetSoaked(Vector3i(a_BlockX, a_BlockY, a_BlockZ), a_BlockMeta, a_ChunkInterface);
	}





	/** Check blocks above and around to see if they are water. If one is, convert this into concrete block */
	void GetSoaked(Vector3i a_Rel, NIBBLETYPE a_BlockMeta, cChunkInterface & a_Chunk)
	{
		bool ShouldSoak = false;

		static const Vector3i Coords[] =
		{
			Vector3i( 1, 0,  0),
			Vector3i(-1, 0,  0),
			Vector3i( 0, 0,  1),
			Vector3i( 0, 0, -1),
			Vector3i( 0, 1,  0),
		};
		for (size_t i = 0; i < ARRAYCOUNT(Coords); i++)
		{
			if (IsBlockWater(a_Chunk.GetBlock({a_Rel.x + Coords[i].x, a_Rel.y + Coords[i].y, a_Rel.z + Coords[i].z})))
			{
				ShouldSoak = true;
				continue;
			}
		}  // for i - Coords[]

		if (ShouldSoak)
		{
			a_Chunk.SetBlock(a_Rel.x, a_Rel.y, a_Rel.z, E_BLOCK_CONCRETE, a_BlockMeta);
		}
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




