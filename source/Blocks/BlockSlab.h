
#pragma once

#include "BlockHandler.h"





class cBlockSlabHandler :
	public cBlockHandler
{
public:
	cBlockSlabHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		char Count = ((m_BlockType == E_BLOCK_DOUBLE_STONE_SLAB) || (m_BlockType == E_BLOCK_DOUBLE_WOODEN_SLAB)) ? 2 : 1;
		a_Pickups.push_back(cItem(m_BlockType, Count, a_BlockMeta));
	}


	virtual void PlaceBlock(cWorld *a_World, cPlayer *a_Player, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override
	{
		a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, m_BlockType, DirectionToMetaData( a_Dir, a_BlockMeta ));
		OnPlacedByPlayer(a_World, a_Player, a_BlockX, a_BlockY, a_BlockZ, a_Dir);
	}
	
	
	static char DirectionToMetaData( char a_Direction, NIBBLETYPE Meta )
	{
		char result = Meta;
		if( a_Direction == 0)
		{
		  result |= 0x8;
		}
		return result;
	}


	virtual const char * GetStepSound(void) override
	{		
		return ((m_BlockType == E_BLOCK_WOODEN_SLAB) || (m_BlockType == E_BLOCK_DOUBLE_WOODEN_SLAB)) ?  "step.wood" : "step.stone";
	}
} ;




