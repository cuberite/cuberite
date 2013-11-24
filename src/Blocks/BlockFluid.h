
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
	
		
	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// No pickups
	}
	
	
	virtual bool DoesIgnoreBuildCollision(void) override
	{
		return true;
	}
	
	
	virtual void Check(int a_RelX, int a_RelY, int a_RelZ, cChunk & a_Chunk) override
	{
		switch (m_BlockType)
		{
			case E_BLOCK_STATIONARY_LAVA:
			{
				a_Chunk.FastSetBlock(a_RelX, a_RelY, a_RelZ, E_BLOCK_LAVA, a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ));
				break;
			}
			case E_BLOCK_STATIONARY_WATER:
			{
				a_Chunk.FastSetBlock(a_RelX, a_RelY, a_RelZ, E_BLOCK_WATER, a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ));
				break;
			}
		}
		super::Check(a_RelX, a_RelY, a_RelZ, a_Chunk);
	}
} ;




