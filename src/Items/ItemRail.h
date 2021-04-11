#pragma once

#include "ItemHandler.h"
#include "Blocks/BlockRail.h"





class cItemRailHandler :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

private:

	virtual bool OnPlacementCommit(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		cChunkInterface ChunkInterface(a_Player.GetWorld()->GetChunkMap());
		const auto RailType = static_cast<BLOCKTYPE>(a_HeldItem.m_ItemType);
		return a_Player.PlaceBlock(a_PlacePosition, RailType, cBlockRailHandler::FindMeta(ChunkInterface, a_PlacePosition, RailType));
	}
};
