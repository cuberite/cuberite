#pragma once

#include "ItemHandler.h"
#include "Blocks/BlockTorch.h"





class cItemTorchHandler :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

private:

	/** Converts the block face of the neighbor to which the torch is attached, to the torch block's meta. */
	static NIBBLETYPE BlockFaceToMetaData(eBlockFace a_BlockFace)
	{
		switch (a_BlockFace)
		{
			case BLOCK_FACE_TOP:    return E_META_TORCH_FLOOR;
			case BLOCK_FACE_EAST:   return E_META_TORCH_EAST;
			case BLOCK_FACE_WEST:   return E_META_TORCH_WEST;
			case BLOCK_FACE_NORTH:  return E_META_TORCH_NORTH;
			case BLOCK_FACE_SOUTH:  return E_META_TORCH_SOUTH;
			default: UNREACHABLE("Unsupported block face");
		}
	}


	/** Returns a suitable neighbor's blockface to place the torch at the specified position.
	Returns BLOCK_FACE_NONE on failure. */
	static eBlockFace FindSuitableFace(const cWorld & a_World, const Vector3i a_Position)
	{
		for (const auto Face : { BLOCK_FACE_ZM, BLOCK_FACE_XP, BLOCK_FACE_ZP, BLOCK_FACE_XM, BLOCK_FACE_YP })  // Loop through all faces in specific order.
		{
			// The direction of Face is relative to the direction the torch faces.
			// This is the position, computed inverted, that such a torch would attach to.
			const auto NeighborPosition = AddFaceDirection(a_Position, Face, true);

			BLOCKTYPE NeighborBlockType;
			NIBBLETYPE NeighborBlockMeta;
			a_World.GetBlockTypeMeta(NeighborPosition, NeighborBlockType, NeighborBlockMeta);

			if (cBlockTorchHandler::CanBePlacedOn(NeighborBlockType, NeighborBlockMeta, Face))
			{
				return Face;
			}
		}

		return BLOCK_FACE_NONE;
	}

	virtual bool OnPlacementCommit(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		const auto & World = *a_Player.GetWorld();
		BLOCKTYPE ClickedBlockType;
		NIBBLETYPE ClickedBlockMeta;
		World.GetBlockTypeMeta(AddFaceDirection(a_PlacePosition, a_ClickedBlockFace, true), ClickedBlockType, ClickedBlockMeta);

		// Try finding a suitable neighbor block face for the torch; start with the given one:
		if (!cBlockTorchHandler::CanBePlacedOn(ClickedBlockType, ClickedBlockMeta, a_ClickedBlockFace))
		{
			// Couldn't be placed on whatever face was clicked, last ditch resort - find another face:
			a_ClickedBlockFace = FindSuitableFace(World, a_PlacePosition);
			if (a_ClickedBlockFace == BLOCK_FACE_NONE)
			{
				// No attachable face found - don't place the torch:
				return false;
			}
		}

		return a_Player.PlaceBlock(a_PlacePosition, static_cast<BLOCKTYPE>(a_HeldItem.m_ItemType), BlockFaceToMetaData(a_ClickedBlockFace));
	}
};
