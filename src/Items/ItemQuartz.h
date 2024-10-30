
#pragma once

#include "ItemHandler.h"





class cItemQuartzHandler final:
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
			case BlockType::QuartzBlock:  return a_Player.PlaceBlock(a_PlacePosition, Block::QuartzBlock::QuartzBlock());
			case BlockType::QuartzPillar:
			{
				switch (a_ClickedBlockFace)
				{
					case BLOCK_FACE_XM:
					case BLOCK_FACE_XP:
					{
						return a_Player.PlaceBlock(a_PlacePosition, Block::QuartzPillar::QuartzPillar(Block::QuartzPillar::Axis::X));
					}
					case BLOCK_FACE_YM:
					case BLOCK_FACE_YP:
					{
						return a_Player.PlaceBlock(a_PlacePosition, Block::QuartzPillar::QuartzPillar(Block::QuartzPillar::Axis::Y));
					}
					case BLOCK_FACE_ZM:
					case BLOCK_FACE_ZP:
					{
						return a_Player.PlaceBlock(a_PlacePosition, Block::QuartzPillar::QuartzPillar(Block::QuartzPillar::Axis::Z));
					}
					default: return false;
				}
			}
			default: return false;
		}
	}
};
