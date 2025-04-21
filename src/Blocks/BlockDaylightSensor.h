
#pragma once

#include "BlockHandler.h"
#include "ChunkInterface.h"




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
		using namespace Block;

		auto BlockToReplace = a_ChunkInterface.GetBlock(a_BlockPos);
		BlockToReplace = DaylightDetector::DaylightDetector(!DaylightDetector::Inverted(BlockToReplace), Block::DaylightDetector::Power(BlockToReplace));
		a_ChunkInterface.SetBlock(a_BlockPos, BlockToReplace);
		return true;
	}


	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		UNUSED(a_Tool);
		// Always drop the regular daylight sensor:
		return { Item::DaylightDetector };
	}


	virtual bool IsUseable(void) const override
	{
		return true;
	}

public:

	static unsigned char GetPowerLevel(BlockState a_Block)
	{
		return Block::DaylightDetector::Power(a_Block);
	}
};
