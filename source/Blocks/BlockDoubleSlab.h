
#pragma once

#include "BlockHandler.h"
#include "../Items/ItemHandler.h"





class cBlockDoubleSlabHandler :
	public cBlockHandler
{
public:
	cBlockDoubleSlabHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		if (m_BlockType ==  E_BLOCK_DOUBLE_STONE_SLAB)
		{
			m_BlockType = E_BLOCK_STONE_SLAB;
		}
		else
		{
			m_BlockType = E_BLOCK_WOODEN_SLAB;
		}
		a_Pickups.push_back(cItem(m_BlockType, 2, a_BlockMeta));
	}

	
	virtual const char * GetStepSound(void) override
	{		
		return ((m_BlockType == E_BLOCK_DOUBLE_WOODEN_SLAB) || (m_BlockType == E_BLOCK_DOUBLE_WOODEN_SLAB)) ?  "step.wood" : "step.stone";
	}
} ;




