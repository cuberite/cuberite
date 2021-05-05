
#pragma once

#include "BlockPlant.h"
#include "../FastRandom.h"





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





	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
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
		cItems Res;

		switch (m_BlockType)
		{
			case E_BLOCK_BEETROOTS:
			{
				const auto SeedCount = CalculateSeedCount(0, 3, ToolFortuneLevel(a_Tool));
				Res.Add(E_ITEM_BEETROOT_SEEDS, SeedCount);
				Res.Add(E_ITEM_BEETROOT);
				break;
			}
			case E_BLOCK_CROPS:
			{
				// https://minecraft.fandom.com/wiki/Seeds_(Wheat)
				Res.Add(E_ITEM_WHEAT);
				const auto SeedCount = CalculateSeedCount(1, 3, ToolFortuneLevel(a_Tool));
				Res.Add(E_ITEM_SEEDS, SeedCount);
				break;
			}
			case E_BLOCK_CARROTS:
			{
				// https://minecraft.gamepedia.com/Carrot#Breaking
				const auto CarrotCount = CalculateSeedCount(1, 4, ToolFortuneLevel(a_Tool));
				Res.Add(E_ITEM_CARROT, CarrotCount);
				break;
			}
			case E_BLOCK_POTATOES:
			{
				// https://minecraft.gamepedia.com/Potato#Breaking
				const auto PotatoCount = CalculateSeedCount(2, 3, ToolFortuneLevel(a_Tool));
				Res.Add(E_ITEM_POTATO, PotatoCount);
				if (rand.RandBool(0.02))
				{
					// https://minecraft.gamepedia.com/Poisonous_Potato#Obtaining
					// With a 2% chance, drop a poisonous potato as well:
					Res.Add(E_ITEM_POISONOUS_POTATO);
				}
				break;
			}
			default:
			{
				ASSERT(!"Unhandled block type");
				break;
			}
		}  // switch (m_BlockType)
		return Res;
	}





	virtual int Grow(cChunk & a_Chunk, Vector3i a_RelPos, int a_NumStages = 1) const override
	{
		const auto OldMeta = a_Chunk.GetMeta(a_RelPos);
		const auto NewMeta = std::clamp<NIBBLETYPE>(static_cast<NIBBLETYPE>(OldMeta + a_NumStages), 0, RipeMeta);
		a_Chunk.SetMeta(a_RelPos, NewMeta);
		return NewMeta - OldMeta;
	}





	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const NIBBLETYPE a_Meta) const override
	{
		return (a_Position.y > 0) && (a_Chunk.GetBlock(a_Position.addedY(-1)) == E_BLOCK_FARMLAND);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 7;
	}
};
