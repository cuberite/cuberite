
#pragma once

#include "BlockHandler.h"





class cBlockDaylightSensorHandler final :
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
		if (a_ChunkInterface.GetBlock(a_BlockPos) == E_BLOCK_DAYLIGHT_SENSOR)
		{
			a_ChunkInterface.SetBlock(a_BlockPos, E_BLOCK_INVERTED_DAYLIGHT_SENSOR, 0);
		}
		else
		{
			a_ChunkInterface.SetBlock(a_BlockPos, E_BLOCK_DAYLIGHT_SENSOR, 0);
		}

		return true;
	}


	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		// Always drop the regular daylight sensor:
		return { E_BLOCK_DAYLIGHT_SENSOR };
	}


	virtual bool IsUseable(void) const override
	{
		return true;
	}
};
