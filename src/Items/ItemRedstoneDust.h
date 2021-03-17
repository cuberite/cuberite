
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
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BlockState & a_Block
	) override
	{
		// Check the block below, if it supports dust on top of it:
		auto UnderPos = a_PlacedBlockPos.addedY(-1);
		if (UnderPos.y < 0)
		{
			return false;
		}
		BlockState BlockBelow;
		if (!a_World->GetBlock(UnderPos, BlockBelow))
		{
			return false;
		}
		if (!IsBlockTypeUnderSuitable(BlockBelow))
		{
			return false;
		}

		a_Block = Block::RedstoneWire::RedstoneWire();
		return true;
	}





	/** Returns true if the specified block type / meta is suitable to have redstone dust on top of it. */
	static bool IsBlockTypeUnderSuitable(BlockState a_Block)
	{
		if (cBlockInfo::FullyOccupiesVoxel(a_Block))
		{
			return true;
		}

		if (cBlockSlabHandler::IsAnySlabType(a_Block) && cBlockSlabHandler::IsSlabTop(a_Block))
		{
			return true;
		}
		return false;
	}
} ;




