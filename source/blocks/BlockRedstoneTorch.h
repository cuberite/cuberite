#pragma once
#include "BlockRedstone.h"
#include "../cTorch.h"


class cBlockRedstoneTorchHandler : public cBlockRedstoneHandler
{
public:
	cBlockRedstoneTorchHandler(BLOCKTYPE a_BlockID)
		: cBlockRedstoneHandler(a_BlockID)
	{
	}

	virtual bool CanBeAt(cWorld *a_World, int a_X, int a_Y, int a_Z)
	{
		char Dir = cTorch::MetaDataToDirection(a_World->GetBlockMeta( a_X, a_Y, a_Z));
		AddDirection( a_X, a_Y, a_Z, Dir, true );
		return a_World->GetBlock( a_X, a_Y, a_Z ) != E_BLOCK_AIR;
	}

	virtual int GetDropID()
	{
		return E_ITEM_REDSTONE_TORCH_ON;
	}
	
	virtual bool CanBePlacedOnSide()
	{
		return true;
	}
};