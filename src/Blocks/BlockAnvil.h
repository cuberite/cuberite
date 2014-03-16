
#pragma once

#include "BlockHandler.h"
#include "../World.h"
#include "../Entities/Player.h"





class cBlockAnvilHandler :
	public cBlockHandler
{
public:
	cBlockAnvilHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}
	
	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_BLOCK_ANVIL, 1, a_BlockMeta));
	}
	
	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		
		int Direction = (int)floor(a_Player->GetYaw() * 4.0 / 360.0 + 0.5) & 0x3;
		int RawMeta = a_BlockMeta >> 2;
		
		Direction++;
		Direction %= 4;
		switch (Direction)
		{
			case 0:      a_BlockMeta = 0x2 | RawMeta << 2; break;
			case 1:      a_BlockMeta = 0x3 | RawMeta << 2; break;
			case 2:      a_BlockMeta = 0x0 | RawMeta << 2; break;
			case 3:      a_BlockMeta = 0x1 | RawMeta << 2; break;
			default:
			{
				return false;
			}
		}
		
		return true;
	}

	virtual bool IsUseable() override
	{
		return true;
	}
} ;




