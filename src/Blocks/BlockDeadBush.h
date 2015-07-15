
#pragma once

#include "BlockHandler.h"





class cBlockDeadBushHandler :
	public cBlockHandler
{
public:
	cBlockDeadBushHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		if (a_RelY <= 0)
		{
			return false;
		}

		BLOCKTYPE BelowBlock = a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ);
		switch (BelowBlock)
		{
			case E_BLOCK_CLAY:
			case E_BLOCK_HARDENED_CLAY:
			case E_BLOCK_STAINED_CLAY:
			case E_BLOCK_SAND:
			{
				return true;
			}
			default: return false;
		}
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;




