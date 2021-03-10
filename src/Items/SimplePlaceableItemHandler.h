#pragma once

#include "ItemHandler.h"





class cSimplePlaceableItemHandler :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cSimplePlaceableItemHandler(int a_ItemType, BlockType a_BlockType) :
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
		BlockState & a_Block) override
	{
		cChunkInterface ChunkInterface(a_World->GetChunkMap());
		cBlockHandler::For(m_BlockType).GetPlacementBlockTypeMeta(ChunkInterface, *a_Player, a_PlacedBlockPos, a_ClickedBlockFace, a_CursorPos, a_Block);
		return true;
	}

private:

	BlockType m_BlockType;
};
