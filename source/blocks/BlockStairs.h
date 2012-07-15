#pragma once
#include "Block.h"


class cBlockStairsHandler : public cBlockHandler
{
public:
	cBlockStairsHandler(BLOCKTYPE a_BlockID);
	
	
	virtual void PlaceBlock(cWorld *a_World, cPlayer *a_Player, char a_BlockMeta, int a_X, int a_Y, int a_Z, char a_Dir);
	
	
};