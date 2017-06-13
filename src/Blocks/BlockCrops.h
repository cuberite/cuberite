
#pragma once

#include "BlockPlant.h"
#include "../FastRandom.h"





/** Common class that takes care of beetroots, carrots, potatoes and wheat */
template <NIBBLETYPE RipeMeta>
class cBlockCropsHandler :
	public cBlockPlant
{
	typedef cBlockPlant Super;

public:

	cBlockCropsHandler(BLOCKTYPE a_BlockType):
		Super(a_BlockType, true)
	{
	}



	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_Meta) override
	{
		auto & rand = GetRandomProvider();

		// If not fully grown, drop the "seed" of whatever is growing:
		if (a_Meta < RipeMeta)
		{
			switch (m_BlockType)
			{
				case E_BLOCK_BEETROOTS: a_Pickups.push_back(cItem(E_ITEM_BEETROOT_SEEDS, 1, 0)); break;
				case E_BLOCK_CROPS:     a_Pickups.push_back(cItem(E_ITEM_SEEDS,          1, 0)); break;
				case E_BLOCK_CARROTS:   a_Pickups.push_back(cItem(E_ITEM_CARROT,         1, 0)); break;
				case E_BLOCK_POTATOES:  a_Pickups.push_back(cItem(E_ITEM_POTATO,         1, 0)); break;
				default:
				{
					ASSERT(!"Unhandled block type");
					break;
				}
			}
			return;
		}

		// Fully grown, drop the crop's produce:
		switch (m_BlockType)
		{
			case E_BLOCK_BEETROOTS:
			{
				char SeedCount = 1 + ((rand.RandInt<char>(2) + rand.RandInt<char>(2)) / 2);  // [1 .. 3] with high preference of 2
				a_Pickups.emplace_back(E_ITEM_BEETROOT_SEEDS, SeedCount, 0);
				char BeetrootCount = 1 + ((rand.RandInt<char>(2) + rand.RandInt<char>(2)) / 2);  // [1 .. 3] with high preference of 2
				a_Pickups.emplace_back(E_ITEM_BEETROOT, BeetrootCount, 0);
				break;
			}
			case E_BLOCK_CROPS:
			{
				a_Pickups.emplace_back(E_ITEM_WHEAT, 1, 0);
				a_Pickups.emplace_back(E_ITEM_SEEDS, 1 + ((rand.RandInt<char>(2) + rand.RandInt<char>(2)) / 2), 0);  // [1 .. 3] with high preference of 2
				break;
			}
			case E_BLOCK_CARROTS:
			{
				a_Pickups.emplace_back(E_ITEM_CARROT, 1 + ((rand.RandInt<char>(2) + rand.RandInt<char>(2)) / 2), 0);  // [1 .. 3] with high preference of 2
				break;
			}
			case E_BLOCK_POTATOES:
			{
				a_Pickups.emplace_back(E_ITEM_POTATO, 1 + ((rand.RandInt<char>(2) + rand.RandInt<char>(2)) / 2), 0);  // [1 .. 3] with high preference of 2
				if (rand.RandBool(0.05))
				{
					// With a 5% chance, drop a poisonous potato as well
					a_Pickups.emplace_back(E_ITEM_POISONOUS_POTATO, 1, 0);
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



	virtual void OnUpdate(cChunkInterface & cChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_PluginInterface, cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ) override
	{
		NIBBLETYPE Meta = a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ);

		// Check to see if the plant can grow
		auto Action = CanGrow(a_Chunk, a_RelX, a_RelY, a_RelZ);

		// If there is still room to grow and the plant can grow, then grow.
		// Otherwise if the plant needs to die, then dig it up
		if ((Meta < RipeMeta) && (Action == paGrowth))
		{
			a_Chunk.FastSetBlock(a_RelX, a_RelY, a_RelZ, m_BlockType, ++Meta);
		}
		else if (Action == paDeath)
		{
			a_Chunk.GetWorld()->DigBlock(a_RelX + a_Chunk.GetPosX() * cChunkDef::Width, a_RelY, a_RelZ + a_Chunk.GetPosZ() * cChunkDef::Width);
		}
	}



	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		return ((a_RelY > 0) && (a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ) == E_BLOCK_FARMLAND));
	}



	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 7;
	}
} ;




