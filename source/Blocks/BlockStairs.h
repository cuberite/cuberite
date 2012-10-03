#pragma once
#include "BlockHandler.h"
#include "../Stairs.h"

class cBlockStairsHandler : public cBlockHandler
{
public:
	cBlockStairsHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{

	}
	
	
	virtual void PlaceBlock(cWorld *a_World, cPlayer *a_Player, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override
	{
		a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, m_BlockType, cStairs::RotationToMetaData(a_Player->GetRotation(), a_Dir));
		OnPlacedByPlayer(a_World, a_Player, a_BlockX, a_BlockY, a_BlockZ, a_Dir);
	}
	
	//TODO: step sound
};
