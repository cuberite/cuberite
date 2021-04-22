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


	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player, const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPos,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta) override
	{
		a_BlockType = m_BlockType;
		a_BlockMeta = 0;
		return true;
	}

private:

	BLOCKTYPE m_BlockType;
};
