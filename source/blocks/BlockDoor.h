#pragma once
#include "Block.h"


class cBlockDoorHandler : public cBlockHandler
{
public:
	cBlockDoorHandler(BLOCKTYPE a_BlockID);
	virtual void OnPlaced(cWorld *a_World, int a_X, int a_Y, int a_Z, int a_Dir);
	virtual void OnDestroyed(cWorld *a_World, int a_X, int a_Y, int a_Z);
	virtual void OnClick(cWorld *a_World, cPlayer *a_Player, int a_X, int a_Y, int a_Z);
	virtual char GetDropCount();
	virtual bool IsUseable()
	{
		return true;
	}

	virtual void PlaceBlock(cWorld *a_World, cPlayer *a_Player, char a_BlockMeta, int a_X, int a_Y, int a_Z, char a_Dir);

	virtual int GetDropID()
	{
		return (m_BlockID == E_BLOCK_WOODEN_DOOR) ? E_ITEM_WOODEN_DOOR : E_ITEM_IRON_DOOR;
	}

	virtual bool CanBePlacedOnSide()
	{
		return false;
	}
};