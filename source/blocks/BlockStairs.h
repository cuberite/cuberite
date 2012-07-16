#pragma once
#include "Block.h"
#include "../cStairs.h"

class cBlockStairsHandler : public cBlockHandler
{
public:
	cBlockStairsHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{

	}
	
	
	virtual void PlaceBlock(cWorld *a_World, cPlayer *a_Player, NIBBLETYPE a_BlockMeta, int a_X, int a_Y, int a_Z, char a_Dir) override
	{
		a_World->SetBlock(a_X, a_Y, a_Z, m_BlockID, cStairs::RotationToMetaData(a_Player->GetRotation(), a_Dir));
		OnPlacedByPlayer(a_World, a_Player, a_X, a_Y, a_Z, a_Dir);
	}
	
	
};