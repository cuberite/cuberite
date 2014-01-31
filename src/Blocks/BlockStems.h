
#pragma once

#include "BlockHandler.h"
#include "../World.h"





class cBlockStemsHandler :
	public cBlockHandler
{
public:
	cBlockStemsHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		int ItemType = (m_BlockType == E_BLOCK_MELON_STEM) ? E_ITEM_MELON_SEEDS : E_ITEM_PUMPKIN_SEEDS;
		a_Pickups.push_back(cItem(ItemType, 1, 0));
	}

	
	void OnUpdate(cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ) override
	{
		NIBBLETYPE Meta = a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ);
		if (Meta >= 7)
		{
			// Grow the produce:
			int BlockX = a_RelX + a_Chunk.GetPosX() * cChunkDef::Width;
			int BlockZ = a_RelZ + a_Chunk.GetPosZ() * cChunkDef::Width;
			a_Chunk.GetWorld()->GrowMelonPumpkin(BlockX, a_RelY, BlockZ, m_BlockType);
		}
		else
		{
			// Grow the stem:
			a_Chunk.FastSetBlock(a_RelX, a_RelY, a_RelZ, m_BlockType, Meta + 1);
		}
	}


	virtual bool CanBeAt(cChunkInterface * a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		return ((a_RelY > 0) && (a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ) == E_BLOCK_FARMLAND));
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
} ;




