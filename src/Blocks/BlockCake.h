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
		auto Self = a_ChunkInterface.GetBlock(a_BlockPos);

		if (!a_Player.Feed(2, 0.4))
		{
			return false;
		}

		a_Player.GetStatistics().Custom[CustomStatistic::EatCakeSlice]++;
		auto EatenSlices = Block::Cake::Bites(Self);
		if (EatenSlices >= 5)
		{
			a_ChunkInterface.DigBlock(a_WorldInterface, a_BlockPos, &a_Player);
		}
		else
		{
			a_ChunkInterface.SetBlock(a_BlockPos, Block::Cake::Cake(EatenSlices + 1));
		}
		return true;
	}





	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		// Give nothing
		return {};
	}





	virtual bool IsUseable(void) const override
	{
		return true;
	}

	virtual ColourID GetMapBaseColourID() const override
	{
		return 14;
	}
} ;




