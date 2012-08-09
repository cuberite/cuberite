
#pragma once
#include "BlockRedstone.h"
#include "BlockTorch.h"
#include "../cTorch.h"





class cBlockRedstoneTorchHandler : public cBlockRedstoneHandler
{
public:
	cBlockRedstoneTorchHandler(BLOCKTYPE a_BlockID)
		: cBlockRedstoneHandler(a_BlockID)
	{
	}


	virtual bool CanBePlacedAt(cWorld *a_World, int a_X, int a_Y, int a_Z, char a_Dir) override
	{
		return cBlockTorchHandler::TorchCanBePlacedAt(a_World, a_X, a_Y, a_Z, a_Dir);
	}


	virtual bool CanBeAt(cWorld *a_World, int a_X, int a_Y, int a_Z) override
	{
		char Dir = cTorch::MetaDataToDirection(a_World->GetBlockMeta( a_X, a_Y, a_Z));
		return CanBePlacedAt(a_World, a_X, a_Y, a_Z, Dir);
	}


	virtual int GetDropID(void) override
	{
		return E_ITEM_REDSTONE_TORCH_ON;
	}

	
	virtual bool CanBePlacedOnSide() override
	{
		return true;
	}
};