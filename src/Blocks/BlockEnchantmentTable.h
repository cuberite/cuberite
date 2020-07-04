
#pragma once

#include "BlockHandler.h"
#include "../UI/EnchantingWindow.h"
#include "../Entities/Player.h"





class cBlockEnchantmentTableHandler:
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	cBlockEnchantmentTableHandler(BLOCKTYPE a_BlockType):
		Super(a_BlockType)
	{
	}





	virtual bool OnUse(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace,
		const Vector3i a_CursorPos
	) override
	{
		cWindow * Window = new cEnchantingWindow(a_BlockPos);
		a_Player.OpenWindow(*Window);
		return true;
	}





	virtual bool IsUseable(void) override
	{
		return true;
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 29;
	}
};




