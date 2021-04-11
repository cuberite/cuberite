#pragma once

#include "ItemHandler.h"





class cItemHopperHandler :
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
			case BLOCK_FACE_BOTTOM: return E_META_HOPPER_FACING_YM;
			case BLOCK_FACE_TOP:    return E_META_HOPPER_FACING_YM;
			case BLOCK_FACE_EAST:   return E_META_HOPPER_FACING_XM;
			case BLOCK_FACE_NORTH:  return E_META_HOPPER_FACING_ZP;
			case BLOCK_FACE_SOUTH:  return E_META_HOPPER_FACING_ZM;
			case BLOCK_FACE_WEST:   return E_META_HOPPER_FACING_XP;
			default: UNREACHABLE("Unsupported block face");
		}
	}


	virtual bool OnPlacementCommit(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		return a_Player.PlaceBlock(a_PlacePosition, E_BLOCK_HOPPER, BlockFaceToMetaData(a_ClickedBlockFace));
	}
};
