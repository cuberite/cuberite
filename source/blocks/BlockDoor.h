#pragma once
#include "Block.h"


class cBlockDoorHandler : public cBlockHandler
{
public:
	cBlockDoorHandler(BLOCKTYPE a_BlockID);
	virtual void OnPlaced(cWorld *a_World, int a_X, int a_Y, int a_Z, int a_Dir) override;
	virtual void OnDestroyed(cWorld *a_World, int a_X, int a_Y, int a_Z) override;
	virtual void OnClick(cWorld *a_World, cPlayer *a_Player, int a_X, int a_Y, int a_Z) override;
	virtual char GetDropCount() override;
	virtual bool IsUseable() override
	{
		return true;
	}

	virtual void PlaceBlock(cWorld *a_World, cPlayer *a_Player, NIBBLETYPE a_BlockMeta, int a_X, int a_Y, int a_Z, char a_Dir) override;

	virtual int GetDropID() override
	{
		return (m_BlockID == E_BLOCK_WOODEN_DOOR) ? E_ITEM_WOODEN_DOOR : E_ITEM_IRON_DOOR;
	}

	virtual bool CanBePlacedOnSide() override
	{
		return false;
	}
};