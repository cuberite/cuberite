
#pragma once

#include "BlockPlant.h"
#include "../FastRandom.h"
#include <iostream>





/** Common class that takes care of beetroots, carrots, potatoes and wheat */
template <NIBBLETYPE RipeMeta>
class cBlockCropsHandler final :
	public cBlockPlant<true>
{
	using Super = cBlockPlant<true>;

public:

	using Super::Super;

private:

	/** Calculate the number of seeds to drop when the crop is broken. */
	static char CalculateSeedCount(char a_Min, char a_BaseRolls, unsigned char a_FortuneLevel)
	{
		std::binomial_distribution<> Binomial(a_BaseRolls + a_FortuneLevel, 0.57);
		return static_cast<char>(a_Min + Binomial(GetRandomProvider().Engine()));
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		auto & rand = GetRandomProvider();

		// If not fully grown, drop the "seed" of whatever is growing:
		if (a_BlockMeta < RipeMeta)
		{
			switch (m_BlockType)
			{
				case E_BLOCK_BEETROOTS: return cItem(E_ITEM_BEETROOT_SEEDS);
				case E_BLOCK_CROPS:     return cItem(E_ITEM_SEEDS);
				case E_BLOCK_CARROTS:   return cItem(E_ITEM_CARROT);
				case E_BLOCK_POTATOES:  return cItem(E_ITEM_POTATO);
			}

			ASSERT(!"Unhandled block type");
			return {};
		}

		// Fully grown, drop the crop's produce:
		cItems res;
		// Get current fortune level
		int fortune = 0;
		if (a_Tool)
		{
			fortune = a_Tool->m_Enchantments.GetLevel(cEnchantments::eEnchantment::enchFortune);
		}

		switch (m_BlockType)
		{
			case E_BLOCK_BEETROOTS:
			{
				const auto SeedCount = CalculateSeedCount(0, 3, ToolFortuneLevel(a_Tool));
				res.Add(E_ITEM_BEETROOT_SEEDS, SeedCount);
				res.Add(E_ITEM_BEETROOT);
				break;
			}
			case E_BLOCK_CROPS:
			{
				// https://minecraft.fandom.com/wiki/Seeds_(Wheat)
				res.Add(E_ITEM_WHEAT);
				const auto SeedCount = CalculateSeedCount(1, 3, ToolFortuneLevel(a_Tool));
				res.Add(E_ITEM_SEEDS, SeedCount);
				break;
			}
			case E_BLOCK_CARROTS:
			{
				// https://minecraft.gamepedia.com/Carrot#Breaking
				const auto CarrotCount = CalculateSeedCount(1, 4, ToolFortuneLevel(a_Tool));
				res.Add(E_ITEM_CARROT, CarrotCount);
				break;
			}
			case E_BLOCK_POTATOES:
			{
				// https://minecraft.gamepedia.com/Potato#Breaking
				const auto PotatoCount = CalculateSeedCount(2, 3, ToolFortuneLevel(a_Tool));
				res.Add(E_ITEM_POTATO, PotatoCount);
				if (rand.RandBool(0.02))
				{
					// https://minecraft.gamepedia.com/Poisonous_Potato#Obtaining
					// With a 2% chance, drop a poisonous potato as well:
					res.Add(E_ITEM_POISONOUS_POTATO);
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





private:

	/**
		Generate a random number using a binomial distribution
		See https://minecraft.gamepedia.com/Fortune#Seeds
	*/
	int GenerateAmountSeedDrop(int a_Min, int a_Count, double a_Probability, MTRand &a_Rand)
	{
		int SeedCount = a_Min;
		for (int i(0); i < a_Count; i++)
		{
			if (a_Rand.RandBool(a_Probability))
			{
				SeedCount++;
			}
		}
		return SeedCount;
	}
} ;



