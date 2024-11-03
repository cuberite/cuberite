#pragma once
#include "AllTags/BlockTags.h"

class cItemFenceHandler final :
	public cSimplePlaceableItemHandler
{
	using Super = cSimplePlaceableItemHandler;


	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		BlockState ToPlace;

		bool north = false, south = false, west = false, east = false;
#define GET_DIR(FenceType) \
	{ \
		north = BlockTags::Fences(a_Player.GetWorld()->GetBlock(a_PlacePosition.addedZ(-1)).Type()); \
		south = BlockTags::Fences(a_Player.GetWorld()->GetBlock(a_PlacePosition.addedZ(1)).Type()); \
		west = BlockTags::Fences(a_Player.GetWorld()->GetBlock(a_PlacePosition.addedX(-1)).Type()); \
		east = BlockTags::Fences(a_Player.GetWorld()->GetBlock(a_PlacePosition.addedX(1)).Type()); \
		break; \
	}

#define GET_FENCE(FenceType) \
	ToPlace = Block::FenceType::FenceType(east, north, south, IsWater, west); break; \


		switch (a_HeldItem.m_ItemType)
		{
			case Item::OakFence: GET_DIR(OakFence)
			case Item::AcaciaFence: GET_DIR(AcaciaFence)
			case Item::BirchFence: GET_DIR(BirchFence)
			case Item::BambooFence: GET_DIR(BambooFence)
			case Item::DarkOakFence: GET_DIR(DarkOakFence)
			case Item::JungleFence: GET_DIR(JungleFence)
			case Item::CrimsonFence: GET_DIR(CrimsonFence)
			case Item::CherryFence: GET_DIR(CherryFence)
			case Item::MangroveFence: GET_DIR(MangroveFence)
			case Item::NetherBrickFence: GET_DIR(NetherBrickFence)
			case Item::PaleOakFence: GET_DIR(PaleOakFence)
			case Item::SpruceFence: GET_DIR(SpruceFence)
			case Item::WarpedFence: GET_DIR(WarpedFence)
		}

		bool IsWater = a_Player.GetWorld()->GetBlock(a_PlacePosition).Type() == BlockType::Water;

		switch (a_HeldItem.m_ItemType)
		{
			case Item::OakFence: GET_FENCE(OakFence)
			case Item::AcaciaFence: GET_FENCE(AcaciaFence)
			case Item::BirchFence: GET_FENCE(BirchFence)
			case Item::BambooFence: GET_FENCE(BambooFence)
			case Item::DarkOakFence: GET_FENCE(DarkOakFence)
			case Item::JungleFence: GET_FENCE(JungleFence)
			case Item::CrimsonFence: GET_FENCE(CrimsonFence)
			case Item::CherryFence: GET_FENCE(CherryFence)
			case Item::MangroveFence: GET_FENCE(MangroveFence)
			case Item::NetherBrickFence: GET_FENCE(NetherBrickFence)
			case Item::PaleOakFence: GET_FENCE(PaleOakFence)
			case Item::SpruceFence: GET_FENCE(SpruceFence)
			case Item::WarpedFence: GET_FENCE(WarpedFence)
		}

		return a_Player.PlaceBlock(a_PlacePosition, ToPlace);
	}
public:
	using Super::Super;
};
