
#pragma once

#include "BlockPlant.h"
#include "../FastRandom.h"





/** Common class that takes care of beetroots, carrots, potatoes and wheat */
template <NIBBLETYPE RipeMeta>
class cBlockCropsHandler:
	public cBlockPlant<true>
{
	using Super = cBlockPlant<true>;

public:

	using Super::Super;

private:

	static char FortuneBinomialRandom(char a_Min, char a_Rolls)
	{
		auto & random = GetRandomProvider();
		char DropNum = a_Min;
		for (unsigned char i=0; i<a_Rolls; i++)
		{
			if (random.RandBool(0.57))
			{
				DropNum++;
			}
		}
		return DropNum;
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) const override
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
				const char SeedCount = FortuneBinomialRandom(0, 3 + ToolFortuneLevel(a_Tool));
				res.Add(E_ITEM_BEETROOT_SEEDS, SeedCount, 0);
				res.Add(E_ITEM_BEETROOT, 1, 0);
				break;
			}
			case E_BLOCK_CROPS:
			{
				res.Add(E_ITEM_WHEAT, 1, 0);
				const char SeedCount = FortuneBinomialRandom(1, 3 + ToolFortuneLevel(a_Tool));
				res.Add(E_ITEM_SEEDS, SeedCount, 0);
				break;
			}
			case E_BLOCK_CARROTS:
			{
				const char CarrotCount = FortuneBinomialRandom(1, 4 + ToolFortuneLevel(a_Tool));
				res.Add(E_ITEM_CARROT, CarrotCount, 0);
				break;
			}
			case E_BLOCK_POTATOES:
			{
				const char PotatoCount = FortuneBinomialRandom(2, 3 + ToolFortuneLevel(a_Tool));
				res.Add(E_ITEM_POTATO, PotatoCount, 0);
				if (rand.RandBool(0.02))
				{
					// With a 2% chance, drop a poisonous potato as well
					res.Add(E_ITEM_POISONOUS_POTATO, 1, 0);
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





	virtual int Grow(cChunk & a_Chunk, Vector3i a_RelPos, int a_NumStages = 1) const override
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





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		return ((a_RelPos.y > 0) && (a_Chunk.GetBlock(a_RelPos.addedY(-1)) == E_BLOCK_FARMLAND));
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 7;
	}
} ;




