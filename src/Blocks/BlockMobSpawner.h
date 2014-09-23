
#pragma once

#include "BlockHandler.h"
#include "../World.h"
#include "../Items/ItemHandler.h"





class cBlockMobSpawnerHandler :
	public cBlockHandler
{
public:
	cBlockMobSpawnerHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// No pickups
	}
	
	
	virtual void OnDestroyedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		cItemHandler * Handler = a_Player->GetEquippedItem().GetHandler();
		if (a_Player->IsGameModeCreative() || !Handler->CanHarvestBlock(E_BLOCK_MOB_SPAWNER))
		{
			return;
		}

		cFastRandom Random;
		int Reward = 15 + Random.NextInt(15) + Random.NextInt(15);
		a_WorldInterface.SpawnExperienceOrb((double)a_BlockX, (double)a_BlockY + 1, (double)a_BlockZ, Reward);
	}
} ;
