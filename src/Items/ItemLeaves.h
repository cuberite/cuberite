
#pragma once

#include "ItemHandler.h"
#include "Blocks/BlockLeaves.h"





class cItemLeavesHandler final :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;





	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		switch (BlockItemConverter::FromItem(a_HeldItem.m_ItemType))
		{
			case BlockType::AcaciaLeaves:  return a_Player.PlaceBlock(a_PlacePosition, Block::AcaciaLeaves::AcaciaLeaves(LEAVES_CHECK_DISTANCE, true));
			case BlockType::BirchLeaves:   return a_Player.PlaceBlock(a_PlacePosition, Block::BirchLeaves::BirchLeaves(LEAVES_CHECK_DISTANCE, true));
			case BlockType::DarkOakLeaves: return a_Player.PlaceBlock(a_PlacePosition, Block::DarkOakLeaves::DarkOakLeaves(LEAVES_CHECK_DISTANCE, true));
			case BlockType::JungleLeaves:  return a_Player.PlaceBlock(a_PlacePosition, Block::JungleLeaves::JungleLeaves(LEAVES_CHECK_DISTANCE, true));
			case BlockType::OakLeaves:     return a_Player.PlaceBlock(a_PlacePosition, Block::OakLeaves::OakLeaves(LEAVES_CHECK_DISTANCE, true));
			case BlockType::SpruceLeaves:  return a_Player.PlaceBlock(a_PlacePosition, Block::SpruceLeaves::SpruceLeaves(LEAVES_CHECK_DISTANCE, true));
			default: return false;
		}
	}
} ;




