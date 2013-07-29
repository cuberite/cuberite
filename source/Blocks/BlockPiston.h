
#pragma once

#include "BlockHandler.h"





class cBlockPistonHandler :
	public cBlockHandler
{
public:
	cBlockPistonHandler(BLOCKTYPE a_BlockType);
	
	virtual void OnDestroyed(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override;

	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override;
} ;




