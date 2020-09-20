
#pragma once

#include "ItemHandler.h"
#include "ItemThrowable.h"





class cItemEyeOfEnderHandler:
	public cItemThrowableHandler
{
	using Super = cItemThrowableHandler;

public:

	cItemEyeOfEnderHandler():
		Super(E_ITEM_EYE_OF_ENDER, cProjectileEntity::pkSnowball, 30)
	{
	}





	virtual bool OnItemUse(
		cWorld * a_World, cPlayer * a_Player, cBlockPluginInterface & a_PluginInterface, const cItem & a_Item,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace
	) override
	{
		// Try to fill an End Portal Frame block:
		if (a_ClickedBlockFace != BLOCK_FACE_NONE)
		{
			BLOCKTYPE FacingBlock;
			NIBBLETYPE FacingMeta;
			a_World->GetBlockTypeMeta(a_ClickedBlockPos, FacingBlock, FacingMeta);
			if (FacingBlock == E_BLOCK_END_PORTAL_FRAME)
			{
				// Fill the portal frame. E_META_END_PORTAL_EYE is the bit for holding the eye of ender.
				if ((FacingMeta & E_META_END_PORTAL_FRAME_EYE) != E_META_END_PORTAL_FRAME_EYE)
				{
					a_World->SetBlock(a_ClickedBlockPos, E_BLOCK_END_PORTAL_FRAME, FacingMeta | E_META_END_PORTAL_FRAME_EYE);
					if (!a_Player->IsGameModeCreative())
					{
						a_Player->GetInventory().RemoveOneEquippedItem();
					}

					cChunkInterface ChunkInterface(a_World->GetChunkMap());

					// Try to spawn portal:
					FindAndSetPortal(a_ClickedBlockPos, FacingMeta & 3, ChunkInterface, *a_World);
					return true;
				}
			}
			return false;
		}

		// TODO: Create projectile for Eye Of Ender
		// return Super::OnItemUse(a_World, a_Player, a_PluginInterface, a_Item, a_ClickedBlockPos, a_ClickedBlockFace);

		return false;
	}




	/** Returns false if portal cannot be made, true if portal was made. */
	static bool FindAndSetPortal(Vector3i a_FirstFrame, NIBBLETYPE a_Direction, cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface)
	{
		/*
		PORTAL FINDING ALGORITH
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
			{ 1, 0,  0},  // 0, South, left block is East  / XP
			{ 0, 0,  1},  // 1, West,  left block is South / ZP
			{-1, 0,  0},  // 2, North, left block is West  / XM
			{ 0, 0, -1},  // 3, East,  left block is North / ZM
		};
		static const Vector3i LeftForward[] =
		{
			{ 1, 0,  1},  // 0, South, left block is SouthEast / XP ZP
			{-1, 0,  1},  // 1, West,  left block is SouthWest / XM ZP
			{-1, 0, -1},  // 2, North, left block is NorthWest / XM ZM
			{ 1, 0, -1},  // 3, East,  left block is NorthEast / XP ZM
		};


		int EdgesComplete = -1;  // We start search _before_ finding the first edge
		Vector3i NorthWestCorner;
		int EdgeWidth[4] = { 1, 1, 1, 1 };
		NIBBLETYPE CurrentDirection = a_Direction;
		Vector3i CurrentPos = a_FirstFrame;

		// Scan clockwise until we have seen all 4 edges
		while (EdgesComplete < 4)
		{
			// Check if we are at a corner
			Vector3i NextPos = CurrentPos + LeftForward[CurrentDirection];
			if (IsPortalFrame(a_ChunkInterface.GetBlock(NextPos)))
			{
				// We have found the corner, move clockwise to next edge
				if (CurrentDirection == E_META_END_PORTAL_FRAME_XP)
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
				CurrentDirection = (CurrentDirection + 1) % 4;
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

		if ((EdgeWidth[0] != EdgeWidth[2]) || (EdgeWidth[1] != EdgeWidth[3]))
		{
			// Mismatched Portal Dimensions.
			return false;
		}
		if ((EdgeWidth[0] < MIN_PORTAL_WIDTH) || (EdgeWidth[1] < MIN_PORTAL_WIDTH))
		{
			// Portal too small.
			return false;
		}

		for (int i = 0; i < EdgeWidth[0]; i++)
		{
			for (int j = 0; j < EdgeWidth[1]; j++)
			{
				a_ChunkInterface.SetBlock(NorthWestCorner.x + i, NorthWestCorner.y, NorthWestCorner.z + j, E_BLOCK_END_PORTAL, 0);
			}
		}
		return true;
	}





	/** Return true if this block is a portal frame, has an eye, and is facing the correct direction. */
	static bool IsValidFrameAtPos(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos, NIBBLETYPE a_ShouldFace)
	{
		BLOCKTYPE BlockType;
		NIBBLETYPE BlockMeta;

		return (
			a_ChunkInterface.GetBlockTypeMeta(a_BlockPos, BlockType, BlockMeta) &&
			(BlockType == E_BLOCK_END_PORTAL_FRAME) &&
			(BlockMeta == (a_ShouldFace | E_META_END_PORTAL_FRAME_EYE))
		);
	}




	/** Return true if this block is a portal frame. */
	static bool IsPortalFrame(BLOCKTYPE BlockType)
	{
		return (BlockType == E_BLOCK_END_PORTAL_FRAME);
	}
} ;





