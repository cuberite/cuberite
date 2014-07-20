
#pragma once

#include "BlockHandler.h"





class cBlockBigFlowerHandler :
	public cBlockHandler
{
public:
	typedef cBlockHandler super;

	cBlockBigFlowerHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}


	virtual void DropBlock(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_BlockPluginInterface, cEntity * a_Digger, int a_BlockX, int a_BlockY, int a_BlockZ, bool a_DropVerbatim) override
	{
		NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		if (Meta & 0x8)
		{
			super::DropBlock(a_ChunkInterface, a_WorldInterface, a_BlockPluginInterface, a_Digger, a_BlockX, a_BlockY - 1, a_BlockZ, a_DropVerbatim);
		}
		else
		{
			super::DropBlock(a_ChunkInterface, a_WorldInterface, a_BlockPluginInterface, a_Digger, a_BlockX, a_BlockY, a_BlockZ, a_DropVerbatim);
		}
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		NIBBLETYPE Meta = a_BlockMeta & 0x7;
		
		if ((Meta == 2) || (Meta == 3))
		{
			return;
		}
		
		a_Pickups.push_back(cItem(E_BLOCK_BIG_FLOWER, 1, Meta));
	}


	virtual void OnDestroyedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		if (Meta & 0x8)
		{
			Meta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY - 1, a_BlockZ);
		}
		
		NIBBLETYPE FlowerMeta = Meta & 0x7;
		if (!a_Player->IsGameModeCreative())
		{
			if (((FlowerMeta == 2) || (FlowerMeta == 3)) && (a_Player->GetEquippedItem().m_ItemType == E_ITEM_SHEARS))
			{
				MTRand r1;
				if (r1.randInt(10) == 5)
				{
					cItems Pickups;
					if (FlowerMeta == 2)
					{
						Pickups.Add(E_BLOCK_TALL_GRASS, 2, 1);
					}
					else if (FlowerMeta == 3)
					{
						Pickups.Add(E_BLOCK_TALL_GRASS, 2, 2);
					}
					a_WorldInterface.SpawnItemPickups(Pickups, a_BlockX, a_BlockY, a_BlockZ);
				}
				a_Player->UseEquippedItem();
			}
		}
	}


	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		return ((a_RelY > 0) && (a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ) != E_BLOCK_AIR) && (a_RelY < cChunkDef::Height) && ((a_Chunk.GetBlock(a_RelX, a_RelY + 1, a_RelZ) == E_BLOCK_AIR) || (a_Chunk.GetBlock(a_RelX, a_RelY + 1, a_RelZ) == E_BLOCK_BIG_FLOWER)));
	}


	virtual void OnPlacedByPlayer(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta
	) override
	{
		int Meta = (((int)floor(a_Player->GetYaw() * 4.0 / 360.0 + 0.5) & 0x3) + 2) % 4;
		a_ChunkInterface.FastSetBlock(a_BlockX, a_BlockY + 1, a_BlockZ, m_BlockType, 0x8 | Meta);
	}


	virtual void OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		NIBBLETYPE OldMeta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);

		if (OldMeta & 0x8)
		{
			// Was upper part of flower
			if (a_ChunkInterface.GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ) == m_BlockType)
			{
				a_ChunkInterface.FastSetBlock(a_BlockX, a_BlockY - 1, a_BlockZ, E_BLOCK_AIR, 0);
			}
		}
		else
		{
			// Was lower part
			if (a_ChunkInterface.GetBlock(a_BlockX, a_BlockY + 1, a_BlockZ) == m_BlockType)
			{
				a_ChunkInterface.FastSetBlock(a_BlockX, a_BlockY + 1, a_BlockZ, E_BLOCK_AIR, 0);
			}
		}
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.grass";
	}
} ;




