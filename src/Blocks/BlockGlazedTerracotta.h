
#pragma once

#include "Mixins.h"





class cBlockGlazedTerracottaHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface,
		cPlayer & a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BlockState & a_Block
	) const override
	{
		using namespace Block;
		switch (m_BlockType)
		{
			case BlockType::BlackGlazedTerracotta:     BlackGlazedTerracotta::BlackGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())); break;
			case BlockType::BlueGlazedTerracotta:      BlueGlazedTerracotta::BlueGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())); break;
			case BlockType::BrownGlazedTerracotta:     BrownGlazedTerracotta::BrownGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())); break;
			case BlockType::CyanGlazedTerracotta:      CyanGlazedTerracotta::CyanGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())); break;
			case BlockType::GrayGlazedTerracotta:      GrayGlazedTerracotta::GrayGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())); break;
			case BlockType::GreenGlazedTerracotta:     GreenGlazedTerracotta::GreenGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())); break;
			case BlockType::LightBlueGlazedTerracotta: LightBlueGlazedTerracotta::LightBlueGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())); break;
			case BlockType::LightGrayGlazedTerracotta: LightGrayGlazedTerracotta::LightGrayGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())); break;
			case BlockType::LimeGlazedTerracotta:      LimeGlazedTerracotta::LimeGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())); break;
			case BlockType::MagentaGlazedTerracotta:   MagentaGlazedTerracotta::MagentaGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())); break;
			case BlockType::OrangeGlazedTerracotta:    OrangeGlazedTerracotta::OrangeGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())); break;
			case BlockType::PinkGlazedTerracotta:      PinkGlazedTerracotta::PinkGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())); break;
			case BlockType::PurpleGlazedTerracotta:    PurpleGlazedTerracotta::PurpleGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())); break;
			case BlockType::RedGlazedTerracotta:       RedGlazedTerracotta::RedGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())); break;
			case BlockType::WhiteGlazedTerracotta:     WhiteGlazedTerracotta::WhiteGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())); break;
			case BlockType::YellowGlazedTerracotta:    YellowGlazedTerracotta::YellowGlazedTerracotta(RotationToBlockFace(a_Player.GetYaw())); break;
			default: return false;
		}
		ASSERT(a_Block.Type() == m_BlockType);
		return true;
	}

};
