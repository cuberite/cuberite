
#pragma once

#include "BlockHandler.h"
#include "../MersenneTwister.h"
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

	
	void OnUpdate(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		NIBBLETYPE Meta = a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		if (Meta >= 7)
		{
			// Grow the produce:
			a_World->GrowMelonPumpkin(a_BlockX, a_BlockY, a_BlockZ, m_BlockType);
		}
		else
		{
			// Grow the stem:
			a_World->FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, m_BlockType, Meta + 1);
		}
	}


	virtual bool CanBeAt(int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		return ((a_RelY > 0) && (a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ) == E_BLOCK_FARMLAND));
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
} ;




