
#pragma once

#include "ItemHandler.h"
#include "Blocks/BlockGlazedTerracotta.h"





class cItemGlazedTerracottaHandler final  :
	public cSimplePlaceableItemHandler
{
	using Super = cSimplePlaceableItemHandler;

public:

	using Super::Super;

private:

	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		using namespace Block;
		switch (BlockItemConverter::FromItem(a_HeldItem.m_ItemType))
		{
			case BlockType::BlackGlazedTerracotta:     return a_Player.PlaceBlock(a_PlacePosition, BlackGlazedTerracotta::BlackGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::BlueGlazedTerracotta:      return a_Player.PlaceBlock(a_PlacePosition, BlueGlazedTerracotta::BlueGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::BrownGlazedTerracotta:     return a_Player.PlaceBlock(a_PlacePosition, BrownGlazedTerracotta::BrownGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::CyanGlazedTerracotta:      return a_Player.PlaceBlock(a_PlacePosition, CyanGlazedTerracotta::CyanGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::GrayGlazedTerracotta:      return a_Player.PlaceBlock(a_PlacePosition, GrayGlazedTerracotta::GrayGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::GreenGlazedTerracotta:     return a_Player.PlaceBlock(a_PlacePosition, GreenGlazedTerracotta::GreenGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::LightBlueGlazedTerracotta: return a_Player.PlaceBlock(a_PlacePosition, LightBlueGlazedTerracotta::LightBlueGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::LightGrayGlazedTerracotta: return a_Player.PlaceBlock(a_PlacePosition, LightGrayGlazedTerracotta::LightGrayGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::LimeGlazedTerracotta:      return a_Player.PlaceBlock(a_PlacePosition, LimeGlazedTerracotta::LimeGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::MagentaGlazedTerracotta:   return a_Player.PlaceBlock(a_PlacePosition, MagentaGlazedTerracotta::MagentaGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::OrangeGlazedTerracotta:    return a_Player.PlaceBlock(a_PlacePosition, OrangeGlazedTerracotta::OrangeGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::PinkGlazedTerracotta:      return a_Player.PlaceBlock(a_PlacePosition, PinkGlazedTerracotta::PinkGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::PurpleGlazedTerracotta:    return a_Player.PlaceBlock(a_PlacePosition, PurpleGlazedTerracotta::PurpleGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::RedGlazedTerracotta:       return a_Player.PlaceBlock(a_PlacePosition, RedGlazedTerracotta::RedGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::WhiteGlazedTerracotta:     return a_Player.PlaceBlock(a_PlacePosition, WhiteGlazedTerracotta::WhiteGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::YellowGlazedTerracotta:    return a_Player.PlaceBlock(a_PlacePosition, YellowGlazedTerracotta::YellowGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			default: return false;
		}
	}
};
