
#pragma once

#include "BlockEntity.h"
#include "../World.h"
#include "../Piston.h"
#include "../Player.h"





class cBlockChestHandler :
	public cBlockEntityHandler
{
public:
	cBlockChestHandler(BLOCKTYPE a_BlockType)
		: cBlockEntityHandler(a_BlockType)
	{
	}

	
	virtual void PlaceBlock(cWorld * a_World, cPlayer * a_Player, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override
	{
		a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, m_BlockType, cPiston::RotationPitchToMetaData(a_Player->GetRotation(), 0));
		OnPlacedByPlayer(a_World, a_Player, a_BlockX, a_BlockY, a_BlockZ, a_Dir);
	}
	
	
	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
} ;




