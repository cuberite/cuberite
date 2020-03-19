
#pragma once

#include "BlockHandler.h"
#include "../Items/ItemHandler.h"





class cBlockMobSpawnerHandler :
	public cBlockHandler
{
public:
	cBlockMobSpawnerHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}


	virtual bool OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		return a_ChunkInterface.UseBlockEntity(&a_Player, a_BlockX, a_BlockY, a_BlockZ);
	}


	virtual bool IsUseable() override
	{
		return true;
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		// No pickups
		return {};
	}





	virtual void OnPlayerBrokeBlock(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		Vector3i a_BlockPos,
		BLOCKTYPE a_OldBlockType, NIBBLETYPE a_OldBlockMeta
	) override
	{
		auto handler = a_Player.GetEquippedItem().GetHandler();
		if (!a_Player.IsGameModeSurvival() || !handler->CanHarvestBlock(E_BLOCK_MOB_SPAWNER))
		{
			return;
		}

		auto & random = GetRandomProvider();
		int reward = 15 + random.RandInt(14) + random.RandInt(14);
		a_WorldInterface.SpawnSplitExperienceOrbs(Vector3d(0.5, 0.5, 0.5) + a_BlockPos, reward);
	}
} ;
