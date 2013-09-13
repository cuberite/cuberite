
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


	virtual bool CanBeAt(int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
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
		for (int i = 0; i < ARRAYCOUNT(Coords); i++)
		{
			BLOCKTYPE BlockType;
			NIBBLETYPE BlockMeta;
			if (
				a_Chunk.UnboundedRelGetBlock(a_RelX + Coords[i].x, a_RelY, a_RelZ + Coords[i].z, BlockType, BlockMeta) &&
				(BlockType != E_BLOCK_AIR)
			)
			{
				return false;
			}
		}  // for i - Coords[]

		return true;
	}
	

	void OnUpdate(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		a_World->GrowCactus(a_BlockX, a_BlockY, a_BlockZ, 1);
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.cloth";
	}
} ;




