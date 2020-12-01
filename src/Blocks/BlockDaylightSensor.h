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
	)const override
	{
	BLOCKTYPE BlockType = a_ChunkInterface.GetBlock(a_BlockPos);
	if (BlockType == E_BLOCK_DAYLIGHT_SENSOR)
	{
	a_ChunkInterface.FastSetBlock(a_BlockPos, E_BLOCK_INVERTED_DAYLIGHT_SENSOR, 0);
	}
	else
	{
	a_ChunkInterface.FastSetBlock(a_BlockPos, E_BLOCK_DAYLIGHT_SENSOR, 0);
	}

	return true;
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
	// Always drop the regular daylight sensor:
	return(cItem(E_BLOCK_DAYLIGHT_SENSOR, 1, 0));
	}





	virtual bool IsUseable(void) const override
	{
	return true;
	}
};
