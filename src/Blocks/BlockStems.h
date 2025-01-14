
#pragma once

#include "BlockPlant.h"





/** Handler for stems from which produce grows in an adjacent block (melon, pumpkin) after it becomes ripe (meta == 7).
ProduceBlockType is the blocktype for the produce to be grown.
StemPickupType is the item type for the pickup resulting from breaking the stem. */
template <BlockType ProduceBlockType, Item StemPickupType>
class cBlockStemsHandler final :
	public cBlockPlant<true>
{
	using Super = cBlockPlant<true>;

public:

	using Super::Super;

	static constexpr unsigned char MaxStemAge = 7;

	static inline bool IsBlockStem(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::PumpkinStem:
			case BlockType::AttachedPumpkinStem:
			case BlockType::MelonStem:
			case BlockType::AttachedMelonStem:
				return true;
			default: return false;
		}
	}

	static inline unsigned char GetStemAge(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::PumpkinStem:         return Block::PumpkinStem::Age(a_Block);
			case BlockType::AttachedPumpkinStem: return MaxStemAge;
			case BlockType::MelonStem:           return Block::MelonStem::Age(a_Block);
			case BlockType::AttachedMelonStem:   return MaxStemAge;
			default: return 0;
		}
	}





	bool FullyOccupiesVoxel(const BlockState a_Block) const override
	{
		return false;
	}

private:

	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		/*
			Use correct percent:
			https://minecraft.wiki/w/Melon_Seeds#Breaking
			https://minecraft.wiki/w/Pumpkin_Seeds#Breaking
		*/

		auto Age = GetStemAge(a_Block);
		// Age > 7 (Impossible)
		if (Age > MaxStemAge)
		{
			return cItem(StemPickupType);
		}

		const auto Threshold = GetRandomProvider().RandReal<double>(100);
		double Cumulative = 0;
		char Count = 0;

		for (; Count < 4; Count++)
		{
			Cumulative += m_AgeSeedDropProbability[static_cast<size_t>(Age)][static_cast<size_t>(Count)];
			if (Cumulative > Threshold)
			{
				break;
			}
		}

		return cItem(StemPickupType, Count);
	}





	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const BlockState a_BlockToReplace) const override
	{
		const auto BelowPos = a_Position.addedY(-1);
		return (cChunkDef::IsValidHeight(BelowPos) && (a_Chunk.GetBlock(BelowPos) == BlockType::Farmland));
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 7;
	}





	virtual int Grow(cChunk & a_Chunk, Vector3i a_Position, char a_NumStages = 1) const override
	{
		const auto OldAge = GetStemAge(a_Chunk.GetBlock(a_Position));
		const auto NewAge = std::clamp<unsigned char>(static_cast<unsigned char>(OldAge + a_NumStages), 0, 7);
		switch (m_BlockType)
		{
			case BlockType::PumpkinStem: a_Chunk.SetBlock(a_Position, Block::PumpkinStem::PumpkinStem(NewAge)); break;
			case BlockType::MelonStem:   a_Chunk.SetBlock(a_Position, Block::MelonStem::MelonStem(NewAge)); break;
			default: return 0;
		}
		return NewAge - OldAge;
	}





	virtual void BearFruit(cChunk & a_Chunk, const Vector3i a_StemRelPos) const override
	{
		auto & Random = GetRandomProvider();

		// Check if there's another produce around the stem, if so, abort:
		static constexpr std::array<Vector3i, 4> NeighborOfs =
		{
			{
				{ 1, 0,  0},
				{-1, 0,  0},
				{ 0, 0,  1},
				{ 0, 0, -1},
			}
		};

		std::array<BlockState, 4> Neighbors;
		if (
			!a_Chunk.UnboundedRelGetBlock(a_StemRelPos + NeighborOfs[0], Neighbors[0]) ||
			!a_Chunk.UnboundedRelGetBlock(a_StemRelPos + NeighborOfs[1], Neighbors[1]) ||
			!a_Chunk.UnboundedRelGetBlock(a_StemRelPos + NeighborOfs[2], Neighbors[2]) ||
			!a_Chunk.UnboundedRelGetBlock(a_StemRelPos + NeighborOfs[3], Neighbors[3]) ||
			(Neighbors[0].Type() == ProduceBlockType) ||
			(Neighbors[1].Type() == ProduceBlockType) ||
			(Neighbors[2].Type() == ProduceBlockType) ||
			(Neighbors[3].Type() == ProduceBlockType)
		)
		{
			// Neighbors not valid or already taken by the same produce:
			return;
		}

		// Pick a direction in which to place the produce:
		int x = 0, z = 0;
		const auto CheckType = Random.RandInt<size_t>(3);  // The index to the neighbors array which should be checked for emptiness
		switch (CheckType)
		{
			case 0: x =  1; break;
			case 1: x = -1; break;
			case 2: z =  1; break;
			case 3: z = -1; break;
		}

		// Check that the block in that direction is empty:
		switch (Neighbors[CheckType].Type())
		{
			case BlockType::Air:
			case BlockType::CaveAir:
			case BlockType::VoidAir:
			case BlockType::Snow:
			case BlockType::TallGrass:
			case BlockType::DeadBush:
			{
				break;
			}
			default: return;
		}

		// Check if there's soil under the neighbor. We already know the neighbors are valid. Place produce if ok
		BlockState SoilBlock;
		const auto ProduceRelPos = a_StemRelPos + Vector3i(x, 0, z);
		VERIFY(a_Chunk.UnboundedRelGetBlock(ProduceRelPos.addedY(-1), SoilBlock));

		switch (SoilBlock.Type())
		{
			case BlockType::Dirt:
			case BlockType::ShortGrass:
			case BlockType::Farmland:
			{
				FLOGD("Growing melon / pumpkin at {0} (<{1}, {2}> from stem), overwriting {3}, growing on top of {4}, meta {5}",
					a_Chunk.RelativeToAbsolute(ProduceRelPos),
					x, z,
					Neighbors[CheckType],
					SoilBlock
				);
				// Place a randomly-facing produce:
				a_Chunk.SetBlock(ProduceRelPos, ProduceBlockType);
				break;
			}
			default: break;
		}
	}

private:

	// https://minecraft.wiki/w/Pumpkin_Seeds#Breaking
	// https://minecraft.wiki/w/Melon_Seeds#Breaking
	/** The array describes how many seed may be dropped at which age. The inner arrays describe the probability to drop 0, 1, 2, 3 seeds.
	The outer describes the age of the stem. */
	static constexpr std::array<std::array<double, 4>, 8> m_AgeSeedDropProbability
	{
		{
			{
				81.3, 17.42, 1.24, 0.03
			},
			{
				65.1, 30.04, 4.62, 0.24
			},
			{
				51.2, 38.4, 9.6, 0.8
			},
			{
				39.44, 43.02, 15.64, 1.9
			},
			{
				29.13, 44.44, 22.22, 3.7
			},
			{
				21.6, 43.2, 28.8, 6.4
			},
			{
				15.17, 39.82, 34.84, 10.16
			},
			{
				10.16, 34.84, 39.82, 15.17
			}
		}
	};
} ;

using cBlockMelonStemHandler           = cBlockStemsHandler<BlockType::Melon,   Item::MelonSeeds>;
using cBlockPumpkinStemHandler         = cBlockStemsHandler<BlockType::Pumpkin, Item::PumpkinSeeds>;
