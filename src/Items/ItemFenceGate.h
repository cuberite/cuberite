
#pragma once

#include "ItemHandler.h"
#include "Blocks/BlockFenceGate.h"





class cItemFenceGateHandler final  :
	public cSimplePlaceableItemHandler
{
	using Super = cSimplePlaceableItemHandler;

public:

	using Super::Super;

private:

	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		using namespace Block;
		auto facing = RotationToBlockFace(a_Player.GetYaw(), true);
		switch (BlockItemConverter::FromItem(a_HeldItem.m_ItemType))
		{
			// TODO(12xx12) Check for neighbours
			case BlockType::AcaciaFenceGate:  return a_Player.PlaceBlock(a_PlacePosition, AcaciaFenceGate::AcaciaFenceGate(facing, false, false, false));
			case BlockType::BirchFenceGate:   return a_Player.PlaceBlock(a_PlacePosition, BirchFenceGate::BirchFenceGate(facing, false, false, false));
			case BlockType::CrimsonFenceGate: return a_Player.PlaceBlock(a_PlacePosition, CrimsonFenceGate::CrimsonFenceGate(facing, false, false, false));
			case BlockType::DarkOakFenceGate: return a_Player.PlaceBlock(a_PlacePosition, DarkOakFenceGate::DarkOakFenceGate(facing, false, false, false));
			case BlockType::JungleFenceGate:  return a_Player.PlaceBlock(a_PlacePosition, JungleFenceGate::JungleFenceGate(facing, false, false, false));
			case BlockType::OakFenceGate:     return a_Player.PlaceBlock(a_PlacePosition, OakFenceGate::OakFenceGate(facing, false, false, false));
			case BlockType::SpruceFenceGate:  return a_Player.PlaceBlock(a_PlacePosition, SpruceFenceGate::SpruceFenceGate(facing, false, false, false));
			case BlockType::WarpedFenceGate:  return a_Player.PlaceBlock(a_PlacePosition, WarpedFenceGate::WarpedFenceGate(facing, false, false, false));
			case BlockType::BambooFenceGate:  return a_Player.PlaceBlock(a_PlacePosition, BambooFenceGate::BambooFenceGate(facing, false, false, false));
			case BlockType::CherryFenceGate:  return a_Player.PlaceBlock(a_PlacePosition, CherryFenceGate::CherryFenceGate(facing, false, false, false));
			case BlockType::MangroveFenceGate: return a_Player.PlaceBlock(a_PlacePosition, MangroveFenceGate::MangroveFenceGate(facing, false, false, false));
			case BlockType::PaleOakFenceGate: return a_Player.PlaceBlock(a_PlacePosition, PaleOakFenceGate::PaleOakFenceGate(facing, false, false, false));
			default: return false;
		}
	}
};
