
#pragma once

#include "BlockPlant.h"





class cBlockCactusHandler :
	public cBlockPlant
{
	typedef cBlockPlant Super;
public:
	cBlockCactusHandler(BLOCKTYPE a_BlockType)
		: Super(a_BlockType, false)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to 0
		a_Pickups.push_back(cItem(m_BlockType, 1, 0));
	}

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		if (a_RelY <= 0)
		{
			return false;
		}
		BLOCKTYPE Surface = a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ);
		if ((Surface != E_BLOCK_SAND) && (Surface != E_BLOCK_CACTUS))
		{
			// Cactus can only be placed on sand and itself
			return false;
		}

		// Check surroundings. Cacti may ONLY be surrounded by non-solid blocks
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
		for (size_t i = 0; i < ARRAYCOUNT(Coords); i++)
		{
			BLOCKTYPE BlockType;
			NIBBLETYPE BlockMeta;
			if (
				a_Chunk.UnboundedRelGetBlock(a_RelX + Coords[i].x, a_RelY, a_RelZ + Coords[i].z, BlockType, BlockMeta) &&
				(
					cBlockInfo::IsSolid(BlockType) ||
					(BlockType == E_BLOCK_LAVA) ||
					(BlockType == E_BLOCK_STATIONARY_LAVA)
				)
			)
			{
				return false;
			}
		}  // for i - Coords[]

		return true;
	}

	virtual void OnUpdate(cChunkInterface & cChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_PluginInterface, cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ) override
	{
		if (CanGrow(a_Chunk, a_RelX, a_RelY, a_RelZ) == paGrowth)
		{
			a_Chunk.GetWorld()->GrowCactus(a_RelX + a_Chunk.GetPosX() * cChunkDef::Width, a_RelY, a_RelZ + a_Chunk.GetPosZ() * cChunkDef::Width, 1);
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




