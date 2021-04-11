
#pragma once

#include "ItemHandler.h"





class cItemSaplingHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemSaplingHandler(int a_ItemType):
		Super(a_ItemType)
	{
	}


	virtual bool OnPlacementCommit(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		return a_Player.PlaceBlock(
			a_PlacePosition,
			static_cast<BLOCKTYPE>(m_ItemType),
			static_cast<NIBBLETYPE>(a_HeldItem.m_ItemDamage & 0x07)  // Allow only the lowest 3 bits (top bit is for growth).
		);
	}
} ;
