
#pragma once

#include "ItemHandler.h"
#include "../BlockEntities/EnchantingTableEntity.h"
#include "../World.h"





class cItemEnchantingTableHandler final:
	public cSimplePlaceableItemHandler
{
	using Super = cSimplePlaceableItemHandler;

public:

	using Super::Super;

private:

	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		if (!a_Player.PlaceBlock(a_PlacePosition, Block::EnchantingTable::EnchantingTable()))
		{
			return false;
		}

		if (a_HeldItem.IsCustomNameEmpty())
		{
			return true;
		}

		a_Player.GetWorld()->DoWithBlockEntityAt(a_PlacePosition, [&a_HeldItem](cBlockEntity & a_BlockEntity)
		{
			ASSERT(a_BlockEntity.GetBlockType() == BlockType::EnchantingTable);

			static_cast<cEnchantingTableEntity &>(a_BlockEntity).SetCustomName(a_HeldItem.m_CustomName);
			return false;
		});

		return true;
	}
} ;
