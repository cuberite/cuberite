
#pragma once

#include "ItemHandler.h"
#include "Blocks/BlockBed.h"
#include "BlockEntities/BedEntity.h"





class cItemBedHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemBedHandler(int a_ItemType):
		Super(a_ItemType)
	{
	}


	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		const auto BlockMeta = cBlockBedHandler::YawToMetaData(a_Player.GetYaw());
		const auto HeadPosition = a_PlacePosition + cBlockBedHandler::MetaDataToDirection(BlockMeta);

		auto & World = *a_Player.GetWorld();
		BLOCKTYPE HeadType;
		NIBBLETYPE HeadMeta;
		World.GetBlockTypeMeta(HeadPosition, HeadType, HeadMeta);

		// Vanilla only allows beds to be placed into air.
		// Check if there is empty space for the "head" block:
		if (!cBlockHandler::For(HeadType).DoesIgnoreBuildCollision(World, a_HeldItem, HeadPosition, HeadMeta, a_ClickedBlockFace, false))
		{
			return false;
		}

		// The "foot", and the "head" block:
		if (
			!a_Player.PlaceBlocks(
			{
				{ a_PlacePosition, E_BLOCK_BED, BlockMeta },
				{ HeadPosition, E_BLOCK_BED, static_cast<NIBBLETYPE>(BlockMeta | 0x08) }
			})
		)
		{
			return false;
		}

		auto SetColor = [&a_HeldItem](cBlockEntity & a_BlockEntity)
		{
			ASSERT(a_BlockEntity.GetBlockType() == E_BLOCK_BED);

			static_cast<cBedEntity &>(a_BlockEntity).SetColor(a_HeldItem.m_ItemDamage);
			return false;
		};
		World.DoWithBlockEntityAt(a_PlacePosition, SetColor);
		World.DoWithBlockEntityAt(HeadPosition, SetColor);

		return true;
	}


	virtual bool IsPlaceable(void) override
	{
		return true;
	}
};
