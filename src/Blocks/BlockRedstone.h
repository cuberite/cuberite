
#pragma once

#include "BlockHandler.h"
#include "../World.h"





class cBlockRedstoneHandler :
	public cBlockHandler
{
public:
	cBlockRedstoneHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}
	

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		NIBBLETYPE Meta;
		a_Chunk.UnboundedRelGetBlockMeta(a_RelX, a_RelY, a_RelZ, Meta);

		BLOCKTYPE BlockIsOn;
		a_Chunk.UnboundedRelGetBlockType(a_RelX, a_RelY, a_RelZ, BlockIsOn);

		if (
			(
				((BlockIsOn == E_BLOCK_WOODEN_SLAB) && ((Meta & 0x08) == 0x08)) ||
				((BlockIsOn == E_BLOCK_STONE_SLAB) && ((Meta & 0x08) == 0x08))
			) &&
			(a_RelY > 0)
		)
		{
			return true;
		}
		else
		{
			return ((a_RelY > 0) && cBlockInfo::FullyOccupiesVoxel(a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ)));
		}
	}
	
	
	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to zero
		a_Pickups.push_back(cItem(E_ITEM_REDSTONE_DUST, 1, 0));
	}
} ;




