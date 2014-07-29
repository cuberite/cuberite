
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
		a_Pickups.push_back(cItem(E_BLOCK_ANVIL, 1, a_BlockMeta >> 2));
	}


	virtual void OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		cWindow * Window = new cAnvilWindow(a_BlockX, a_BlockY, a_BlockZ);
		a_Player->OpenWindow(Window);
	}
	
	
	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		NIBBLETYPE HighBits = a_BlockMeta & 0x0c;  // Only highest two bits are preserved
		int Direction = (int)floor(a_Player->GetYaw() * 4.0 / 360.0 + 1.5) & 0x3;
		switch (Direction)
		{
			case 0: a_BlockMeta = 0x2 | HighBits; break;
			case 1: a_BlockMeta = 0x3 | HighBits; break;
			case 2: a_BlockMeta = 0x0 | HighBits; break;
			case 3: a_BlockMeta = 0x1 | HighBits; break;
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




