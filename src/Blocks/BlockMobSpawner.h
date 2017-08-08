
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


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// No pickups
	}


	virtual void OnDestroyedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		cItemHandler * Handler = a_Player.GetEquippedItem().GetHandler();
		if (a_Player.IsGameModeCreative() || !Handler->CanHarvestBlock(E_BLOCK_MOB_SPAWNER))
		{
			return;
		}

		auto & Random = GetRandomProvider();
		int Reward = 15 + Random.RandInt(14) + Random.RandInt(14);
		a_WorldInterface.SpawnExperienceOrb(static_cast<double>(a_BlockX), static_cast<double>(a_BlockY + 1), static_cast<double>(a_BlockZ), Reward);
	}
} ;
