
#pragma once

#include "ItemHandler.h"
#include "Blocks/BlockAnvil.h"





class cItemAnvilHandler :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

private:

	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		return a_Player.PlaceBlock(
			a_PlacePosition,
			static_cast<BLOCKTYPE>(a_HeldItem.m_ItemType),
			cBlockAnvilHandler::YawToMetaData(a_Player.GetYaw()) | static_cast<NIBBLETYPE>(a_HeldItem.m_ItemDamage << 2)
		);
	}
};
