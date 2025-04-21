
#pragma once

#include "ItemHandler.h"





class cItemLeverHandler final  :
	public cSimplePlaceableItemHandler
{
	using Super = cSimplePlaceableItemHandler;

public:

	using Super::Super;

private:

	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		enum Block::Lever::Face Facing;
		switch (a_ClickedBlockFace)
		{
			case BLOCK_FACE_XM:
			case BLOCK_FACE_XP:
			case BLOCK_FACE_ZM:
			case BLOCK_FACE_ZP:
			{
				Facing = Block::Lever::Face::Wall;
				return a_Player.PlaceBlock(a_PlacePosition, Block::Lever::Lever(Facing, a_ClickedBlockFace, false));
			}
			case BLOCK_FACE_YM:
			{
				Facing = Block::Lever::Face::Ceiling;
				break;
			}
			case BLOCK_FACE_YP:
			{
				Facing = Block::Lever::Face::Floor;
				break;
			}
			default: return false;
		}

		auto Lever = Block::Lever::Lever(Facing, RotationToBlockFace(a_Player.GetYaw()), false);
		return a_Player.PlaceBlock(a_PlacePosition, Lever);
	}
};
