
#pragma once

#include "BlockHandler.h"
#include "../World.h"





class cBlockSaplingHandler :
	public cBlockHandler
{
public:
	cBlockSaplingHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Only the first 2 bits contain the display information, the others are for growing
		a_Pickups.push_back(cItem(E_BLOCK_SAPLING, 1, a_BlockMeta & 3));
	}


	virtual bool CanBeAt(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		return IsBlockTypeOfDirt(a_World->GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ));
	}


	virtual bool DoesAllowBlockOnTop(void) override
	{
		return false;
	}
	

	void OnUpdate(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		NIBBLETYPE Meta = a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		
		if ((Meta & 0x08) != 0)
		{
			a_World->GrowTree(a_BlockX, a_BlockY, a_BlockZ);
		}
		else
		{
			a_World->SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, Meta | 0x08);
		}
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




