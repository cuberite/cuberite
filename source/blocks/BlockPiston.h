#pragma once
#include "BlockHandler.h"


class cBlockPistonHandler : public cBlockHandler
{
public:
	cBlockPistonHandler(BLOCKTYPE a_BlockID);
	virtual void OnPlaced(cWorld *a_World, int a_X, int a_Y, int a_Z, int a_Dir) override;
	virtual void OnDestroyed(cWorld *a_World, int a_X, int a_Y, int a_Z) override;
	
	
	virtual void PlaceBlock(cWorld *a_World, cPlayer *a_Player, NIBBLETYPE a_BlockMeta, int a_X, int a_Y, int a_Z, char a_Dir) override;
	
};