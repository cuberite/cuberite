
#pragma once

#include "BlockPlant.h"





class cBlockSugarcaneHandler final :
	public cBlockPlant<false>
{
	using Super = cBlockPlant<false>;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		return cItem(E_ITEM_SUGARCANE, 1, 0);
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		if (a_RelPos.y <= 0)
		{
			return false;
		}

		switch (a_Chunk.GetBlock(a_RelPos.addedY(-1)))
		{
			case E_BLOCK_DIRT:
			case E_BLOCK_GRASS:
			case E_BLOCK_FARMLAND:
			case E_BLOCK_SAND:
			{
				static const Vector3i Coords[] =
				{
					{-1, -1,  0},
					{ 1, -1,  0},
					{ 0, -1, -1},
					{ 0, -1,  1},
				} ;
				for (size_t i = 0; i < ARRAYCOUNT(Coords); i++)
				{
					BLOCKTYPE BlockType;
					NIBBLETYPE BlockMeta;
					if (!a_Chunk.UnboundedRelGetBlock(a_RelPos + Coords[i], BlockType, BlockMeta))
					{
						// Too close to the edge, cannot simulate
						return true;
					}
					if (IsBlockWater(BlockType) || (BlockType == E_BLOCK_FROSTED_ICE))
					{
						return true;
					}
				}  // for i - Coords[]
				// Not directly neighboring a water block
				return false;
			}
			case E_BLOCK_SUGARCANE:
			{
				return true;
			}
		}
		return false;
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 7;
	}





	virtual int Grow(cChunk & a_Chunk, Vector3i a_RelPos, int a_NumStages = 1) const override
	{
		// Check the total height of the sugarcane blocks here:
		int top = a_RelPos.y + 1;
		while (
			(top < cChunkDef::Height) &&
			(a_Chunk.GetBlock({a_RelPos.x, top, a_RelPos.z}) == E_BLOCK_SUGARCANE)
		)
		{
			++top;
		}
		int bottom = a_RelPos.y - 1;
		while (
			(bottom > 0) &&
			(a_Chunk.GetBlock({a_RelPos.x, bottom, a_RelPos.z}) == E_BLOCK_SUGARCANE)
		)
		{
			--bottom;
		}

		// Grow by at most a_NumStages, but no more than max height:
		auto toGrow = std::min(a_NumStages, a_Chunk.GetWorld()->GetMaxSugarcaneHeight() + 1 - (top - bottom));
		Vector3i topPos(a_RelPos.x, top, a_RelPos.z);
		for (int i = 0; i < toGrow; i++)
		{
			if (a_Chunk.GetBlock(topPos.addedY(i)) == E_BLOCK_AIR)
			{
				a_Chunk.SetBlock(topPos.addedY(i), E_BLOCK_SUGARCANE, 0);
			}
			else
			{
				return i;
			}
		}  // for i
		return toGrow;
	}

	virtual PlantAction CanGrow(cChunk & a_Chunk, Vector3i a_RelPos) const override
	{
		// Only allow growing if there's an air block above:
		if (((a_RelPos.y + 1) < cChunkDef::Height) && (a_Chunk.GetBlock(a_RelPos.addedY(1)) == E_BLOCK_AIR))
		{
			return Super::CanGrow(a_Chunk, a_RelPos);
		}
		return paStay;
	}
} ;




