
#pragma once

#include "ItemHandler.h"
#include "../BlockInfo.h"





class cItemBigFlowerHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemBigFlowerHandler():
		Super(E_BLOCK_BIG_FLOWER)
	{
	}





	virtual bool OnPlacementCommit(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		// Needs at least two free blocks to build in:
		if (a_PlacePosition.y >= (cChunkDef::Height - 1))
		{
			return false;
		}

		const auto & World = *a_Player.GetWorld();
		const auto TopPos = a_PlacePosition.addedY(1);
		BLOCKTYPE TopType;
		NIBBLETYPE TopMeta;
		World.GetBlockTypeMeta(TopPos, TopType, TopMeta);

		if (!cBlockHandler::For(TopType).DoesIgnoreBuildCollision(World, a_HeldItem, TopPos, TopMeta, a_ClickedBlockFace, false))
		{
			return false;
		}

		return a_Player.PlaceBlocks(
		{
			{ a_PlacePosition, E_BLOCK_BIG_FLOWER, static_cast<NIBBLETYPE>(a_HeldItem.m_ItemDamage & 0x07) },
			{ TopPos,          E_BLOCK_BIG_FLOWER, E_META_BIG_FLOWER_TOP }
		});
	}
};
