
#pragma once

#include "ItemHandler.h"
#include "Blocks/BlockGlazedTerracotta.h"





class cItemGlazedTerracottaHandler final  :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

private:

	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		using namespace Block;
		switch (BlockItemConverter::FromItem(PaletteUpgrade::FromItem(a_HeldItem.m_ItemType, a_HeldItem.m_ItemDamage)))
		{
			case BlockType::BlackGlazedTerracotta:     a_Player.PlaceBlock(a_PlacePosition, BlackGlazedTerracotta::BlackGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::BlueGlazedTerracotta:      a_Player.PlaceBlock(a_PlacePosition, BlueGlazedTerracotta::BlueGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::BrownGlazedTerracotta:     a_Player.PlaceBlock(a_PlacePosition, BrownGlazedTerracotta::BrownGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::CyanGlazedTerracotta:      a_Player.PlaceBlock(a_PlacePosition, CyanGlazedTerracotta::CyanGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::GrayGlazedTerracotta:      a_Player.PlaceBlock(a_PlacePosition, GrayGlazedTerracotta::GrayGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::GreenGlazedTerracotta:     a_Player.PlaceBlock(a_PlacePosition, GreenGlazedTerracotta::GreenGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::LightBlueGlazedTerracotta: a_Player.PlaceBlock(a_PlacePosition, LightBlueGlazedTerracotta::LightBlueGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::LightGrayGlazedTerracotta: a_Player.PlaceBlock(a_PlacePosition, LightGrayGlazedTerracotta::LightGrayGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::LimeGlazedTerracotta:      a_Player.PlaceBlock(a_PlacePosition, LimeGlazedTerracotta::LimeGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::MagentaGlazedTerracotta:   a_Player.PlaceBlock(a_PlacePosition, MagentaGlazedTerracotta::MagentaGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::OrangeGlazedTerracotta:    a_Player.PlaceBlock(a_PlacePosition, OrangeGlazedTerracotta::OrangeGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::PinkGlazedTerracotta:      a_Player.PlaceBlock(a_PlacePosition, PinkGlazedTerracotta::PinkGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::PurpleGlazedTerracotta:    a_Player.PlaceBlock(a_PlacePosition, PurpleGlazedTerracotta::PurpleGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::RedGlazedTerracotta:       a_Player.PlaceBlock(a_PlacePosition, RedGlazedTerracotta::RedGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::WhiteGlazedTerracotta:     a_Player.PlaceBlock(a_PlacePosition, WhiteGlazedTerracotta::WhiteGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			case BlockType::YellowGlazedTerracotta:    a_Player.PlaceBlock(a_PlacePosition, YellowGlazedTerracotta::YellowGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())));
			default: return false;
		}
	}
};
