
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





	virtual bool GetBlocksToPlace(
		cWorld & a_World,
		cPlayer & a_Player,
		const cItem & a_EquippedItem,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		sSetBlockVector & a_BlocksToPlace
	) override
	{
		// Can only be placed on dirt:
		if ((a_PlacedBlockPos.y <= 0) || !IsBlockTypeOfDirt(a_World.GetBlock(a_PlacedBlockPos.addedY(-1))))
		{
			return false;
		}

		// Needs at least two free blocks to build in
		if (a_PlacedBlockPos.y >= cChunkDef::Height - 1)
		{
			return false;
		}

		auto TopPos = a_PlacedBlockPos.addedY(1);
		BLOCKTYPE TopType;
		NIBBLETYPE TopMeta;
		a_World.GetBlockTypeMeta(TopPos, TopType, TopMeta);
		cChunkInterface ChunkInterface(a_World.GetChunkMap());

		if (!cBlockHandler::For(TopType).DoesIgnoreBuildCollision(ChunkInterface, TopPos, a_Player, TopMeta))
		{
			return false;
		}

		a_BlocksToPlace.emplace_back(a_PlacedBlockPos, E_BLOCK_BIG_FLOWER, a_EquippedItem.m_ItemDamage & 0x07);
		a_BlocksToPlace.emplace_back(TopPos,           E_BLOCK_BIG_FLOWER, E_META_BIG_FLOWER_TOP);
		return true;
	}
};
