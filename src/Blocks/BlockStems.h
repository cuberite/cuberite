
#pragma once

#include "BlockPlant.h"





/** Handler for stems from which produce grows in an adjacent block (melon, pumpkin) after it becomes ripe (meta == 7).
ProduceBlockType is the blocktype for the produce to be grown.
StemPickupType is the item type for the pickup resulting from breaking the stem. */
template <BLOCKTYPE ProduceBlockType, ENUM_ITEM_TYPE StemPickupType>
class cBlockStemsHandler final :
	public cBlockPlant<true>
{
	using Super = cBlockPlant<true>;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		/*
			Use correct percent:
			https://minecraft.gamepedia.com/Melon_Seeds#Breaking
			https://minecraft.gamepedia.com/Pumpkin_Seeds#Breaking
		*/

		// Age > 7 (Impossible)
		if (a_BlockMeta > 7)
		{
			return cItem(StemPickupType);
		}

		const auto Threshold = GetRandomProvider().RandReal<double>(100);
		double Cumulative = 0;
		char Count = 0;

		for (; Count < 4; Count++)
		{
			Cumulative += m_AgeSeedDropProbability[static_cast<size_t>(a_BlockMeta)][static_cast<size_t>(Count)];
			if (Cumulative > Threshold)
			{
				break;
			}
		}

		return cItem(StemPickupType, Count);
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





	virtual int Grow(cChunk & a_Chunk, Vector3i a_RelPos, int a_NumStages = 1) const override
	{
		const auto OldMeta = a_Chunk.GetMeta(a_RelPos);
		const auto NewMeta = std::clamp<NIBBLETYPE>(static_cast<NIBBLETYPE>(OldMeta + a_NumStages), 0, 7);
		a_Chunk.SetMeta(a_RelPos, NewMeta);
		return NewMeta - OldMeta;
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

		std::array<BLOCKTYPE, 4> BlockType;
		if (
			!a_Chunk.UnboundedRelGetBlockType(a_StemRelPos + NeighborOfs[0], BlockType[0]) ||
			!a_Chunk.UnboundedRelGetBlockType(a_StemRelPos + NeighborOfs[1], BlockType[1]) ||
			!a_Chunk.UnboundedRelGetBlockType(a_StemRelPos + NeighborOfs[2], BlockType[2]) ||
			!a_Chunk.UnboundedRelGetBlockType(a_StemRelPos + NeighborOfs[3], BlockType[3]) ||
			(BlockType[0] == ProduceBlockType) ||
			(BlockType[1] == ProduceBlockType) ||
			(BlockType[2] == ProduceBlockType) ||
			(BlockType[3] == ProduceBlockType)
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
		switch (BlockType[CheckType])
		{
			case E_BLOCK_AIR:
			case E_BLOCK_SNOW:
			case E_BLOCK_TALL_GRASS:
			case E_BLOCK_DEAD_BUSH:
			{
				break;
			}
			default: return;
		}

		// Check if there's soil under the neighbor. We already know the neighbors are valid. Place produce if ok
		BLOCKTYPE SoilType;
		const auto ProduceRelPos = a_StemRelPos + Vector3i(x, 0, z);
		VERIFY(a_Chunk.UnboundedRelGetBlockType(ProduceRelPos.addedY(-1), SoilType));

		switch (SoilType)
		{
			case E_BLOCK_DIRT:
			case E_BLOCK_GRASS:
			case E_BLOCK_FARMLAND:
			{
				const NIBBLETYPE Meta = (ProduceBlockType == E_BLOCK_MELON) ? 0 : static_cast<NIBBLETYPE>(Random.RandInt(4) % 4);

				FLOGD("Growing melon / pumpkin at {0} (<{1}, {2}> from stem), overwriting {3}, growing on top of {4}, meta {5}",
					a_Chunk.RelativeToAbsolute(ProduceRelPos),
					x, z,
					ItemTypeToString(BlockType[CheckType]),
					ItemTypeToString(SoilType),
					Meta
				);

				// Place a randomly-facing produce:
				a_Chunk.SetBlock(ProduceRelPos, ProduceBlockType, Meta);
			}
		}
	}

private:

	// https://minecraft.gamepedia.com/Pumpkin_Seeds#Breaking
	// https://minecraft.gamepedia.com/Melon_Seeds#Breaking
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

using cBlockMelonStemHandler   = cBlockStemsHandler<E_BLOCK_MELON,   E_ITEM_MELON_SEEDS>;
using cBlockPumpkinStemHandler = cBlockStemsHandler<E_BLOCK_PUMPKIN, E_ITEM_PUMPKIN_SEEDS>;
