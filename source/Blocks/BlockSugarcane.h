
#pragma once

#include "BlockHandler.h"





class cBlockSugarcaneHandler :
	public cBlockHandler
{
public:
	cBlockSugarcaneHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_ITEM_SUGARCANE, 1, 0));
	}


	virtual bool CanBeAt(int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
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
				for (int i = 0; i < ARRAYCOUNT(Coords); i++)
				{
					BLOCKTYPE BlockType;
					NIBBLETYPE BlockMeta;
					if (!a_Chunk.UnboundedRelGetBlock(a_RelX + Coords[i].x, a_RelY, a_RelZ + Coords[i].z, BlockType, BlockMeta))
					{
						// Too close to the edge, cannot simulate
						return true;
					}
					if (IsBlockWater(BlockType))
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
	
	
	void OnUpdate(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		a_World->GrowSugarcane(a_BlockX, a_BlockY, a_BlockZ, 1);
	}


	virtual bool CanBePlacedOnSide() override
	{
		return false;
	}
	
	
	virtual const char * GetStepSound(void) override
	{
		return "step.grass";
	}
} ;




