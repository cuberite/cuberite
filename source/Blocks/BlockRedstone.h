
#pragma once

#include "BlockHandler.h"
#include "../World.h"





class cBlockRedstoneHandler :
	public cBlockHandler
{
public:
	cBlockRedstoneHandler(BLOCKTYPE a_BlockType);

	virtual void OnDestroyed(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override;
	
	virtual bool DoesAllowBlockOnTop(void) override
	{
		return false;
	}


	virtual bool CanBeAt(int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		return ((a_RelY > 0) && g_BlockIsSolid[a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ)]);
	}
	
	
	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to 0
		a_Pickups.push_back(cItem(E_ITEM_REDSTONE_DUST, 1));
	}
	
	
	virtual bool CanBePlacedOnSide(void) override
	{
		return false;
	}
} ;




