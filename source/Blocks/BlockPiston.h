#pragma once
#include "BlockHandler.h"


class cBlockPistonHandler : public cBlockHandler
{
public:
	cBlockPistonHandler(BLOCKTYPE a_BlockType);
	virtual void OnPlaced(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ, int a_Dir) override;
	virtual void OnDestroyed(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override;
	
	
	virtual void PlaceBlock(cWorld *a_World, cPlayer *a_Player, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override;
	
};