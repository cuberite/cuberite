
#pragma once

#include "BlockHandler.h"





class cBlockCactusHandler :
	public cBlockHandler
{
public:
	cBlockCactusHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
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

		// Check surroundings. Cacti may ONLY be surrounded by air
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
				(g_BlockIsSolid[BlockType])
			)
			{
				return false;
			}
		}  // for i - Coords[]

		return true;
	}
	

	void OnUpdate(cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ) override
	{
		a_Chunk.GetWorld()->GrowCactus(a_RelX + a_Chunk.GetPosX() * cChunkDef::Width, a_RelY, a_RelZ + a_Chunk.GetPosZ() * cChunkDef::Width, 1);
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.cloth";
	}
} ;




