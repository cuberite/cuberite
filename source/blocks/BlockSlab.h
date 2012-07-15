#pragma once
#include "Block.h"


class cBlockSlabHandler : public cBlockHandler
{
public:
	cBlockSlabHandler(BLOCKTYPE a_BlockID);
	virtual char GetDropMeta(char a_BlockMeta)
	{
		return a_BlockMeta;
	}
	
	virtual char GetDropCount()
	{
		if(m_BlockID == E_BLOCK_DOUBLE_STONE_SLAB
			|| m_BlockID == E_BLOCK_DOUBLE_WOODEN_SLAB)
			return 2;
		return 1;
	}

	virtual void PlaceBlock(cWorld *a_World, cPlayer *a_Player, char a_BlockMeta, int a_X, int a_Y, int a_Z, char a_Dir);
	
	static char DirectionToMetaData( char a_Direction, NIBBLETYPE Meta )
	{
		char result = Meta;
		if( a_Direction == 0)
		{
		  result |= 0x8;
		}
		return result;
	}
};