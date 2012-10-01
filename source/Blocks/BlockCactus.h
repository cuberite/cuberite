
#pragma once

#include "BlockHandler.h"





class cBlockCactusHandler :
	public cBlockHandler
{
public:
	cBlockCactusHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to 0
		a_Pickups.push_back(cItem(m_BlockID, 1, 0));
	}


	virtual bool CanBeAt(cWorld * a_World, int a_X, int a_Y, int a_Z) override
	{
		BLOCKTYPE Surface = a_World->GetBlock(a_X, a_Y - 1, a_Z);
		if ((Surface != E_BLOCK_SAND) && (Surface != E_BLOCK_CACTUS))
		{
			// Cactus can only be placed on sand and itself
			return false;
		}

		// Check surroundings. Cacti may ONLY be surrounded by air
		if (
			(a_World->GetBlock(a_X - 1, a_Y, a_Z)     != E_BLOCK_AIR) ||
			(a_World->GetBlock(a_X + 1, a_Y, a_Z)     != E_BLOCK_AIR) ||
			(a_World->GetBlock(a_X,     a_Y, a_Z - 1) != E_BLOCK_AIR) ||
			(a_World->GetBlock(a_X,     a_Y, a_Z + 1) != E_BLOCK_AIR)
		)
		{
			return false;
		}

		return true;
	}

	
	virtual bool CanBePlacedOnSide(void) override
	{
		return false;
	}
	

	virtual const char * GetStepSound(void) override
	{
		return "step.cloth";
	}
} ;




