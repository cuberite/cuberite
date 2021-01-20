
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"





class cItemEndCrystalHandler : public cItemHandler
{
	using Super = cItemHandler;

  public:
	cItemEndCrystalHandler(int a_ItemType) : Super(a_ItemType) {}





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

		auto * a_ChunkMap = a_World->GetChunkMap();

		cChunkInterface ChunkInterface(a_ChunkMap);

		// Don't place if two blocks above placement block aren't air
		if (ChunkInterface.GetBlock(a_BlockPos + Vector3i(0, 1, 0)) != E_BLOCK_AIR  ||
			ChunkInterface.GetBlock(a_BlockPos + Vector3i(0, 2, 0)) != E_BLOCK_AIR )
		{
			return false;
		}

		auto a_BlockType = ChunkInterface.GetBlock(a_BlockPos);

		// Dont't place if placement block is not obsidian or bedrock
		if (a_BlockType != E_BLOCK_OBSIDIAN && a_BlockType != E_BLOCK_BEDROCK)
		{
			return false;
		}

		// Checks if there are end crystals in bounding box
		bool CanBePlaced = a_ChunkMap->ForEachEntityInBox(cBoundingBox(a_BlockPos,Vector3d(a_BlockPos.x + 1.0, a_BlockPos.y + 2.0, a_BlockPos.z + 1.0)),
			[](cEntity & a_Entity) -> bool
			{
				return a_Entity.IsEnderCrystal();
			}
		);

		if (!CanBePlaced)
		{
			return false;
		}

		// Spawns ender crystal entity
		a_World->SpawnEnderCrystal(Vector3d(a_BlockPos.x + 0.5, a_BlockPos.y + 1.0, a_BlockPos.z + 0.5), false);

		if (!a_Player->IsGameModeCreative())
		{
			a_Player->GetInventory().RemoveOneEquippedItem();
		}

		return true;
	}
};
