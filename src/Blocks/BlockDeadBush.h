
#pragma once

#include "BlockHandler.h"





class cBlockDeadBushHandler :
	public cBlockHandler
{
public:
	cBlockDeadBushHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		if (a_RelY <= 0)
		{
			return false;
		}

		BLOCKTYPE BelowBlock = a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ);
		switch (BelowBlock)
		{
			case E_BLOCK_CLAY:
			case E_BLOCK_HARDENED_CLAY:
			case E_BLOCK_STAINED_CLAY:
			case E_BLOCK_SAND:
			{
				return true;
			}
			default: return false;
		}
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 0;
	}

	virtual void OnDestroyedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		if ((!a_Player->IsGameModeCreative()) && (a_Player->GetEquippedItem().m_ItemType != E_ITEM_SHEARS))
		{
			cFastRandom random;
			int chance = random.NextInt(3);
			if (chance != 0)
			{
				cItems Pickups;
				Pickups.push_back(cItem(E_ITEM_STICK, static_cast<char>(chance), 0));
				a_WorldInterface.SpawnItemPickups(Pickups, a_BlockX, a_BlockY, a_BlockZ);
			}
		}
	}
} ;




