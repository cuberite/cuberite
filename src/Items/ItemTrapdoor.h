#pragma once

#include "ItemHandler.h"
#include "Blocks/BlockTrapdoor.h"





class cItemTrapdoorHandler :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

private:

	inline static NIBBLETYPE BlockFaceToMetaData(eBlockFace a_BlockFace)
	{
		switch (a_BlockFace)
		{
			case BLOCK_FACE_ZP: return 0x1;
			case BLOCK_FACE_ZM: return 0x0;
			case BLOCK_FACE_XP: return 0x3;
			case BLOCK_FACE_XM: return 0x2;
			default: UNREACHABLE("Unsupported block face");
		}
	}


	virtual bool OnPlacementCommit(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		NIBBLETYPE Meta;

		if (a_ClickedBlockFace == BLOCK_FACE_YP)
		{
			// Trapdoor is placed on top of a block.
			// Engage yaw rotation to determine hinge direction:
			Meta = cBlockTrapdoorHandler::YawToMetaData(a_Player.GetYaw());
		}
		else if (a_ClickedBlockFace == BLOCK_FACE_YM)
		{
			// Trapdoor is placed on bottom of a block.
			// Engage yaw rotation to determine hinge direction, and toggle 'Move up half-block' bit on:
			Meta = cBlockTrapdoorHandler::YawToMetaData(a_Player.GetYaw()) | 0x8;
		}
		else
		{
			// Placement on block sides; hinge direction is determined by which side was clicked:
			Meta = BlockFaceToMetaData(a_ClickedBlockFace);

			if (a_CursorPosition.y > 7)
			{
				// Trapdoor is placed on a higher half of a vertical block.
				// Toggle 'Move up half-block' bit on:
				Meta |= 0x8;
			}
		}

		return a_Player.PlaceBlock(a_PlacePosition, static_cast<BLOCKTYPE>(a_HeldItem.m_ItemType), Meta);
	}
};
