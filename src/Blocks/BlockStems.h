
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

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		/**
			Use correct percent:
			https://minecraft.gamepedia.com/Melon_Seeds#Breaking
			https://minecraft.gamepedia.com/Pumpkin_Seeds#Breaking
			Good new: Melon & Pumpkin have same random values !
		*/

		// Age > 7 (Impossible)
		if ((a_BlockMeta > 7) || (a_BlockMeta < 0))
		{
			return cItem(StemPickupType, 1, 0);
		}

		auto & Rand = GetRandomProvider();
		float RandomValue = Rand.RandReal<float>(100);
		float Max = 0;
		int Count = 0;
		for (; Count < 4; Count++)
		{
			Max += m_AgeSeedDropProbability[a_BlockMeta][Count];
			if (Max > RandomValue)
			{
				break;
			}
		}
		return cItem(StemPickupType, Count, 0);
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





	virtual int Grow(cChunk & a_Chunk, Vector3i a_RelPos, int a_NumStages = 1) const override
	{
		auto OldMeta = a_Chunk.GetMeta(a_RelPos);
		if (OldMeta >= 7)
		{
			// Already ripe
			return 0;
		}
		auto NewMeta = std::min<int>(OldMeta + a_NumStages, 7);
		ASSERT(NewMeta > OldMeta);
		a_Chunk.SetBlock(a_RelPos, m_BlockType, static_cast<NIBBLETYPE>(std::min(NewMeta, 7)));  // Update the stem
		return NewMeta - OldMeta;
	}





	virtual bool IsFullGrown(cChunk & a_Chunk, Vector3i a_RelPos) const override
	{
		auto oldMeta = a_Chunk.GetMeta(a_RelPos);
		return (oldMeta >= 7);
	}





	virtual bool BearFruit(cChunk & a_Chunk, const Vector3i a_StemRelPos) const override
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
		bool IsValid;
		std::array<BLOCKTYPE, 4> BlockType;
		NIBBLETYPE BlockMeta;  // unused
		IsValid =            a_Chunk.UnboundedRelGetBlock(a_StemRelPos + NeighborOfs[0], BlockType[0], BlockMeta);
		IsValid = IsValid && a_Chunk.UnboundedRelGetBlock(a_StemRelPos + NeighborOfs[1], BlockType[1], BlockMeta);
		IsValid = IsValid && a_Chunk.UnboundedRelGetBlock(a_StemRelPos + NeighborOfs[2], BlockType[2], BlockMeta);
		IsValid = IsValid && a_Chunk.UnboundedRelGetBlock(a_StemRelPos + NeighborOfs[3], BlockType[3], BlockMeta);
		if (
			!IsValid ||
			(BlockType[0] == ProduceBlockType) ||
			(BlockType[1] == ProduceBlockType) ||
			(BlockType[2] == ProduceBlockType) ||
			(BlockType[3] == ProduceBlockType)
		)
		{
			// Neighbors not valid or already taken by the same produce
			return false;
		}

		// Pick a direction in which to place the produce:
		int x = 0, z = 0;
		int CheckType = Random.RandInt(3);  // The index to the neighbors array which should be checked for emptiness
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
			default: return false;
		}

		// Check if there's soil under the neighbor. We already know the neighbors are valid. Place produce if ok
		BLOCKTYPE SoilType;
		auto produceRelPos = a_StemRelPos + Vector3i(x, 0, z);
		VERIFY(a_Chunk.UnboundedRelGetBlock(produceRelPos.addedY(-1), SoilType, BlockMeta));
		switch (SoilType)
		{
			case E_BLOCK_DIRT:
			case E_BLOCK_GRASS:
			case E_BLOCK_FARMLAND:
			{
				// Place a randomly-facing produce:
				NIBBLETYPE meta = (ProduceBlockType == E_BLOCK_MELON) ? 0 : static_cast<NIBBLETYPE>(Random.RandInt(4) % 4);
				auto produceAbsPos = a_Chunk.RelativeToAbsolute(produceRelPos);
				FLOGD("Growing melon / pumpkin at {0} (<{1}, {2}> from stem), overwriting {3}, growing on top of {4}, meta {5}",
					produceAbsPos,
					x, z,
					ItemTypeToString(BlockType[CheckType]),
					ItemTypeToString(SoilType),
					meta
				);
				a_Chunk.GetWorld()->SetBlock(produceAbsPos, ProduceBlockType, meta);
				return true;
			}
		}
		return false;
	}




private:
	// https://minecraft.gamepedia.com/Pumpkin_Seeds#Breaking
	// https://minecraft.gamepedia.com/Melon_Seeds#Breaking
	/** The array describes how many seed may be dropped at which age. The outer arrays describe the probability to drop 0, 1, 2, 3 seeds.
	The outer describes the age of the stem. */
	static constexpr std::array<std::array<float, 4>, 8> m_AgeSeedDropProbability =
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



