
#pragma once

#include "ItemHandler.h"





class cItemLeavesHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemLeavesHandler(int a_ItemType):
		Super(a_ItemType)
	{
	}





	virtual bool OnPlacementCommit(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		return a_Player.PlaceBlock(
			a_PlacePosition,
			static_cast<BLOCKTYPE>(m_ItemType),
			static_cast<NIBBLETYPE>(a_HeldItem.m_ItemDamage | 0x4)  // 0x4 bit set means this is a player-placed leaves block, not to be decayed.
		);
	}
} ;




