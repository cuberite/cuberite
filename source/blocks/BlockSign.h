#pragma once
#include "BlockHandler.h"
#include "../World.h"
#include "../Sign.h"
#include "../Player.h"

class cBlockSignHandler : public cBlockHandler
{
public:
	cBlockSignHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{

	}

	virtual void PlaceBlock(cWorld *a_World, cPlayer *a_Player, NIBBLETYPE a_BlockMeta, int a_X, int a_Y, int a_Z, char a_Dir) override
	{
		BLOCKTYPE Block;
		NIBBLETYPE Meta;
		if(a_Dir == 1)
		{
			Meta = cSign::RotationToMetaData(a_Player->GetRotation());
			Block = E_BLOCK_SIGN_POST;
		}else{
			Meta = cSign::DirectionToMetaData(a_Dir);
			Block = E_BLOCK_WALLSIGN;
		}

		a_World->SetBlock(a_X, a_Y, a_Z, Block, Meta);
		OnPlacedByPlayer(a_World, a_Player, a_X, a_Y, a_Z, a_Dir);
	}

	virtual int GetDropID() override
	{
		return E_ITEM_SIGN;
	}
	
	virtual bool AllowBlockOnTop() override
	{
		return false;
	}

	virtual AString GetStepSound(void) override
	{
		return "step.wood";
	}
};
