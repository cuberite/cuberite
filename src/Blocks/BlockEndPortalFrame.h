
#pragma once

#include "BlockHandler.h"





class cBlockEndPortalFrameHandler final :
	public cMetaRotator<cBlockHandler, 0x03,
		E_META_END_PORTAL_FRAME_ZM,
		E_META_END_PORTAL_FRAME_XP,
		E_META_END_PORTAL_FRAME_ZP,
		E_META_END_PORTAL_FRAME_XM
	>
{
	using Super = cMetaRotator<
		cBlockHandler, 0x03,
		E_META_END_PORTAL_FRAME_ZM,
		E_META_END_PORTAL_FRAME_XP,
		E_META_END_PORTAL_FRAME_ZP,
		E_META_END_PORTAL_FRAME_XM
	>;

public:

	using Super::Super;

private:

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface,
		cPlayer & a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) const override
	{
		a_BlockType = m_BlockType;
		a_BlockMeta = YawToMetaData(a_Player.GetYaw());
		return true;
	}





	inline static NIBBLETYPE YawToMetaData(double a_Rotation)
	{
		a_Rotation += 90 + 45;  // So its not aligned with axis
		if (a_Rotation > 360)
		{
			a_Rotation -= 360;
		}

		if ((a_Rotation >= 0) && (a_Rotation < 90))
		{
			return E_META_END_PORTAL_FRAME_XM;
		}
		else if ((a_Rotation >= 180) && (a_Rotation < 270))
		{
			return E_META_END_PORTAL_FRAME_XP;
		}
		else if ((a_Rotation >= 90) && (a_Rotation < 180))
		{
			return E_META_END_PORTAL_FRAME_ZM;
		}
		else
		{
			return E_META_END_PORTAL_FRAME_ZP;
		}
	}





	virtual void OnPlaced(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, Vector3i a_BlockPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) const override
	{
		// E_META_END_PORTAL_FRAME_EYE is the bit which signifies the eye of ender is in it.
		// LOG("PortalPlaced, meta %d", a_BlockMeta);
		if ((a_BlockMeta & E_META_END_PORTAL_FRAME_EYE) == E_META_END_PORTAL_FRAME_EYE)
		{
			// LOG("Location is %d %d %d", a_BlockX, a_BlockY, a_BlockZ);
			// Direction is the first two bits, masked by 0x3
			FindAndSetPortal(a_BlockPos, a_BlockMeta & 3, a_ChunkInterface, a_WorldInterface);
		}
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

		// LOG("NW corner (low corner) %d %d %d", Corner.x, Corner.y, Corner.z);
		// LOG("%d by %d", Width[0], Width[1]);
		for (int i = 0; i < EdgeWidth[0]; i++)
		{
			for (int j = 0; j < EdgeWidth[1]; j++)
			{
				a_ChunkInterface.SetBlock(NorthWestCorner.x + i, NorthWestCorner.y, NorthWestCorner.z + j, E_BLOCK_END_PORTAL, 0);
				// TODO: Create block entity so portal doesn't become invisible on relog.
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





	virtual bool IsClickedThrough(void) const override
	{
		// TODO: Colision
		return true;
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 27;
	}
};




