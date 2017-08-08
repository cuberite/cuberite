
#pragma once

#include "BlockHandler.h"
#include "../Entities/Player.h"
#include "../UI/AnvilWindow.h"





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

	virtual bool OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		cWindow * Window = new cAnvilWindow(a_BlockX, a_BlockY, a_BlockZ);
		a_Player.OpenWindow(*Window);
		return true;
	}

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer & a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		NIBBLETYPE Meta = static_cast<NIBBLETYPE>(a_Player.GetEquippedItem().m_ItemDamage);
		int Direction = static_cast<int>(floor(a_Player.GetYaw() * 4.0 / 360.0 + 1.5)) & 0x3;

		switch (Direction)
		{
			case 0: a_BlockMeta = static_cast<NIBBLETYPE>(0x2 | Meta << 2); break;
			case 1: a_BlockMeta = static_cast<NIBBLETYPE>(0x3 | Meta << 2); break;
			case 2: a_BlockMeta = static_cast<NIBBLETYPE>(0x0 | Meta << 2); break;
			case 3: a_BlockMeta = static_cast<NIBBLETYPE>(0x1 | Meta << 2); break;
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

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 6;
	}
} ;




