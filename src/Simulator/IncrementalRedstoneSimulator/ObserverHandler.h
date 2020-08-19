
#pragma once

#include "../../Blocks/BlockObserver.h"





namespace ObserverHandler
{
	inline bool IsOn(NIBBLETYPE a_Meta)
	{
		return (a_Meta & 0x8) == 0x8;
	}

	inline bool ShouldPowerOn(cChunk & Chunk, const Vector3i a_Position, NIBBLETYPE a_Meta, cIncrementalRedstoneSimulatorChunkData & a_Data)
	{
		BLOCKTYPE BlockType;
		NIBBLETYPE BlockMeta;
		if (!Chunk.UnboundedRelGetBlock(a_Position + cBlockObserverHandler::GetObservingFaceOffset(a_Meta), BlockType, BlockMeta))
		{
			return false;
		}

		// Cache the last seen block type and meta in the power data for this position
		auto Observed = PoweringData(BlockType, BlockMeta);
		auto Previous = a_Data.ExchangeUpdateOncePowerData(a_Position, Observed);

		// Determine if to signal an update based on the block previously observed changed
		return (Previous.PoweringBlock != Observed.PoweringBlock) || (Previous.PowerLevel != Observed.PowerLevel);
	}

	inline unsigned char GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType, bool IsLinked)
	{
		const auto Meta = a_Chunk.GetMeta(a_Position);
		return (IsOn(Meta) && (a_QueryPosition == (a_Position + cBlockObserverHandler::GetSignalOutputOffset(Meta)))) ? 15 : 0;
	}

	inline void Update(cChunk & a_Chunk, cChunk & CurrentlyTicking, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData)
	{
		// LOGD("Evaluating Lenny the observer (%i %i %i)", a_Position.x, a_Position.y, a_Position.z);

		auto & Data = DataForChunk(a_Chunk);
		auto DelayInfo = Data.GetMechanismDelayInfo(a_Position);

		if (DelayInfo == nullptr)
		{
			if (!ShouldPowerOn(a_Chunk, a_Position, a_Meta, Data))
			{
				return;
			}

			// From rest, we've determined there was a block update
			// Schedule power-on 1 tick in the future
			Data.m_MechanismDelays[a_Position] = std::make_pair(1, true);

			return;
		}

		int DelayTicks;
		bool ShouldPowerOn;
		std::tie(DelayTicks, ShouldPowerOn) = *DelayInfo;

		if (DelayTicks != 0)
		{
			return;
		}

		if (ShouldPowerOn)
		{
			// Remain on for 1 tick before resetting
			*DelayInfo = std::make_pair(1, false);
			a_Chunk.SetMeta(a_Position, a_Meta | 0x8);
		}
		else
		{
			// We've reset. Erase delay data in preparation for detecting further updates
			Data.m_MechanismDelays.erase(a_Position);
			a_Chunk.SetMeta(a_Position, a_Meta & ~0x8);
		}

		UpdateAdjustedRelative(a_Chunk, CurrentlyTicking, a_Position, cBlockObserverHandler::GetSignalOutputOffset(a_Meta));
	}

	inline void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_BlockType);
	}
};
