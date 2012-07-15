#pragma once
#include "Block.h"
#include "../cTorch.h"
#include "../cWorld.h"


class cBlockTorchHandler : public cBlockHandler
{
public:
	cBlockTorchHandler(BLOCKTYPE a_BlockID);
	
	virtual void PlaceBlock(cWorld *a_World, cPlayer *a_Player, char a_BlockMeta, int a_X, int a_Y, int a_Z, char a_Dir);
	
	virtual bool AllowBlockOnTop()
	{
		return false;
	}

	virtual bool CanBeAt(cWorld *a_World, int a_X, int a_Y, int a_Z)
	{
		char Dir = cTorch::MetaDataToDirection(a_World->GetBlockMeta( a_X, a_Y, a_Z));
		AddDirection( a_X, a_Y, a_Z, Dir, true );
		return a_World->GetBlock( a_X, a_Y, a_Z ) != E_BLOCK_AIR;
	}
};