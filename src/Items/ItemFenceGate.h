
#pragma once

#include "ItemHandler.h"
#include "Blocks/BlockFenceGate.h"





class cItemFenceGateHandler :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

private:

	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		using namespace Block;
		switch (BlockItemConverter::FromItem(PaletteUpgrade::FromItem(a_HeldItem.m_ItemType, a_HeldItem.m_ItemDamage)))
		{
			// TODO(12xx12) Check for neighbours
			case BlockType::AcaciaFenceGate:  return a_Player.PlaceBlock(a_PlacePosition, AcaciaFenceGate::AcaciaFenceGate(RotationToBlockFace(a_Player.GetYaw()), false, false, false));
			case BlockType::BirchFenceGate:   return a_Player.PlaceBlock(a_PlacePosition, BirchFenceGate::BirchFenceGate(RotationToBlockFace(a_Player.GetYaw()), false, false, false));
			case BlockType::CrimsonFenceGate: return a_Player.PlaceBlock(a_PlacePosition, CrimsonFenceGate::CrimsonFenceGate(RotationToBlockFace(a_Player.GetYaw()), false, false, false));
			case BlockType::DarkOakFenceGate: return a_Player.PlaceBlock(a_PlacePosition, DarkOakFenceGate::DarkOakFenceGate(RotationToBlockFace(a_Player.GetYaw()), false, false, false));
			case BlockType::JungleFenceGate:  return a_Player.PlaceBlock(a_PlacePosition, JungleFenceGate::JungleFenceGate(RotationToBlockFace(a_Player.GetYaw()), false, false, false));
			case BlockType::OakFenceGate:     return a_Player.PlaceBlock(a_PlacePosition, OakFenceGate::OakFenceGate(RotationToBlockFace(a_Player.GetYaw()), false, false, false));
			case BlockType::SpruceFenceGate:  return a_Player.PlaceBlock(a_PlacePosition, SpruceFenceGate::SpruceFenceGate(RotationToBlockFace(a_Player.GetYaw()), false, false, false));
			case BlockType::WarpedFenceGate:  return a_Player.PlaceBlock(a_PlacePosition, WarpedFenceGate::WarpedFenceGate(RotationToBlockFace(a_Player.GetYaw()), false, false, false));
			default: return false;
		}
	}
};
