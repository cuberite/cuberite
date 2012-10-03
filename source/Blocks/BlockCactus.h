
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


	virtual bool CanBeAt(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		BLOCKTYPE Surface = a_World->GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ);
		if ((Surface != E_BLOCK_SAND) && (Surface != E_BLOCK_CACTUS))
		{
			// Cactus can only be placed on sand and itself
			return false;
		}

		// Check surroundings. Cacti may ONLY be surrounded by air
		if (
			(a_World->GetBlock(a_BlockX - 1, a_BlockY, a_BlockZ)     != E_BLOCK_AIR) ||
			(a_World->GetBlock(a_BlockX + 1, a_BlockY, a_BlockZ)     != E_BLOCK_AIR) ||
			(a_World->GetBlock(a_BlockX,     a_BlockY, a_BlockZ - 1) != E_BLOCK_AIR) ||
			(a_World->GetBlock(a_BlockX,     a_BlockY, a_BlockZ + 1) != E_BLOCK_AIR)
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
	

	void OnUpdate(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		a_World->GrowCactus(a_BlockX, a_BlockY, a_BlockZ, 1);
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.cloth";
	}
} ;




