
// BlockCarpet.h

// Declares the cBlockCarpetHandler class representing the handler for the carpet block




#pragma once

#include "BlockHandler.h"





class cBlockCarpetHandler :
	public cBlockHandler
{
public:
	cBlockCarpetHandler(BLOCKTYPE a_BlockType) :
		cBlockHandler(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_BLOCK_CARPET, 1, a_BlockMeta));
	}

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk, NIBBLETYPE a_BlockMeta) override
	{
		return (a_RelY > 0) && (a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ) != E_BLOCK_AIR);
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		switch (a_Meta)
		{
			case E_META_CARPET_WHITE: return 14;
			case E_META_CARPET_ORANGE: return 15;
			case E_META_CARPET_MAGENTA: return 16;
			case E_META_CARPET_LIGHTBLUE: return 17;
			case E_META_CARPET_YELLOW: return 18;
			case E_META_CARPET_LIGHTGREEN: return 19;
			case E_META_CARPET_PINK: return 20;
			case E_META_CARPET_GRAY: return 21;
			case E_META_CARPET_LIGHTGRAY: return 22;
			case E_META_CARPET_CYAN: return 23;
			case E_META_CARPET_PURPLE: return 24;
			case E_META_CARPET_BLUE: return 25;
			case E_META_CARPET_BROWN: return 26;
			case E_META_CARPET_GREEN: return 27;
			case E_META_CARPET_RED: return 28;
			case E_META_CARPET_BLACK: return 29;
			default:
			{
				ASSERT(!"Unhandled meta in carpet handler!");
				return 0;
			}
		}
	}
} ;




