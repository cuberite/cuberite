
#pragma once

#include "BlockHandler.h"
#include "../World.h"
#include "../Sign.h"
#include "../Player.h"





class cBlockSignHandler :
	public cBlockHandler
{
public:
	cBlockSignHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}
	

	virtual void PlaceBlock(cWorld * a_World, cPlayer * a_Player, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override
	{
		BLOCKTYPE Block;
		NIBBLETYPE Meta;
		if (a_Dir == BLOCK_FACE_TOP)
		{
			Meta = cSign::RotationToMetaData(a_Player->GetRotation());
			Block = E_BLOCK_SIGN_POST;
		}
		else
		{
			Meta = cSign::DirectionToMetaData(a_Dir);
			Block = E_BLOCK_WALLSIGN;
		}

		a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, Block, Meta);
		OnPlacedByPlayer(a_World, a_Player, a_BlockX, a_BlockY, a_BlockZ, a_Dir);
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_ITEM_SIGN, 1, 0));
	}

	
	virtual bool DoesAllowBlockOnTop(void) override
	{
		return false;
	}
	

	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
} ;




