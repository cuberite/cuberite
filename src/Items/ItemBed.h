
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Blocks/BlockBed.h"





class cItemBedHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemBedHandler(int a_ItemType):
		Super(a_ItemType)
	{
	}





	virtual bool IsPlaceable(void) override
	{
		return true;
	}





	virtual bool GetBlocksToPlace(
		cWorld & a_World,
		cPlayer & a_Player,
		const cItem & a_EquippedItem,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		sSetBlockVector & a_BlocksToPlace
	) override
	{
		// Can only be placed on the floor:
		if (a_ClickedBlockFace != BLOCK_FACE_TOP)
		{
			return false;
		}

		// The "foot" block:
		NIBBLETYPE BlockMeta = cBlockBedHandler::YawToMetaData(a_Player.GetYaw());
		a_BlocksToPlace.emplace_back(a_PlacedBlockPos, E_BLOCK_BED, BlockMeta);

		// Check if there is empty space for the "head" block:
		// (Vanilla only allows beds to be placed into air)
		auto Direction = cBlockBedHandler::MetaDataToDirection(BlockMeta);
		auto HeadPos = a_PlacedBlockPos + Direction;
		if (a_World.GetBlock(HeadPos) != E_BLOCK_AIR)
		{
			return false;
		}
		a_BlocksToPlace.emplace_back(HeadPos, E_BLOCK_BED, BlockMeta | 0x08);
		return true;
	}
};
