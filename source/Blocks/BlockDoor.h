
#pragma once

#include "BlockHandler.h"
#include "../World.h"





class cBlockDoorHandler :
	public cBlockHandler
{
public:
	cBlockDoorHandler(BLOCKTYPE a_BlockType);
	virtual void OnPlaced(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, int a_Dir) override;
	virtual void OnDestroyed(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override;
	virtual void OnUse(cWorld * a_World, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ) override;
	virtual const char * GetStepSound(void) override;
	
	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem((m_BlockType == E_BLOCK_WOODEN_DOOR) ? E_ITEM_WOODEN_DOOR : E_ITEM_IRON_DOOR, 1, 0));
	}

	virtual bool IsUseable() override
	{
		return true;
	}

	virtual void PlaceBlock(cWorld *a_World, cPlayer *a_Player, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override;

	virtual bool CanBePlacedOnSide(void) override
	{
		return false;
	}

	virtual bool CanBeAt(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		return (a_World->GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ) != E_BLOCK_AIR);
	}
} ;




