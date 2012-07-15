#pragma once
#include "Block.h"
#include "../MersenneTwister.h"
#include "../cWorld.h"

class cBlockSugarcaneHandler : public cBlockHandler
{
public:
	cBlockSugarcaneHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}

	virtual bool NeedsRandomTicks()
	{
		return true;
	}

	virtual int GetDropID()
	{
		return E_ITEM_SUGARCANE;
	}

	virtual bool CanBeAt(cWorld *a_World, int a_X, int a_Y, int a_Z)
	{
		 BLOCKTYPE Block = a_World->GetBlock(a_X, a_Y-1, a_Z);
		 if(!IsBlockTypeOfDirt(Block) && Block != E_BLOCK_SAND && Block != E_BLOCK_SUGARCANE)
			 return false;
		 
		return a_World->IsBlockDirectlyWatered(a_X, a_Y - 1, a_Z);
	}
	
	void OnUpdate(cWorld *a_World, int a_X, int a_Y, int a_Z)
	{

		//TODO: Handle Growing here
	}

	virtual bool CanBePlacedOnSide()
	{
		return false;
	}
};