
#pragma once

#include "BlockHandler.h"





class cBlockTallGrassHandler :
	public cBlockHandler
{
public:
	cBlockTallGrassHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}


	virtual bool DoesIgnoreBuildCollision(void) override
	{
		return true;
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Drop seeds, sometimes
		MTRand r1;
		if (r1.randInt(10) == 5)
		{
			a_Pickups.push_back(cItem(E_ITEM_SEEDS, 1, 0));
		}
	}


	virtual void OnDestroyedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		
		if ((!a_Player->IsGameModeCreative()) && (a_Player->GetEquippedItem().m_ItemType == E_ITEM_SHEARS))
		{
			cItems Pickups;
			Pickups.Add(E_BLOCK_TALL_GRASS, 1, Meta);
			a_WorldInterface.SpawnItemPickups(Pickups, a_BlockX, a_BlockY, a_BlockZ);

			a_Player->UseEquippedItem();
		}
	}


	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		return ((a_RelY > 0) && (a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ) != E_BLOCK_AIR));
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.grass";
	}
} ;




