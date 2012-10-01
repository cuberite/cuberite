
#pragma once

#include "BlockHandler.h"
#include "../World.h"





class cBlockRedstoneRepeaterHandler :
	public cBlockHandler
{
public:
	cBlockRedstoneRepeaterHandler(BLOCKTYPE a_BlockID);
	virtual void OnPlaced(cWorld * a_World, int a_X, int a_Y, int a_Z, int a_Dir) override;
	virtual void OnDestroyed(cWorld * a_World, int a_X, int a_Y, int a_Z) override;
	
	virtual void OnDigging(cWorld * a_World, cPlayer * a_Player, int a_X, int a_Y, int a_Z) override;
	virtual void OnUse(cWorld * a_World, cPlayer * a_Player, int a_X, int a_Y, int a_Z) override;


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to 0
		a_Pickups.push_back(cItem(E_ITEM_REDSTONE_REPEATER, 1, 0));
	}


	virtual bool IsUseable(void) override
	{
		return true;
	}
	
	
	virtual void PlaceBlock(cWorld * a_World, cPlayer * a_Player, NIBBLETYPE a_BlockMeta, int a_X, int a_Y, int a_Z, char a_Dir) override;


	virtual bool DoesAllowBlockOnTop(void) override
	{
		return false;
	}
	
	
	virtual bool CanBeAt(cWorld * a_World, int a_X, int a_Y, int a_Z) override
	{
		return a_World->GetBlock(a_X, a_Y - 1, a_Z) != E_BLOCK_AIR;
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




