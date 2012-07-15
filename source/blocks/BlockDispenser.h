#pragma once
#include "BlockEntity.h"
#include "../cWorld.h"
#include "../cPiston.h"
#include "../cPlayer.h"

class cBlockDispenserHandler : public cBlockEntityHandler
{
public:
	cBlockDispenserHandler(BLOCKTYPE a_BlockID)
		: cBlockEntityHandler(a_BlockID)
	{
	}
	
	virtual void PlaceBlock(cWorld *a_World, cPlayer *a_Player, char a_BlockMeta, int a_X, int a_Y, int a_Z, char a_Dir)
	{
		a_World->SetBlock(a_X, a_Y, a_Z, m_BlockID, cPiston::RotationPitchToMetaData(a_Player->GetRotation(), 0));
		OnPlacedByPlayer(a_World, a_Player, a_X, a_Y, a_Z, a_Dir);
	}
	
	
};