
#pragma once

#include "BlockPlant.h"
#include "../FastRandom.h"





/** Common class that takes care of beetroots, carrots, potatoes and wheat */
template <NIBBLETYPE RipeMeta>
class cBlockCropsHandler:
	public cBlockPlant<true>
{
	using super = cBlockPlant<true>;

public:

	cBlockCropsHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		auto & rand = GetRandomProvider();

		// If not fully grown, drop the "seed" of whatever is growing:
		if (a_BlockMeta < RipeMeta)
		{
			switch (m_BlockType)
			{
				case E_BLOCK_BEETROOTS: return cItem(E_ITEM_BEETROOT_SEEDS, 1, 0); break;
				case E_BLOCK_CROPS:     return cItem(E_ITEM_SEEDS,          1, 0); break;
				case E_BLOCK_CARROTS:   return cItem(E_ITEM_CARROT,         1, 0); break;
				case E_BLOCK_POTATOES:  return cItem(E_ITEM_POTATO,         1, 0); break;
			}
			ASSERT(!"Unhandled block type");
			return {};
		}

		// Fully grown, drop the crop's produce:
		cItems res;
		switch (m_BlockType)
		{
			case E_BLOCK_BEETROOTS:
			{
				char SeedCount = 1 + ((rand.RandInt<char>(2) + rand.RandInt<char>(2)) / 2);  // [1 .. 3] with high preference of 2
				res.Add(E_ITEM_BEETROOT_SEEDS, SeedCount, 0);
				char BeetrootCount = 1 + ((rand.RandInt<char>(2) + rand.RandInt<char>(2)) / 2);  // [1 .. 3] with high preference of 2
				res.Add(E_ITEM_BEETROOT, BeetrootCount, 0);
				break;
			}
			case E_BLOCK_CROPS:
			{
				res.Add(E_ITEM_WHEAT, 1, 0);
				res.Add(E_ITEM_SEEDS, 1 + ((rand.RandInt<char>(2) + rand.RandInt<char>(2)) / 2), 0);  // [1 .. 3] with high preference of 2
				break;
			}
			case E_BLOCK_CARROTS:
			{
				res.Add(E_ITEM_CARROT, 1 + ((rand.RandInt<char>(2) + rand.RandInt<char>(2)) / 2), 0);  // [1 .. 3] with high preference of 2
				break;
			}
			case E_BLOCK_POTATOES:
			{
				res.Add(E_ITEM_POTATO, 1 + ((rand.RandInt<char>(2) + rand.RandInt<char>(2)) / 2), 0);  // [1 .. 3] with high preference of 2
				if (rand.RandBool(0.05))
				{
					// With a 5% chance, drop a poisonous potato as well
					res.emplace_back(E_ITEM_POISONOUS_POTATO, 1, 0);
				}
				break;
			}
			default:
			{
				ASSERT(!"Unhandled block type");
				break;
			}
		}  // switch (m_BlockType)
		return res;
	}





	virtual int Grow(cChunk & a_Chunk, Vector3i a_RelPos, int a_NumStages = 1) override
	{
		auto oldMeta = a_Chunk.GetMeta(a_RelPos);
		if (oldMeta >= RipeMeta)
		{
			// Already ripe
			return 0;
		}
		auto newMeta = std::min<int>(oldMeta + a_NumStages, RipeMeta);
		ASSERT(newMeta > oldMeta);
		a_Chunk.GetWorld()->SetBlock(a_Chunk.RelativeToAbsolute(a_RelPos), m_BlockType, static_cast<NIBBLETYPE>(newMeta));
		return newMeta - oldMeta;
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




