
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

	virtual bool IsPlaceable(void) override
	{
		return true;
	}


	virtual bool OnPlayerPlace(
		cWorld & a_World,
		cPlayer & a_Player,
		const cItem & a_EquippedItem,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos
	) override
	{
		if (!Super::OnPlayerPlace(a_World, a_Player, a_EquippedItem, a_ClickedBlockPos, a_ClickedBlockFace, a_CursorPos))
		{
			return false;
		}

		if (a_EquippedItem.IsCustomNameEmpty())
		{
			return true;
		}

		const auto PlacePos = AddFaceDirection(a_ClickedBlockPos, a_ClickedBlockFace);
		a_World.DoWithBlockEntityAt(PlacePos, [&a_EquippedItem](cBlockEntity & a_Entity)
		{
			ASSERT(a_Entity.GetBlockType() == BlockType::EnchantingTable);

			static_cast<cEnchantingTableEntity &>(a_Entity).SetCustomName(a_EquippedItem.m_CustomName);
			return false;
		});

		return true;
	}
} ;
