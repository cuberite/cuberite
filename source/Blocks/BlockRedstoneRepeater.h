
#pragma once

#include "BlockHandler.h"
#include "../World.h"





class cBlockRedstoneRepeaterHandler :
	public cBlockHandler
{
public:
	cBlockRedstoneRepeaterHandler(BLOCKTYPE a_BlockType);
	virtual void OnPlaced(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, int a_Dir) override;
	virtual void OnDestroyed(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override;
	
	virtual void OnDigging(cWorld * a_World, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ) override;
	virtual void OnUse(cWorld * a_World, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ) override;


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to 0
		a_Pickups.push_back(cItem(E_ITEM_REDSTONE_REPEATER, 1, 0));
	}


	virtual bool IsUseable(void) override
	{
		return true;
	}
	
	
	virtual void PlaceBlock(cWorld * a_World, cPlayer * a_Player, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override;


	virtual bool DoesAllowBlockOnTop(void) override
	{
		return false;
	}
	
	
	virtual bool CanBeAt(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		return a_World->GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ) != E_BLOCK_AIR;
	}

	
	virtual bool CanBePlacedOnSide(void) override
	{
		return false;
	}

	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
} ;




