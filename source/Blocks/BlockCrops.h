
#pragma once

#include "BlockHandler.h"
#include "../MersenneTwister.h"
#include "../World.h"





/// Common class that takes care of carrots, potatoes and wheat
class cBlockCropsHandler :
	public cBlockHandler
{
public:
	cBlockCropsHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}


	virtual bool DoesAllowBlockOnTop() override
	{
		return false;
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_Meta) override
	{
		MTRand rand;

		if (a_Meta == 0x7)
		{
			// Is fully grown, drop the entire produce:
			switch (m_BlockType)
			{
				case E_BLOCK_CROPS:
				{
					a_Pickups.push_back(cItem(E_ITEM_WHEAT, 1, 0));
					a_Pickups.push_back(cItem(E_ITEM_SEEDS, 1 + (int)(rand.randInt(2) + rand.randInt(2)) / 2, 0));  // [1 .. 3] with high preference of 2
					break;
				}
				case E_BLOCK_CARROTS:
				{
					a_Pickups.push_back(cItem(E_ITEM_CARROT, 1 + (int)(rand.randInt(2) + rand.randInt(2)) / 2, 0));  // [1 .. 3] with high preference of 2
					break;
				}
				case E_BLOCK_POTATOES:
				{
					a_Pickups.push_back(cItem(E_ITEM_POTATO, 1 + (int)(rand.randInt(2) + rand.randInt(2)) / 2, 0));  // [1 .. 3] with high preference of 2
					if (rand.randInt(20) == 0)
					{
						// With a 5% chance, drop a poisonous potato as well
						a_Pickups.push_back(cItem(E_ITEM_POISONOUS_POTATO, 1, 0));
					}
					break;
				}
				default:
				{
					ASSERT(!"Unhandled block type");
					break;
				}
			}  // switch (m_BlockType)
		}
		else
		{
			// Drop 1 item of whatever is growing
			switch (m_BlockType)
			{
				case E_BLOCK_CROPS:    a_Pickups.push_back(cItem(E_ITEM_SEEDS,  1, 0)); break;
				case E_BLOCK_CARROTS:  a_Pickups.push_back(cItem(E_ITEM_CARROT, 1, 0)); break;
				case E_BLOCK_POTATOES: a_Pickups.push_back(cItem(E_ITEM_POTATO, 1, 0)); break;
				default:
				{
					ASSERT(!"Unhandled block type");
					break;
				}
			}
		}
	}	
	
	
	void OnUpdate(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		NIBBLETYPE Meta = a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		if (Meta < 7)
		{
			a_World->FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_CROPS, ++Meta);
		}
	}


	virtual bool CanBeAt(int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		return ((a_RelY > 0) && (a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ) == E_BLOCK_FARMLAND));
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.grass";
	}
} ;




