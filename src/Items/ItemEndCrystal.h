
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"





class cItemEndCrystalHandler :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemEndCrystalHandler(int a_ItemType) :
		Super(a_ItemType)
	{
	}


	virtual bool OnItemUse(
		cWorld * a_World, cPlayer * a_Player,
		cBlockPluginInterface & a_PluginInterface, const cItem & a_HeldItem,
		const Vector3i a_BlockPos,
		eBlockFace a_ClickedBlockFace) override
	{
		// Must click a valid block:
		if (a_ClickedBlockFace < 0)
		{
			return false;
		}

		if (
			const auto BlockType = a_World->GetBlock(a_BlockPos);

			// Don't place if placement block is not obsidian or bedrock:
			(BlockType != E_BLOCK_OBSIDIAN) && (BlockType != E_BLOCK_BEDROCK)
		)
		{
			return false;
		}

		// The position of the block above the placement block.
		const auto Above = a_BlockPos.addedY(1);

		if (
			// Don't place if two blocks above placement block aren't air:
			((Above.y != cChunkDef::Height) && (a_World->GetBlock(Above) != E_BLOCK_AIR)) ||
			((Above.y < (cChunkDef::Height - 1)) && (a_World->GetBlock(Above.addedY(1)) != E_BLOCK_AIR)) ||

			// Refuse placement if there are any entities in a (1 by 2 by 1) bounding box with base at the block above:
			!a_World->ForEachEntityInBox(
				{ Above, Above + Vector3i(1, 2, 1) },
				[](cEntity & a_Entity)
				{
					return true;
				}
			)
		)
		{
			return false;
		}

		// Spawns ender crystal entity, aborts if plugin cancelled:
		if (a_World->SpawnEnderCrystal(Vector3d(0.5, 0, 0.5) + Above, false) == cEntity::INVALID_ID)
		{
			return false;
		}

		if (!a_Player->IsGameModeCreative())
		{
			a_Player->GetInventory().RemoveOneEquippedItem();
		}

		return true;
	}
};
