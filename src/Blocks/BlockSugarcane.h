
#pragma once

#include "BlockPlant.h"





class cBlockSugarcaneHandler :
	public cBlockPlant
{
	typedef cBlockPlant Super;
public:
	cBlockSugarcaneHandler(BLOCKTYPE a_BlockType)
		: Super(a_BlockType, false)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_ITEM_SUGARCANE, 1, 0));
	}

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		if (a_RelY <= 0)
		{
			return false;
		}

		switch (a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ))
		{
			case E_BLOCK_DIRT:
			case E_BLOCK_GRASS:
			case E_BLOCK_FARMLAND:
			case E_BLOCK_SAND:
			{
				static const struct
				{
					int x, z;
				} Coords[] =
				{
					{-1,  0},
					{ 1,  0},
					{ 0, -1},
					{ 0,  1},
				} ;
				a_RelY -= 1;
				for (size_t i = 0; i < ARRAYCOUNT(Coords); i++)
				{
					BLOCKTYPE BlockType;
					NIBBLETYPE BlockMeta;
					if (!a_Chunk.UnboundedRelGetBlock(a_RelX + Coords[i].x, a_RelY, a_RelZ + Coords[i].z, BlockType, BlockMeta))
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

	virtual void OnUpdate(cChunkInterface & cChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_PluginInterface, cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ) override
	{
		if (CanGrow(a_Chunk, a_RelX, a_RelY, a_RelZ) == paGrowth)
		{
			a_Chunk.GetWorld()->GrowSugarcane(a_RelX + a_Chunk.GetPosX() * cChunkDef::Width, a_RelY, a_RelZ + a_Chunk.GetPosZ() * cChunkDef::Width, 1);
		}
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 7;
	}

protected:

	virtual PlantAction CanGrow(cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ) override
	{
		auto Action = paStay;
		if (((a_RelY + 1) < cChunkDef::Height) && (a_Chunk.GetBlock(a_RelX, a_RelY + 1, a_RelZ) == E_BLOCK_AIR))
		{
			Action = Super::CanGrow(a_Chunk, a_RelX, a_RelY, a_RelZ);
		}

		return Action;
	}
} ;




