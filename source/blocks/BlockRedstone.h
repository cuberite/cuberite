#pragma once
#include "Block.h"
#include "../cWorld.h"

class cBlockRedstoneHandler : public cBlockHandler
{
public:
	cBlockRedstoneHandler(BLOCKTYPE a_BlockID);
	virtual void OnPlaced(cWorld *a_World, int a_X, int a_Y, int a_Z, int a_Dir) override;
	virtual void OnDestroyed(cWorld *a_World, int a_X, int a_Y, int a_Z) override;
	
	virtual void PlaceBlock(cWorld *a_World, cPlayer *a_Player, NIBBLETYPE a_BlockMeta, int a_X, int a_Y, int a_Z, char a_Dir) override;
	
	virtual bool AllowBlockOnTop() override
	{
		return false;
	}

	virtual bool CanBeAt(cWorld *a_World, int a_X, int a_Y, int a_Z) override
	{
		return a_World->GetBlock(a_X, a_Y - 1, a_Z) != E_BLOCK_AIR;
	}
	
	virtual int GetDropID() override
	{
		return E_ITEM_REDSTONE_DUST;
	}
	
	virtual bool CanBePlacedOnSide() override
	{
		return false;
	}
};