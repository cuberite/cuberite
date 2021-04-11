#pragma once

#include "ItemHandler.h"





class cItemTripwireHookHandler :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

private:

	static NIBBLETYPE BlockFaceToMetaData(eBlockFace a_BlockFace)
	{
		switch (a_BlockFace)
		{
			case BLOCK_FACE_XM: return 0x1;
			case BLOCK_FACE_XP: return 0x3;
			case BLOCK_FACE_ZM: return 0x2;
			case BLOCK_FACE_ZP: return 0x0;
			default: UNREACHABLE("Unsupported block face");
		}
	}


	virtual bool OnPlacementCommit(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		if ((a_ClickedBlockFace == BLOCK_FACE_YP) || (a_ClickedBlockFace == BLOCK_FACE_YM))
		{
			return false;
		}

		return a_Player.PlaceBlock(a_PlacePosition, E_BLOCK_TRIPWIRE_HOOK, BlockFaceToMetaData(a_ClickedBlockFace));
	}
};
