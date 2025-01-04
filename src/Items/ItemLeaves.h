
#pragma once

#include "ItemHandler.h"
#include "Blocks/BlockLeaves.h"





class cItemLeavesHandler final :
	public cSimplePlaceableItemHandler
{
	using Super = cSimplePlaceableItemHandler;

public:

	using Super::Super;





	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		bool IsPlacedInWater = a_Player.GetWorld()->GetBlock(a_PlacePosition).Type() == BlockType::Water;
		switch (BlockItemConverter::FromItem(a_HeldItem.m_ItemType))
		{
			case BlockType::AcaciaLeaves:  return a_Player.PlaceBlock(a_PlacePosition, Block::AcaciaLeaves::AcaciaLeaves(LEAVES_CHECK_DISTANCE, true, IsPlacedInWater));
			case BlockType::BirchLeaves:   return a_Player.PlaceBlock(a_PlacePosition, Block::BirchLeaves::BirchLeaves(LEAVES_CHECK_DISTANCE, true, IsPlacedInWater));
			case BlockType::DarkOakLeaves: return a_Player.PlaceBlock(a_PlacePosition, Block::DarkOakLeaves::DarkOakLeaves(LEAVES_CHECK_DISTANCE, true, IsPlacedInWater));
			case BlockType::JungleLeaves:  return a_Player.PlaceBlock(a_PlacePosition, Block::JungleLeaves::JungleLeaves(LEAVES_CHECK_DISTANCE, true, IsPlacedInWater));
			case BlockType::OakLeaves:     return a_Player.PlaceBlock(a_PlacePosition, Block::OakLeaves::OakLeaves(LEAVES_CHECK_DISTANCE, true, IsPlacedInWater));
			case BlockType::SpruceLeaves:  return a_Player.PlaceBlock(a_PlacePosition, Block::SpruceLeaves::SpruceLeaves(LEAVES_CHECK_DISTANCE, true, IsPlacedInWater));
			case BlockType::AzaleaLeaves:  return a_Player.PlaceBlock(a_PlacePosition, Block::AzaleaLeaves::AzaleaLeaves(LEAVES_CHECK_DISTANCE, true, IsPlacedInWater));
			case BlockType::CherryLeaves:  return a_Player.PlaceBlock(a_PlacePosition, Block::CherryLeaves::CherryLeaves(LEAVES_CHECK_DISTANCE, true, IsPlacedInWater));
			case BlockType::MangroveLeaves:return a_Player.PlaceBlock(a_PlacePosition, Block::MangroveLeaves::MangroveLeaves(LEAVES_CHECK_DISTANCE, true, IsPlacedInWater));
			case BlockType::FloweringAzaleaLeaves: return a_Player.PlaceBlock(a_PlacePosition, Block::FloweringAzaleaLeaves::FloweringAzaleaLeaves(LEAVES_CHECK_DISTANCE, true, IsPlacedInWater));
			case BlockType::PaleOakLeaves: return a_Player.PlaceBlock(a_PlacePosition, Block::PaleOakLeaves::PaleOakLeaves(LEAVES_CHECK_DISTANCE, true, IsPlacedInWater));
			default: return false;
		}
	}
} ;




