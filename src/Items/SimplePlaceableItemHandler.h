#pragma once

#include "ItemHandler.h"





class cSimplePlaceableItemHandler :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cSimplePlaceableItemHandler(int a_ItemType, BLOCKTYPE a_BlockType) :
		Super(a_ItemType),
		m_BlockType(a_BlockType)
	{
	}


	virtual bool IsPlaceable(void) override
	{
		return true;
	}


	virtual bool OnPlacementCommit(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		return a_Player.PlaceBlock(a_PlacePosition, m_BlockType, 0);
	}

private:

	BLOCKTYPE m_BlockType;
};
