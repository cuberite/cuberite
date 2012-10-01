
#pragma once

#include "BlockEntity.h"
#include "../World.h"
#include "../Piston.h"
#include "../Player.h"





class cBlockFurnaceHandler :
	public cBlockEntityHandler
{
public:
	cBlockFurnaceHandler(BLOCKTYPE a_BlockID)
		: cBlockEntityHandler(a_BlockID)
	{
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_ITEM_FURNACE, 1, 0));
	}

	
	virtual void PlaceBlock(cWorld * a_World, cPlayer * a_Player, NIBBLETYPE a_BlockMeta, int a_X, int a_Y, int a_Z, char a_Dir) override
	{
		a_World->SetBlock(a_X, a_Y, a_Z, m_BlockID, cPiston::RotationPitchToMetaData(a_Player->GetRotation(), 0));
		OnPlacedByPlayer(a_World, a_Player, a_X, a_Y, a_Z, a_Dir);
	}
} ;




