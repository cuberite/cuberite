
#pragma once

#include "BlockHandler.h"





class cBlockMushroomHandler :
	public cBlockHandler
{
public:
	cBlockMushroomHandler(BLOCKTYPE a_BlockType)
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
		
		// TODO: Cannot be at too much daylight
		
		switch (a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ))
		{
			case E_BLOCK_GLASS:
			case E_BLOCK_CACTUS:
			case E_BLOCK_ICE:
			case E_BLOCK_LEAVES:
			case E_BLOCK_AIR:
			{
				return false;
			}
		}
		return true;
	}
	

	virtual const char * GetStepSound(void) override
	{
		return "step.grass";
	}
} ;




