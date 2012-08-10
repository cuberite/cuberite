
#pragma once
#include "Block.h"





class cBlockCactusHandler :
	public cBlockHandler
{
public:
	cBlockCactusHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}


	virtual NIBBLETYPE GetDropMeta(NIBBLETYPE a_BlockMeta) override
	{
		return 0;
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

	
	virtual bool CanBePlacedOnSide() override
	{
		return false;
	}
};




