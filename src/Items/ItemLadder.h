#pragma once

#include "ItemHandler.h"
#include "Blocks/BlockLadder.h"





class cItemLadderHandler :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

private:

	/** Converts the block face of the neighbor to which the ladder is attached to the ladder block's meta. */
	static NIBBLETYPE BlockFaceToMetaData(eBlockFace a_NeighborBlockFace)
	{
		switch (a_NeighborBlockFace)
		{
			case BLOCK_FACE_ZM: return 0x2;
			case BLOCK_FACE_ZP: return 0x3;
			case BLOCK_FACE_XM: return 0x4;
			case BLOCK_FACE_XP: return 0x5;
			case BLOCK_FACE_YM:
			case BLOCK_FACE_YP: return 0x2;
			default: UNREACHABLE("Unsupported neighbor block face");
		}
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


	virtual bool OnPlacementCommit(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
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
				// No attachable face found - don't place the torch:
				return false;
			}
		}

		return a_Player.PlaceBlock(a_PlacePosition, E_BLOCK_LADDER, BlockFaceToMetaData(a_ClickedBlockFace));
	}
};
