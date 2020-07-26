
#pragma once

#include "RedstoneHandler.h"





class cSolidBlockHandler:
	public cRedstoneHandler
{
	using Super = cRedstoneHandler;

public:

	virtual unsigned char GetPowerDeliveredToPosition(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType) const override
	{
		const auto SolidBlock = DataForChunk(a_Chunk).GetCachedPowerData(a_Position);
		return (
			!cIncrementalRedstoneSimulator::IsRedstone(a_QueryBlockType) ||
			(
				(a_QueryBlockType == E_BLOCK_REDSTONE_WIRE) &&
				(SolidBlock.PoweringBlock == E_BLOCK_REDSTONE_WIRE)
			)
		) ? 0 : SolidBlock.PowerLevel;
	}

	virtual void Update(cChunk & a_Chunk, cChunk & CurrentlyTicking, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) const override
	{
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		// LOGD("Evaluating blocky the generic block (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		auto PreviousPower = DataForChunk(a_Chunk).ExchangeUpdateOncePowerData(a_Position, a_PoweringData);
		if ((a_PoweringData != PreviousPower) || (a_PoweringData.PoweringBlock != PreviousPower.PoweringBlock))
		{
			UpdateAdjustedRelatives(a_Chunk, CurrentlyTicking, a_Position, RelativeAdjacents);
		}
	}

	virtual void ForValidSourcePositions(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, SourceCallback Callback) const override
	{
		UNUSED(a_Chunk);
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
		InvokeForAdjustedRelatives(Callback, a_Position, RelativeAdjacents);
	}
};
