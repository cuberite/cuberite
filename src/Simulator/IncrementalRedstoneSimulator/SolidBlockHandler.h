
#pragma once

#include "RedstoneHandler.h"





class cSolidBlockHandler : public cRedstoneHandler
{
	typedef cRedstoneHandler super;
public:

	cSolidBlockHandler(cWorld & a_World) :
		super(a_World)
	{
	}

	virtual unsigned char GetPowerDeliveredToPosition(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, const Vector3i & a_QueryPosition, BLOCKTYPE a_QueryBlockType) override
	{
		// TODO: wire isn't linked powered only if the source was a wire, not just because it is a wire
		return (
			!cIncrementalRedstoneSimulator::IsRedstone(a_QueryBlockType) ||
			(
				(a_QueryBlockType == E_BLOCK_REDSTONE_WIRE) &&
				(static_cast<cIncrementalRedstoneSimulator *>(m_World.GetRedstoneSimulator())->GetChunkData()->GetCachedPowerData(a_Position).PoweringBlock == E_BLOCK_REDSTONE_WIRE)
			)
		) ? 0 : GetPowerLevel(a_Position, a_BlockType, a_Meta);
	}

	virtual unsigned char GetPowerLevel(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) override
	{
		return static_cast<cIncrementalRedstoneSimulator *>(m_World.GetRedstoneSimulator())->GetChunkData()->GetCachedPowerData(a_Position).PowerLevel;
	}

	virtual cVector3iArray Update(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) override
	{
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		LOGD("Evaluating blocky the generic block (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		auto PreviousPower = static_cast<cIncrementalRedstoneSimulator *>(m_World.GetRedstoneSimulator())->GetChunkData()->ExchangeUpdateOncePowerData(a_Position, a_PoweringData);
		if ((a_PoweringData != PreviousPower) || (a_PoweringData.PoweringBlock != PreviousPower.PoweringBlock))
		{
			return GetAdjustedRelatives(a_Position, GetRelativeAdjacents());
		}

		return {};
	}

	virtual cVector3iArray GetValidSourcePositions(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) override
	{
		UNUSED(a_BlockType);
		UNUSED(a_Meta);

		/* TODO: is this more performant?
		cVector3iArray Adjacents;
		for (const auto Offset : GetRelativeAdjacents())
		{
			auto Position = Offset + a_Position;
			auto Block = m_World.GetBlock(Position);
			if ((Block == E_BLOCK_REDSTONE_REPEATER_ON) || (Block == E_BLOCK_REDSTONE_WIRE) || (Block == E_BLOCK_TRIPWIRE_HOOK))
			{
				Adjacents.emplace_back(Position);
			}
		}
		*/
		return GetAdjustedRelatives(a_Position, GetRelativeAdjacents());
	}
};
