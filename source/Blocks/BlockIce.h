
#pragma once

#include "BlockHandler.h"
#include "../World.h"





class cBlockIceHandler :
	public cBlockHandler
{
public:
	cBlockIceHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// No pickups
	}
	
	
	virtual void OnDestroyed(cWorld * a_World, int a_X, int a_Y, int a_Z) override
	{
		// TODO: Ice destroyed with air below it should turn into air instead of water
		a_World->FastSetBlock(a_X, a_Y, a_Z, E_BLOCK_STATIONARY_WATER, 8);
		// This is called later than the real destroying of this ice block
	}	
} ;




