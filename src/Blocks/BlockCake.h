#pragma once

#include "BlockHandler.h"





class cBlockCakeHandler final :
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
		NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(a_BlockPos);

		if (!a_Player.Feed(2, 0.4))
		{
			return false;
		}

		a_Player.GetStatManager().AddValue(Statistic::EatCakeSlice);
		if (Meta >= 5)
		{
			a_ChunkInterface.DigBlock(a_WorldInterface, a_BlockPos, &a_Player);
		}
		else
		{
			a_ChunkInterface.SetBlockMeta(a_BlockPos, Meta + 1);
		}
		return true;
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// Give nothing
		return {};
	}





	virtual bool IsUseable(void) const override
	{
		return true;
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 14;
	}
} ;




