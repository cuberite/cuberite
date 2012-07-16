#pragma once
#include "Block.h"


class cBlockSlabHandler : public cBlockHandler
{
public:
	cBlockSlabHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}

	virtual NIBBLETYPE GetDropMeta(NIBBLETYPE a_BlockMeta) override
	{
		return a_BlockMeta;
	}
	
	virtual char GetDropCount() override
	{
		if(m_BlockID == E_BLOCK_DOUBLE_STONE_SLAB
			|| m_BlockID == E_BLOCK_DOUBLE_WOODEN_SLAB)
			return 2;
		return 1;
	}


	virtual void PlaceBlock(cWorld *a_World, cPlayer *a_Player, NIBBLETYPE a_BlockMeta, int a_X, int a_Y, int a_Z, char a_Dir) override
	{
		a_World->SetBlock(a_X, a_Y, a_Z, m_BlockID, DirectionToMetaData( a_Dir, a_BlockMeta ));
		OnPlacedByPlayer(a_World, a_Player, a_X, a_Y, a_Z, a_Dir);
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
};