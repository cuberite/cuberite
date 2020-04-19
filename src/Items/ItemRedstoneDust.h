
#pragma once

#include "ItemHandler.h"





class cItemRedstoneDustHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemRedstoneDustHandler(int a_ItemType):
		Super(a_ItemType)
	{
	}





	virtual bool IsPlaceable(void) override
	{
		return true;
	}





	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		// Check the block below, if it supports dust on top of it:
		auto PlacePos = AddFaceDirection(a_ClickedBlockPos, a_ClickedBlockFace, true);
		auto UnderPos = PlacePos.addedY(-1);
		BLOCKTYPE BlockType;
		NIBBLETYPE BlockMeta;
		if (!a_World->GetBlockTypeMeta(UnderPos, BlockType, BlockMeta))
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




