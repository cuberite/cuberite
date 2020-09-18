
#pragma once

#include "BlockHandler.h"
#include "../UI/EnchantingWindow.h"
#include "../Entities/Player.h"





class cBlockEnchantmentTableHandler:
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual bool OnUse(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace,
		const Vector3i a_CursorPos
	) const override
	{
		cWindow * Window = new cEnchantingWindow(a_BlockPos);
		a_Player.OpenWindow(*Window);
		return true;
	}





	virtual bool IsUseable(void) const override
	{
		return true;
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 29;
	}
};




