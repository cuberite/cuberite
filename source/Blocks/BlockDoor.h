
#pragma once

#include "BlockHandler.h"





class cBlockDoorHandler :
	public cBlockHandler
{
public:
	cBlockDoorHandler(BLOCKTYPE a_BlockID);
	virtual void OnPlaced(cWorld * a_World, int a_X, int a_Y, int a_Z, int a_Dir) override;
	virtual void OnDestroyed(cWorld * a_World, int a_X, int a_Y, int a_Z) override;
	virtual void OnDigging(cWorld * a_World, cPlayer * a_Player, int a_X, int a_Y, int a_Z) override;
	virtual void OnUse(cWorld * a_World, cPlayer * a_Player, int a_X, int a_Y, int a_Z) override;
	virtual const char * GetStepSound(void) override;
	
	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem((m_BlockID == E_BLOCK_WOODEN_DOOR) ? E_ITEM_WOODEN_DOOR : E_ITEM_IRON_DOOR, 1, 0));
	}

	virtual bool IsUseable() override
	{
		return true;
	}

	virtual void PlaceBlock(cWorld *a_World, cPlayer *a_Player, NIBBLETYPE a_BlockMeta, int a_X, int a_Y, int a_Z, char a_Dir) override;

	virtual bool CanBePlacedOnSide(void) override
	{
		return false;
	}
} ;




