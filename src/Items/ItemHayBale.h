
#pragma once

#include "Registries/BlockStates.h"

class cItemHayBaleHandler final : public cSimplePlaceableItemHandler
{
	using Super = cSimplePlaceableItemHandler;

public:

	using Super::Super;

protected:
	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, Vector3i a_PlacePosition, eBlockFace a_ClickedBlockFace, Vector3i a_CursorPosition) const override
	{
		auto Axis = Block::HayBlock::Axis::X;
		switch (a_ClickedBlockFace)
		{
			case BLOCK_FACE_NONE:
			{
				ASSERT(!"Facing is NONE");
				break;
			}
			case BLOCK_FACE_XM:
			case BLOCK_FACE_XP:
			{
				Axis = Block::HayBlock::Axis::X;
				break;
			}
			case BLOCK_FACE_YM:
			case BLOCK_FACE_YP:
			{
				Axis = Block::HayBlock::Axis::Y;
				break;
			}
			case BLOCK_FACE_ZM:
			case BLOCK_FACE_ZP:
			{
				Axis = Block::HayBlock::Axis::Z;
				break;
			}
		}
		a_Player.PlaceBlock(a_PlacePosition, Block::HayBlock::HayBlock(Axis));
		return true;
	}
};
