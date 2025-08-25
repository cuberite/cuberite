
#pragma once

#include "ItemHandler.h"
#include "SimplePlaceableItemHandler.h"
#include "Blocks/BlockChest.h"





class cItemChestHandler final :
	public cSimplePlaceableItemHandler
{
	using Super = cSimplePlaceableItemHandler;

public:

	using Super::Super;

	cItemChestHandler(const cItemChestHandler &) = delete;

private:

	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		// Check that there is at most one single neighbor of the same chest type:
		static const Vector3i CrossCoords[] =
		{
			{-1, 0,  0},
			{ 0, 0, -1},
			{ 1, 0,  0},
			{ 0, 0,  1},
		};

		auto & World = *a_Player.GetWorld();
		int NeighborIdx = -1;
		auto ChestType = BlockItemConverter::FromItem(m_ItemType);

		for (size_t i = 0; i < ARRAYCOUNT(CrossCoords); i++)
		{
			const auto NeighborPos = a_PlacePosition + CrossCoords[i];
			if (World.GetBlock(NeighborPos) != ChestType)
			{
				continue;
			}
			if (NeighborIdx >= 0)
			{
				// Can't place here, there are already two neighbors, this would form a 3-block chest
				return false;
			}
			NeighborIdx = static_cast<int>(i);

			// Check that this neighbor is a single chest:
			for (size_t j = 0; j < ARRAYCOUNT(CrossCoords); j++)
			{
				if (World.GetBlock(NeighborPos + CrossCoords[j]) == ChestType)
				{
					// Trying to place next to a dblchest
					return false;
				}
			}  // for j
		}  // for i

		// Get the meta of the placed chest; take existing neighbors into account:
		const auto Yaw = a_Player.GetYaw();
		bool IsPlacedInWater = World.GetBlock(a_PlacePosition).Type() == BlockType::Water;
		BlockState BlockToPlace;
		using namespace Block;
		switch (NeighborIdx)
		{
			case 0:
			case 2:
			{
				// The neighbor is in the X axis, form a X-axis-aligned dblchest:
				switch (ChestType)
				{
					case BlockType::Chest:
					{
						if ((Yaw >= -90) && (Yaw < 90))
						{
							BlockToPlace = Chest::Chest(BLOCK_FACE_NORTH, Chest::Type::Left, IsPlacedInWater);
							break;
						}
						else
						{
							BlockToPlace = Chest::Chest(BLOCK_FACE_SOUTH, Chest::Type::Right, IsPlacedInWater);
							break;
						}
					}
					case BlockType::TrappedChest:
					{
						if ((Yaw >= -90) && (Yaw < 90))
						{
							BlockToPlace = TrappedChest::TrappedChest(BLOCK_FACE_NORTH, TrappedChest::Type::Left, IsPlacedInWater);
							break;
						}
						else
						{
							BlockToPlace = TrappedChest::TrappedChest(BLOCK_FACE_SOUTH, TrappedChest::Type::Right, IsPlacedInWater);
							break;
						}
					}
					default: return false;
				}
				break;
			}
			case 1:
			case 3:
			{
				// The neighbor is in the Z axis, form a Z-axis-aligned dblchest:
				switch (ChestType)
				{
					case BlockType::Chest:
					{
						if ((Yaw < 0))
						{
							BlockToPlace = Chest::Chest(BLOCK_FACE_EAST, Chest::Type::Left, IsPlacedInWater);
							break;
						}
						else
						{
							BlockToPlace = Chest::Chest(BLOCK_FACE_WEST, Chest::Type::Right, IsPlacedInWater);
							break;
						}
					}
					case BlockType::TrappedChest:
					{
						if ((Yaw < 0))
						{
							BlockToPlace = TrappedChest::TrappedChest(BLOCK_FACE_EAST, TrappedChest::Type::Left, IsPlacedInWater);
							break;
						}
						else
						{
							BlockToPlace = TrappedChest::TrappedChest(BLOCK_FACE_WEST, TrappedChest::Type::Right, IsPlacedInWater);
							break;
						}
					}
					default: return false;
				}
				break;
			}
			default:
			{
				// No neighbor, place based on yaw:
				switch (ChestType)
				{
					case BlockType::Chest:        BlockToPlace = Chest::Chest(RotationToBlockFace(a_Player.GetYaw()), Chest::Type::Single, IsPlacedInWater); break;
					case BlockType::TrappedChest: BlockToPlace = TrappedChest::TrappedChest(RotationToBlockFace(a_Player.GetYaw()), TrappedChest::Type::Single, IsPlacedInWater); break;
					default: return false;
				}
				break;
			}
		}  // switch (NeighborIdx)

		// Place the new chest:
		if (!a_Player.PlaceBlock(a_PlacePosition, BlockToPlace))
		{
			return false;
		}

		// Adjust the existing chest, if any:
		if (NeighborIdx != -1)
		{
			BlockState Neighbour = World.GetBlock(a_PlacePosition + CrossCoords[NeighborIdx]);
			switch (NeighborIdx)
			{
				case 0:
				case 2:
				{
					// The neighbor is in the X axis, form a X-axis-aligned dblchest:
					switch (ChestType)
					{
						case BlockType::Chest:
						{
							if ((Yaw >= -90) && (Yaw < 90))
							{
								Neighbour = Chest::Chest(BLOCK_FACE_NORTH, Chest::Type::Right, IsPlacedInWater);
								break;
							}
							else
							{
								Neighbour = Chest::Chest(BLOCK_FACE_SOUTH, Chest::Type::Left, IsPlacedInWater);
								break;
							}
						}
						case BlockType::TrappedChest:
						{
							if ((Yaw >= -90) && (Yaw < 90))
							{
								Neighbour = TrappedChest::TrappedChest(BLOCK_FACE_NORTH, TrappedChest::Type::Right, IsPlacedInWater);
								break;
							}
							else
							{
								Neighbour = TrappedChest::TrappedChest(BLOCK_FACE_SOUTH, TrappedChest::Type::Left, IsPlacedInWater);
								break;
							}
						}
						default: return false;
					}
					break;
				}
				case 1:
				case 3:
				{
					// The neighbor is in the Z axis, form a Z-axis-aligned dblchest:
					switch (ChestType)
					{
						case BlockType::Chest:
						{
							if ((Yaw < 0))
							{
								Neighbour = Chest::Chest(BLOCK_FACE_EAST, Chest::Type::Right, IsPlacedInWater);
								break;
							}
							else
							{
								Neighbour = Chest::Chest(BLOCK_FACE_WEST, Chest::Type::Left, IsPlacedInWater);
								break;
							}
						}
						case BlockType::TrappedChest:
						{
							if ((Yaw < 0))
							{
								Neighbour = TrappedChest::TrappedChest(BLOCK_FACE_EAST, TrappedChest::Type::Right, IsPlacedInWater);
								break;
							}
							else
							{
								Neighbour = TrappedChest::TrappedChest(BLOCK_FACE_WEST, TrappedChest::Type::Left, IsPlacedInWater);
								break;
							}
						}
						default: return false;
					}
					break;
				}
			}
			World.FastSetBlock(a_PlacePosition + CrossCoords[NeighborIdx], Neighbour);
		}

		return true;
	}
};
