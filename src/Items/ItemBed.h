
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
		const auto BlockMeta = cBlockBedHandler::YawToMetaData(a_Player.GetYaw());
		const auto HeadPosition = a_PlacedBlockPos + cBlockBedHandler::MetaDataToDirection(BlockMeta);

		// Vanilla only allows beds to be placed into air
		// Check if there is empty space for the "head" block:
		if (a_World.GetBlock(HeadPosition) != E_BLOCK_AIR)
		{
			return false;
		}

		// The "foot", and the "head" block:
		a_BlocksToPlace.emplace_back(a_PlacedBlockPos, E_BLOCK_BED, BlockMeta);
		a_BlocksToPlace.emplace_back(HeadPosition, E_BLOCK_BED, BlockMeta | 0x08);
		return true;
	}
};
