
#pragma once

#include "BlockHandler.h"





class cBlockFluidHandler :
	public cBlockHandler
{
	typedef cBlockHandler super;
	
public:
	cBlockFluidHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{

	}
	
	
	virtual bool DoesIgnoreBuildCollision(void) override
	{
		return true;
	}
	
	
	virtual void Check(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		switch (m_BlockType)
		{
			case E_BLOCK_STATIONARY_LAVA:
			{
				a_World->FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_LAVA, a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ));
				break;
			}
			case E_BLOCK_STATIONARY_WATER:
			{
				a_World->FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_WATER, a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ));
				break;
			}
		}
		super::Check(a_World, a_BlockX, a_BlockY, a_BlockZ);
	}
} ;




