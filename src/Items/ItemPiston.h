
#pragma once

#include "ItemHandler.h"
#include "Blocks/BlockPiston.h"





class cItemPistonHandler final:
	public cSimplePlaceableItemHandler
{
	using Super = cSimplePlaceableItemHandler;

public:

	using Super::Super;

private:

	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		switch (BlockItemConverter::FromItem(a_HeldItem.m_ItemType))
		{
			case BlockType::Piston:       return a_Player.PlaceBlock(a_PlacePosition, Block::Piston::Piston(false, DisplacementYawToFacing(a_PlacePosition, a_Player.GetEyePosition(), a_Player.GetYaw())));
			case BlockType::StickyPiston: return a_Player.PlaceBlock(a_PlacePosition, Block::StickyPiston::StickyPiston(false, DisplacementYawToFacing(a_PlacePosition, a_Player.GetEyePosition(), a_Player.GetYaw())));
			default: return false;
		}
	}
};
