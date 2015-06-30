
#pragma once

#include "ItemHandler.h"





class cItemRedstoneDustHandler : public cItemHandler
{
public:
	cItemRedstoneDustHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{
	}

	virtual bool IsPlaceable(void) override
	{
		return true;
	}

	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		// Check if coords are out of range:
		if ((a_BlockY <= 0) || (a_BlockY >= cChunkDef::Height))
		{
			return false;
		}

		// Check the block below, if it supports dust on top of it:
		BLOCKTYPE BlockType;
		NIBBLETYPE BlockMeta;
		if (!a_World->GetBlockTypeMeta(a_BlockX, a_BlockY - 1, a_BlockZ, BlockType, BlockMeta))
		{
			return false;
		}
		if (!IsBlockTypeUnderSuitable(BlockType, BlockMeta))
		{
			return false;
		}

		a_BlockType = E_BLOCK_REDSTONE_WIRE;
		a_BlockMeta = 0;
		return true;
	}


	/** Returns true if the specified block type / meta is suitable to have redstone dust on top of it. */
	static bool IsBlockTypeUnderSuitable(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
	{
		if (cBlockInfo::FullyOccupiesVoxel(a_BlockType))
		{
			return true;
		}

		switch (a_BlockType)
		{
			case E_BLOCK_RED_SANDSTONE_SLAB:
			case E_BLOCK_WOODEN_SLAB:
			case E_BLOCK_STONE_SLAB:
			{
				// Slabs can support redstone if they're upside down:
				return ((a_BlockMeta & 0x08) != 0);
			}
		}
		return false;
	}
} ;




