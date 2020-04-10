
#pragma once

#include "BlockHandler.h"
#include "Mixins.h"
#include "../Entities/Player.h"
#include "../UI/AnvilWindow.h"





class cBlockAnvilHandler :
	public cYawRotator<cBlockHandler, 0x03, 0x03, 0x00, 0x01, 0x02>
{
public:
	using super = cYawRotator<cBlockHandler, 0x03, 0x03, 0x00, 0x01, 0x02>;

	cBlockAnvilHandler(BLOCKTYPE a_BlockType)
		: super(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		return cItem(m_BlockType, 1, a_BlockMeta >> 2);
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
		if (!super::GetPlacementBlockTypeMeta(a_ChunkInterface, a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, a_BlockType, a_BlockMeta))
		{
			return false;
		}

		a_BlockMeta = a_BlockMeta | static_cast<NIBBLETYPE>(a_Player.GetEquippedItem().m_ItemDamage << 2);
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




