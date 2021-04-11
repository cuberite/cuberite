#pragma once

#include "ItemHandler.h"
#include "Blocks/BlockStairs.h"





class cItemStairsHandler :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

private:

	virtual bool OnPlacementCommit(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		NIBBLETYPE Meta = cBlockStairsHandler::YawToMetaData(a_Player.GetYaw());

		switch (a_ClickedBlockFace)
		{
			case BLOCK_FACE_TOP:    break;
			case BLOCK_FACE_BOTTOM: Meta |= 0x4; break;  // When placing onto a bottom face, always place an upside-down stairs block.
			case BLOCK_FACE_EAST:
			case BLOCK_FACE_NORTH:
			case BLOCK_FACE_SOUTH:
			case BLOCK_FACE_WEST:
			{
				// When placing onto a sideways face, check cursor, if in top half, make it an upside-down stairs block:
				if (a_CursorPosition.y > 8)
				{
					Meta |= 0x4;
				}
				break;
			}
			default: UNREACHABLE("Unhandled block face");
		}

		return a_Player.PlaceBlock(a_PlacePosition, static_cast<BLOCKTYPE>(a_HeldItem.m_ItemType), Meta);
	}
};
