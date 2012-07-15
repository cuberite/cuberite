#pragma once
#include "Block.h"
#include "../cWorld.h"

class cBlockRedstoneRepeaterHandler : public cBlockHandler
{
public:
	cBlockRedstoneRepeaterHandler(BLOCKTYPE a_BlockID);
	virtual void OnPlaced(cWorld *a_World, int a_X, int a_Y, int a_Z, int a_Dir);
	virtual void OnDestroyed(cWorld *a_World, int a_X, int a_Y, int a_Z);
	
	virtual void OnClick(cWorld *a_World, cPlayer *a_Player, int a_X, int a_Y, int a_Z);

	virtual char GetDropMeta(char a_BlockMeta)
	{
		return 0;
	}

	virtual int GetDropID()
	{
		return E_ITEM_REDSTONE_REPEATER;
	}

	virtual bool IsUseable()
	{
		return true;
	}
	
	virtual void PlaceBlock(cWorld *a_World, cPlayer *a_Player, char a_BlockMeta, int a_X, int a_Y, int a_Z, char a_Dir);

	virtual bool AllowBlockOnTop()
	{
		return false;
	}
	
	virtual bool CanBeAt(cWorld *a_World, int a_X, int a_Y, int a_Z)
	{
		return a_World->GetBlock(a_X, a_Y - 1, a_Z) != E_BLOCK_AIR;
	}

	
	virtual bool CanBePlacedOnSide()
	{
		return false;
	}
};