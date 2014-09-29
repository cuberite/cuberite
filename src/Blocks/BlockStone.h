
#pragma once

#include "BlockHandler.h"
#include "Defines.h"




class cBlockStoneHandler :
	public cBlockHandler
{
public:
	cBlockStoneHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		switch(a_BlockMeta) {
			case NORMAL_STONE:
				a_Pickups.push_back(cItem(E_BLOCK_COBBLESTONE, 1, 0));
				break;
			default:
				a_Pickups.push_back(cItem(E_BLOCK_STONE, 1, a_BlockMeta));
		}
	}
};




