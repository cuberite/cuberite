#pragma once

#include "ItemHandler.h"





class cSimplePlaceableItemHandler final : public cItemHandler
{
	using Super = cItemHandler;

  public:
	constexpr cSimplePlaceableItemHandler(int a_ItemType, BLOCKTYPE a_BlockType) :
		Super(a_ItemType), m_BlockType(a_BlockType)
	{
	}


	virtual bool IsPlaceable(void) const override { return true; }


	virtual bool CommitPlacement(
		cPlayer & a_Player,
		const cItem & a_HeldItem,
		const Vector3i a_PlacePosition,
		const eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPosition
	) const override
	{
		return a_Player.PlaceBlock(a_PlacePosition, m_BlockType, 0);
	}

  private:
	BLOCKTYPE m_BlockType;
};
