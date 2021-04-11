
#pragma once

#include "ItemHandler.h"
#include "../BlockEntities/EnchantingTableEntity.h"
#include "../World.h"





class cItemEnchantingTableHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

private:

	virtual bool OnPlacementCommit(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		if (!Super::OnPlacementCommit(a_Player, a_HeldItem, a_PlacePosition, a_ClickedBlockFace, a_CursorPosition))
		{
			return false;
		}

		if (a_HeldItem.IsCustomNameEmpty())
		{
			return true;
		}

		a_Player.GetWorld()->DoWithBlockEntityAt(a_PlacePosition, [&a_HeldItem](cBlockEntity & a_BlockEntity)
		{
			ASSERT(a_BlockEntity.GetBlockType() == E_BLOCK_ENCHANTMENT_TABLE);

			static_cast<cEnchantingTableEntity &>(a_BlockEntity).SetCustomName(a_HeldItem.m_CustomName);
			return false;
		});

		return true;
	}


	virtual bool IsPlaceable(void) override
	{
		return true;
	}
} ;
