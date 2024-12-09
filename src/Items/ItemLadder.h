
#pragma once

#include "ItemHandler.h"
#include "SimplePlaceableItemHandler.h"
#include "Blocks/BlockLadder.h"





class cItemLadderHandler final  :
	public cSimplePlaceableItemHandler
{
	using Super = cSimplePlaceableItemHandler;

public:

	using Super::Super;

private:

	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		const auto & World = *a_Player.GetWorld();
		const auto ClickedBlockType = World.GetBlock(AddFaceDirection(a_PlacePosition, a_ClickedBlockFace, true));

		// Try finding a suitable neighbor block face for the ladder; start with the given one:
		if (!cBlockLadderHandler::CanBePlacedOn(ClickedBlockType, a_ClickedBlockFace))
		{
			// Couldn't be placed on whatever face was clicked, last ditch resort - find another face:
			a_ClickedBlockFace = FindSuitableFace(World, a_PlacePosition);
			if (a_ClickedBlockFace == BLOCK_FACE_NONE)
			{
				// No attachable face found - don't place the ladder:
				return false;
			}
		}

		return a_Player.PlaceBlock(a_PlacePosition, Block::Ladder::Ladder(RotationToBlockFace(a_Player.GetYaw()), a_Player.GetWorld()->GetBlock(a_PlacePosition).Type() == BlockType::Water));
	}


	/** Returns a suitable neighbor's blockface to place the ladder at the specified position.
	Returns BLOCK_FACE_NONE on failure. */
	static eBlockFace FindSuitableFace(const cWorld & a_World, const Vector3i a_Position)
	{
		for (const auto Face : { BLOCK_FACE_ZM, BLOCK_FACE_XP, BLOCK_FACE_ZP, BLOCK_FACE_XM })  // Loop through all faces in specific order.
		{
			// The direction of Face is relative to the direction the ladder faces.
			// This is the position, computed inverted, that such a ladder would attach to.
			const auto NeighborPosition = AddFaceDirection(a_Position, Face, true);

			if (cBlockLadderHandler::CanBePlacedOn(a_World.GetBlock(NeighborPosition), Face))
			{
				return Face;
			}
		}

		return BLOCK_FACE_NONE;
	}
};
