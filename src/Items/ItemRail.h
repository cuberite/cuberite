
#pragma once

#include "ItemHandler.h"
#include "Blocks/BlockRail.h"





class cItemRailHandler final :
	public cSimplePlaceableItemHandler
{
	using Super = cSimplePlaceableItemHandler;

public:

	using Super::Super;

private:

	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		cChunkInterface ChunkInterface(a_Player.GetWorld()->GetChunkMap());
		BlockState RailType = cBlockRailHandler::GetRailFromRotation(BlockItemConverter::FromItem(a_HeldItem.m_ItemType), a_Player.GetYaw());
		return a_Player.PlaceBlock(a_PlacePosition, cBlockRailHandler::FindBlock(ChunkInterface, a_PlacePosition, RailType));
	}
};
