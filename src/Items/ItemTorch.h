
#pragma once

#include "ItemHandler.h"
#include "Blocks/BlockTorch.h"





class cItemTorchHandler final  :
	public cSimplePlaceableItemHandler
{
	using Super = cSimplePlaceableItemHandler;

public:

	using Super::Super;

private:


	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		const auto & World = *a_Player.GetWorld();
		auto OldBlock = World.GetBlock(AddFaceDirection(a_PlacePosition, a_ClickedBlockFace, true));

		// Try finding a suitable neighbor block face for the torch; start with the given one:
		if (!cBlockTorchHandler::CanBePlacedOn(OldBlock, a_ClickedBlockFace))
		{
			// Couldn't be placed on whatever face was clicked, last ditch resort - find another face:
			a_ClickedBlockFace = FindSuitableFace(World, a_PlacePosition);
			if (a_ClickedBlockFace == BLOCK_FACE_NONE)
			{
				// No attachable face found - don't place the torch:
				return false;
			}
		}

		BlockState BlockToPlace;

		switch (BlockItemConverter::FromItem(a_HeldItem.m_ItemType))
		{
			case BlockType::Torch:
			case BlockType::WallTorch:
			{
				switch (a_ClickedBlockFace)
				{
					case BLOCK_FACE_YP:
					{
						BlockToPlace = Block::Torch::Torch();
						break;
					}
					case BLOCK_FACE_XM:
					case BLOCK_FACE_XP:
					case BLOCK_FACE_ZM:
					case BLOCK_FACE_ZP:
					{
						BlockToPlace = Block::WallTorch::WallTorch(a_ClickedBlockFace);
						break;
					}
					case BLOCK_FACE_NONE:
					case BLOCK_FACE_YM:
					{
						return false;
					}
				}
				break;
			}
			case BlockType::RedstoneTorch:
			case BlockType::RedstoneWallTorch:
			{
				switch (a_ClickedBlockFace)
				{
					case BLOCK_FACE_YP:
					{
						BlockToPlace = Block::RedstoneTorch::RedstoneTorch();
						break;
					}
					case BLOCK_FACE_XM:
					case BLOCK_FACE_XP:
					case BLOCK_FACE_ZM:
					case BLOCK_FACE_ZP:
					{
						BlockToPlace = Block::RedstoneWallTorch::RedstoneWallTorch(a_ClickedBlockFace, true);
						break;
					}
					case BLOCK_FACE_NONE:
					case BLOCK_FACE_YM:
					{
						return false;
					}
				}
				break;
			}
			case BlockType::SoulTorch:
			case BlockType::SoulWallTorch:
			{
				switch (a_ClickedBlockFace)
				{
					case BLOCK_FACE_YP:
					{
						BlockToPlace = Block::SoulTorch::SoulTorch();
						break;
					}
					case BLOCK_FACE_XM:
					case BLOCK_FACE_XP:
					case BLOCK_FACE_ZM:
					case BLOCK_FACE_ZP:
					{
						BlockToPlace = Block::SoulWallTorch::SoulWallTorch(a_ClickedBlockFace);
						break;
					}
					case BLOCK_FACE_NONE:
					case BLOCK_FACE_YM:
					{
						return false;
					}
				}
				break;
			}
			default: return false;
		}

		return a_Player.PlaceBlock(a_PlacePosition, BlockToPlace);
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

			auto Neighbor = a_World.GetBlock(NeighborPosition);

			if (cBlockTorchHandler::CanBePlacedOn(Neighbor, Face))
			{
				return Face;
			}
		}

		return BLOCK_FACE_NONE;
	}
};
