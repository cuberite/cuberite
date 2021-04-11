#pragma once

#include "ItemHandler.h"





class cItemLeverHandler :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

private:

	/** Converts the block face of the neighbor to which the lever is attached to the lever block's meta. */
	static NIBBLETYPE BlockFaceToMetaData(eBlockFace a_BlockFace)
	{
		// Determine lever direction:
		switch (a_BlockFace)
		{
			case BLOCK_FACE_YP:   return 0x6;
			case BLOCK_FACE_XP:   return 0x1;
			case BLOCK_FACE_XM:   return 0x2;
			case BLOCK_FACE_ZP:   return 0x3;
			case BLOCK_FACE_ZM:   return 0x4;
			case BLOCK_FACE_YM:   return 0x0;
			case BLOCK_FACE_NONE: break;
		}
		UNREACHABLE("Unsupported block face");
	}


	virtual bool OnPlacementCommit(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		return a_Player.PlaceBlock(a_PlacePosition, static_cast<BLOCKTYPE>(a_HeldItem.m_ItemType), BlockFaceToMetaData(a_ClickedBlockFace));
	}
};
