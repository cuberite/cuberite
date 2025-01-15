
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../ClientHandle.h"





class cItemSignHandler final:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

private:

	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		BlockState BlockToPlace;

		bool IsPlacedInWater =a_Player.GetWorld()->GetBlock(a_PlacePosition).Type() == BlockType::Water;
#define BLOCK_PLACE_SIGN(SignType)\
	BlockToPlace = Block::SignType::SignType(RotationToFineFace(a_Player.GetYaw()), IsPlacedInWater); break;\

		if (a_ClickedBlockFace == BLOCK_FACE_TOP)
		{
			switch (a_HeldItem.m_ItemType)
			{
				case Item::AcaciaSign:  BlockToPlace = Block::AcaciaSign::AcaciaSign(RotationToFineFace(a_Player.GetYaw()), IsPlacedInWater); break;
				case Item::BirchSign:   BlockToPlace = Block::BirchSign::BirchSign(RotationToFineFace(a_Player.GetYaw()), IsPlacedInWater); break;
				case Item::CrimsonSign: BlockToPlace = Block::CrimsonSign::CrimsonSign(RotationToFineFace(a_Player.GetYaw()), IsPlacedInWater); break;
				case Item::DarkOakSign: BlockToPlace = Block::DarkOakSign::DarkOakSign(RotationToFineFace(a_Player.GetYaw()), IsPlacedInWater); break;
				case Item::JungleSign:  BlockToPlace = Block::JungleSign::JungleSign(RotationToFineFace(a_Player.GetYaw()), IsPlacedInWater); break;
				case Item::OakSign:     BlockToPlace = Block::OakSign::OakSign(RotationToFineFace(a_Player.GetYaw()), IsPlacedInWater); break;
				case Item::SpruceSign:  BlockToPlace = Block::SpruceSign::SpruceSign(RotationToFineFace(a_Player.GetYaw()), IsPlacedInWater); break;
				case Item::WarpedSign:  BlockToPlace = Block::WarpedSign::WarpedSign(RotationToFineFace(a_Player.GetYaw()), IsPlacedInWater); break;
				case Item::CherrySign:  BLOCK_PLACE_SIGN(CherrySign)
				case Item::BambooSign:  BLOCK_PLACE_SIGN(BambooSign)
				case Item::MangroveSign: BLOCK_PLACE_SIGN(MangroveSign)
				case Item::PaleOakSign: BLOCK_PLACE_SIGN(PaleOakSign)
				default: return false;
			}
		}
		else
		{
			switch (a_HeldItem.m_ItemType)
			{
				case Item::AcaciaSign:  BlockToPlace = Block::AcaciaWallSign::AcaciaWallSign(); break;
				case Item::BirchSign:   BlockToPlace = Block::BirchWallSign::BirchWallSign(); break;
				case Item::CrimsonSign: BlockToPlace = Block::CrimsonWallSign::CrimsonWallSign(); break;
				case Item::DarkOakSign: BlockToPlace = Block::DarkOakWallSign::DarkOakWallSign(); break;
				case Item::JungleSign:  BlockToPlace = Block::JungleWallSign::JungleWallSign(); break;
				case Item::OakSign:     BlockToPlace = Block::OakWallSign::OakWallSign(); break;
				case Item::SpruceSign:  BlockToPlace = Block::SpruceWallSign::SpruceWallSign(); break;
				case Item::WarpedSign:  BlockToPlace = Block::WarpedWallSign::WarpedWallSign(); break;
				case Item::CherrySign:  BlockToPlace = Block::CherrySign::CherrySign(); break;
				case Item::BambooSign:  BlockToPlace = Block::BambooSign::BambooSign(); break;
				case Item::MangroveSign: BlockToPlace = Block::MangroveSign::MangroveSign(); break;
				case Item::PaleOakSign: BlockToPlace = Block::PaleOakSign::PaleOakSign(); break;
				default: return false;
			}
		}

		if (!a_Player.PlaceBlock(a_PlacePosition, BlockToPlace))
		{
			return false;
		}

		// After successfully placing the sign, open the sign editor for the player:
		a_Player.GetClientHandle()->SendEditSign(a_PlacePosition);
		return true;
	}


	virtual bool IsPlaceable(void) const override
	{
		return true;
	}
} ;




