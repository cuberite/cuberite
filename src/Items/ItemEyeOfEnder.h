
#pragma once

#include "ItemHandler.h"
#include "ItemThrowable.h"





class cItemEyeOfEnderHandler final:
	public cItemThrowableHandler
{
	using Super = cItemThrowableHandler;

public:

	constexpr cItemEyeOfEnderHandler(Item a_ItemType):
		Super(a_ItemType, cProjectileEntity::pkSnowball, 30)
	{
	}





	virtual bool OnItemUse(
		cWorld * a_World, cPlayer * a_Player, cBlockPluginInterface & a_PluginInterface, const cItem & a_Item,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace
	) const override
	{
		using namespace Block;

		// Try to fill an End Portal Frame block:
		if (a_ClickedBlockFace != BLOCK_FACE_NONE)
		{
			BlockState DestBlock;
			if (!a_World->GetBlock(a_ClickedBlockPos, DestBlock))
			{
				return false;
			}

			if (DestBlock.Type() == BlockType::EndPortalFrame)
			{
				if (EndPortalFrame::Eye(DestBlock))
				{
					return false;
				}

				a_World->SetBlock(a_ClickedBlockPos, EndPortalFrame::EndPortalFrame(true, EndPortalFrame::Facing(DestBlock)));
				if (!a_Player->IsGameModeCreative())
				{
					a_Player->GetInventory().RemoveOneEquippedItem();
				}

				cChunkInterface ChunkInterface(a_World->GetChunkMap());

				// Try to spawn portal:
				FindAndSetPortal(a_ClickedBlockPos, EndPortalFrame::Facing(DestBlock), ChunkInterface, *a_World);
				return true;
			}
			return false;
		}

		// TODO: Create projectile for Eye Of Ender
		// return Super::OnItemUse(a_World, a_Player, a_PluginInterface, a_Item, a_ClickedBlockPos, a_ClickedBlockFace);

		return false;
	}




	/** Returns false if portal cannot be made, true if portal was made. */
	static bool FindAndSetPortal(Vector3i a_FirstFrame, eBlockFace a_Direction, cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface)
	{
		/*
		PORTAL FINDING ALGORITHM
		=======================
		- Get clicked base block
		- Check diagonally (clockwise) for another portal block
			- if exists, and has eye, Continue. Abort if any are facing the wrong direction.
			- if doesn't exist, check horizontally (the block to the left of this block). Abort if there is no horizontal block.
		- After a corner has been met, traverse the portal clockwise, ensuring valid portal frames connect the rectangle.
		- Track the NorthWest Corner, and the dimensions.
		- If dimensions are valid, create the portal.
		*/

		static_assert((E_META_END_PORTAL_FRAME_ZM - E_META_END_PORTAL_FRAME_XM) == 1, "Should be going clockwise");

		const int MIN_PORTAL_WIDTH = 3;
		const int MAX_PORTAL_WIDTH = 4;

		// Directions to use for the clockwise traversal.
		static const Vector3i Left[] =
		{
			{ 0, 0,  0},  // 0
			{ 0, 0,  0},  // 1
			{-1, 0,  0},  // 2, North, left block is West  / XM  2
			{ 1, 0,  0},  // 0, South, left block is East  / XP  3
			{ 0, 0,  1},  // 1, West,  left block is South / ZP  4
			{ 0, 0, -1},  // 3, East,  left block is North / ZM  5
		};
		static const Vector3i LeftForward[] =
		{
			{ 0, 0,  0},  // 0
			{ 0, 0,  0},  // 1
			{-1, 0, -1},  // 2, North, left block is NorthWest / XM ZM --- 2
			{ 1, 0,  1},  // 0, South, left block is SouthEast / XP ZP --- 3
			{-1, 0,  1},  // 1, West,  left block is SouthWest / XM ZP ----4
			{ 1, 0, -1},  // 3, East,  left block is NorthEast / XP ZM --- 5
		};


		int EdgesComplete = -1;  // We start search _before_ finding the first edge
		Vector3i NorthWestCorner;
		int EdgeWidth[6] = { 1, 1, 1, 1, 1, };
		auto CurrentDirection = a_Direction;
		Vector3i CurrentPos = a_FirstFrame;

		// Scan clockwise until we have seen all 4 edges
		while (EdgesComplete < 4)
		{
			// Check if we are at a corner
			Vector3i NextPos = CurrentPos + LeftForward[CurrentDirection];
			if (a_ChunkInterface.GetBlock(NextPos).Type() == BlockType::EndPortalFrame)
			{
				// We have found the corner, move clockwise to next edge
				if (CurrentDirection == eBlockFace::BLOCK_FACE_XP)
				{
					// We are on the NW (XM, ZM) Corner
					// Relative to the previous frame, the portal should appear to the right of this portal frame.
					NorthWestCorner = NextPos - Left[CurrentDirection];
				}

				if (EdgesComplete == -1)
				{
					// Reset current width, we will revisit it last
					EdgeWidth[CurrentDirection] = 1;
				}

				// Rotate 90 degrees clockwise
				CurrentDirection = RotateBlockFaceCW(CurrentDirection);
				EdgesComplete++;
			}
			else
			{
				// We are not at a corner, keep walking the edge
				NextPos = CurrentPos + Left[CurrentDirection];

				EdgeWidth[CurrentDirection]++;
				if (EdgeWidth[CurrentDirection] > MAX_PORTAL_WIDTH)
				{
					// Don't build a portal that is too long.
					return false;
				}
			}

			if (!IsValidFrameAtPos(a_ChunkInterface, NextPos, CurrentDirection))
			{
				// Neither the edge nor the corner are valid portal blocks.
				return false;
			}

			CurrentPos = NextPos;
		}

		if ((EdgeWidth[2] != EdgeWidth[3]) || (EdgeWidth[4] != EdgeWidth[5]))
		{
			// Mismatched Portal Dimensions.
			return false;
		}
		if ((EdgeWidth[2] < MIN_PORTAL_WIDTH) || (EdgeWidth[4] < MIN_PORTAL_WIDTH))
		{
			// Portal too small.
			return false;
		}

		for (int i = 0; i < EdgeWidth[2]; i++)
		{
			for (int j = 0; j < EdgeWidth[4]; j++)
			{
				a_ChunkInterface.SetBlock(NorthWestCorner.addedXZ(i, j), Block::EndPortal::EndPortal());
			}
		}
		return true;
	}





	/** Return true if this block is a portal frame, has an eye, and is facing the correct direction. */
	static bool IsValidFrameAtPos(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos, eBlockFace a_ShouldFace)
	{
		auto EndPortalFrame = a_ChunkInterface.GetBlock(a_BlockPos);

		return (
			(EndPortalFrame.Type() == BlockType::EndPortalFrame) &&
			(Block::EndPortalFrame::Facing(EndPortalFrame) == a_ShouldFace) &&
			(Block::EndPortalFrame::Eye(EndPortalFrame))
		);
	}
} ;





