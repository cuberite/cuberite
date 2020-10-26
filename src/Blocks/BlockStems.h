
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
		return cItem(StemPickupType, 1, 0);
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
		auto oldMeta = a_Chunk.GetMeta(a_RelPos);
		auto meta = oldMeta + a_NumStages;
		a_Chunk.SetBlock(a_RelPos, m_BlockType, static_cast<NIBBLETYPE>(std::min(meta, 7)));  // Update the stem
		if (meta > 7)
		{
			if (growProduce(a_Chunk, a_RelPos))
			{
				return 8 - oldMeta;
			}
			else
			{
				return 7 - oldMeta;
			}
		}
		return meta - oldMeta;
	}

	/** Grows the final produce next to the stem at the specified pos.
	Returns true if successful, false if not. */
	static bool growProduce(cChunk & a_Chunk, Vector3i a_StemRelPos)
	{
		auto & random = GetRandomProvider();

		// Check if there's another produce around the stem, if so, abort:
		static const Vector3i neighborOfs[] =
		{
			{ 1, 0,  0},
			{-1, 0,  0},
			{ 0, 0,  1},
			{ 0, 0, -1},
		};
		bool isValid;
		BLOCKTYPE blockType[4];
		NIBBLETYPE blockMeta;  // unused
		isValid =            a_Chunk.UnboundedRelGetBlock(a_StemRelPos + neighborOfs[0], blockType[0], blockMeta);
		isValid = isValid && a_Chunk.UnboundedRelGetBlock(a_StemRelPos + neighborOfs[1], blockType[1], blockMeta);
		isValid = isValid && a_Chunk.UnboundedRelGetBlock(a_StemRelPos + neighborOfs[2], blockType[2], blockMeta);
		isValid = isValid && a_Chunk.UnboundedRelGetBlock(a_StemRelPos + neighborOfs[3], blockType[3], blockMeta);
		if (
			!isValid ||
			(blockType[0] == ProduceBlockType) ||
			(blockType[1] == ProduceBlockType) ||
			(blockType[2] == ProduceBlockType) ||
			(blockType[3] == ProduceBlockType)
		)
		{
			// Neighbors not valid or already taken by the same produce
			return false;
		}

		// Pick a direction in which to place the produce:
		int x = 0, z = 0;
		int checkType = random.RandInt(3);  // The index to the neighbors array which should be checked for emptiness
		switch (checkType)
		{
			case 0: x =  1; break;
			case 1: x = -1; break;
			case 2: z =  1; break;
			case 3: z = -1; break;
		}

		// Check that the block in that direction is empty:
		switch (blockType[checkType])
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
		BLOCKTYPE soilType;
		auto produceRelPos = a_StemRelPos + Vector3i(x, 0, z);
		VERIFY(a_Chunk.UnboundedRelGetBlock(produceRelPos.addedY(-1), soilType, blockMeta));
		switch (soilType)
		{
			case E_BLOCK_DIRT:
			case E_BLOCK_GRASS:
			case E_BLOCK_FARMLAND:
			{
				// Place a randomly-facing produce:
				NIBBLETYPE meta = (ProduceBlockType == E_BLOCK_MELON) ? 0 : static_cast<NIBBLETYPE>(random.RandInt(4) % 4);
				auto produceAbsPos = a_Chunk.RelativeToAbsolute(produceRelPos);
				FLOGD("Growing melon / pumpkin at {0} (<{1}, {2}> from stem), overwriting {3}, growing on top of {4}, meta {5}",
					produceAbsPos,
					x, z,
					ItemTypeToString(blockType[checkType]),
					ItemTypeToString(soilType),
					meta
				);
				a_Chunk.GetWorld()->SetBlock(produceAbsPos, ProduceBlockType, meta);
				return true;
			}
		}
		return false;
	}
} ;

using cBlockMelonStemHandler   = cBlockStemsHandler<E_BLOCK_MELON,   E_ITEM_MELON_SEEDS>;
using cBlockPumpkinStemHandler = cBlockStemsHandler<E_BLOCK_PUMPKIN, E_ITEM_PUMPKIN_SEEDS>;



