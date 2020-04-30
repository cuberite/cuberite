#pragma once

#include "BlockHandler.h"





class cBlockCakeHandler:
	public cBlockHandler
{
	using Super = cBlockHandler;
public:
	cBlockCakeHandler(BLOCKTYPE a_BlockType):
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
		NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(a_BlockPos);

		if (!a_Player.Feed(2, 0.4))
		{
			return false;
		}

		if (Meta >= 5)
		{
			a_ChunkInterface.DigBlock(a_WorldInterface, a_BlockPos);
		}
		else
		{
			a_ChunkInterface.SetBlockMeta(a_BlockPos, Meta + 1);
		}
		return true;
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		// Give nothing
		return {};
	}





	virtual bool IsUseable(void) override
	{
		return true;
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 14;
	}
} ;




