
#pragma once

#include "BlockPlant.h"
#include "../FastRandom.h"





/** Common class that takes care of beetroots, carrots, potatoes and wheat */
class cBlockCropsHandler final :
	public cBlockPlant<true>
{
	using Super = cBlockPlant<true>;

public:

	using Super::Super;

	static inline unsigned char BeetrootsMaxAge = 3;
	static inline unsigned char CarrotsMaxAge = 7;
	static inline unsigned char PotatoesMaxAge = 7;
	static inline unsigned char WheatMaxAge = 7;

	static inline bool IsBlockCrop(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::Beetroots:
			case BlockType::Carrots:
			case BlockType::Potatoes:
			case BlockType::Wheat:
				return true;
			default: return false;
		}
	}

	static inline bool IsFullyGrown(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::Beetroots: return (Beetroots::Age(a_Block) == BeetrootsMaxAge);
			case BlockType::Carrots: return (Carrots::Age(a_Block) == CarrotsMaxAge);
			case BlockType::Potatoes: return (Potatoes::Age(a_Block) == PotatoesMaxAge);
			case BlockType::Wheat: return (Wheat::Age(a_Block) == WheatMaxAge);
			default: return false;
		}
	}

	bool FullyOccupiesVoxel(const BlockState a_Block) const override { return false; }

private:

	/** Calculate the number of seeds to drop when the crop is broken. */
	static char CalculateSeedCount(char a_Min, char a_BaseRolls, unsigned char a_FortuneLevel)
	{
		std::binomial_distribution<> Binomial(a_BaseRolls + a_FortuneLevel, 0.57);
		return static_cast<char>(a_Min + Binomial(GetRandomProvider().Engine()));
	}





	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		using namespace Block;
		auto & Rand = GetRandomProvider();
		switch (a_Block.Type())
		{
			case BlockType::Beetroots:
			{
				if (Beetroots::Age(a_Block) < BeetrootsMaxAge)
				{
					return cItem(Item::BeetrootSeeds);
				}
				const auto SeedCount = CalculateSeedCount(0, 3, ToolFortuneLevel(a_Tool));
				cItems Res;
				Res.Add(Item::BeetrootSeeds, SeedCount);
				Res.Add(Item::Beetroot);
				return Res;
			}
			case BlockType::Carrots:
			{
				if (Carrots::Age(a_Block) < CarrotsMaxAge)
				{
					return cItem(Item::Carrot);
				}
				cItems Res;
				// https://minecraft.gamepedia.com/Carrot#Breaking
				const auto CarrotCount = CalculateSeedCount(1, 4, ToolFortuneLevel(a_Tool));
				Res.Add(Item::Carrot, CarrotCount);
				return Res;
			}
			case BlockType::Potatoes:
			{
				if (Potatoes::Age(a_Block) < PotatoesMaxAge)
				{
					return cItem(Item::Potato);
				}
				cItems Res;
				// https://minecraft.gamepedia.com/Potato#Breaking
				const auto PotatoCount = CalculateSeedCount(2, 3, ToolFortuneLevel(a_Tool));
				Res.Add(Item::Potato, PotatoCount);
				if (Rand.RandBool(0.02))
				{
					// https://minecraft.wiki/w/Poisonous_Potato#Obtaining
					// With a 2% chance, drop a poisonous potato as well:
					Res.Add(Item::PoisonousPotato);
				}
				return Res;
			}
			case BlockType::Wheat:
			{
				if (Wheat::Age(a_Block) < WheatMaxAge)
				{
					return cItem(Item::WheatSeeds);
				}
				cItems Res;
				// https://minecraft.fandom.com/wiki/Seeds_(Wheat)
				Res.Add(Item::Wheat);
				const auto SeedCount = CalculateSeedCount(1, 3, ToolFortuneLevel(a_Tool));
				Res.Add(Item::WheatSeeds, SeedCount);
				return Res;
			}
			default:
			{
				ASSERT(!"Unhandled block type");
				return {};
			}
		}
	}





	virtual int Grow(cChunk & a_Chunk, Vector3i a_RelPos, char a_NumStages = 1) const override
	{
		using namespace Block;
		const auto OldSelf = a_Chunk.GetBlock(a_RelPos);
		const auto NumStages = static_cast<unsigned char>(std::clamp<char>(a_NumStages, 0, std::numeric_limits<char>::max()));

		switch (OldSelf.Type())
		{
			case BlockType::Beetroots:
			{
				auto OldAge = Beetroots::Age(OldSelf);
				auto NewAge = std::clamp<unsigned char>(OldAge + NumStages, 0, BeetrootsMaxAge);
				a_Chunk.SetBlock(a_RelPos, Beetroots::Beetroots(NewAge));
				return NewAge - OldAge;
			}
			case BlockType::Carrots:
			{
				auto OldAge = Carrots::Age(OldSelf);
				auto NewAge = std::clamp<unsigned char>(OldAge + NumStages, 0, CarrotsMaxAge);
				a_Chunk.SetBlock(a_RelPos, Carrots::Carrots(NewAge));
				return NewAge - OldAge;
			}
			case BlockType::Potatoes:
			{
				auto OldAge = Potatoes::Age(OldSelf);
				auto NewAge = std::clamp<unsigned char>(OldAge + NumStages, 0, PotatoesMaxAge);
				a_Chunk.SetBlock(a_RelPos, Potatoes::Potatoes(NewAge));
				return NewAge - OldAge;
			}
			case BlockType::Wheat:
			{
				auto OldAge = Wheat::Age(OldSelf);
				auto NewAge = std::clamp<unsigned char>(OldAge + NumStages, 0, WheatMaxAge);
				a_Chunk.SetBlock(a_RelPos, Wheat::Wheat(NewAge));
				return NewAge - OldAge;
			}
			default: return 0;
		}
	}





	virtual bool CanBeAt(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Self) const override
	{
		const auto BlockBelow = a_Position.addedY(-1);

		if (!cChunkDef::IsValidHeight(BlockBelow))
		{
			return false;
		}

		return a_Chunk.GetBlock(BlockBelow) == BlockType::Farmland;
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 7;
	}
};
