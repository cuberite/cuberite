#pragma once

#include "ItemHandler.h"
#include "Blocks/BlockGlazedTerracotta.h"





class cItemGlazedTerracottaHandler :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

private:

	virtual bool OnPlacementCommit(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		return a_Player.PlaceBlock(a_PlacePosition, static_cast<BLOCKTYPE>(a_HeldItem.m_ItemType), cBlockGlazedTerracottaHandler::YawToMetaData(a_Player.GetYaw()));
	}
};
