
#pragma once

#include "../../Blocks/BlockObserver.h"





namespace ObserverHandler
{
	static bool IsOn(BlockState a_Block)
	{
		return Block::Observer::Powered(a_Block);
	}

	static bool ShouldPowerOn(cChunk & Chunk, const Vector3i a_Position, BlockState a_Block, cIncrementalRedstoneSimulatorChunkData & a_Data)
	{
		BlockState Other = 0;
		if (!Chunk.UnboundedRelGetBlock(a_Position + cBlockObserverHandler::GetObservingFaceOffset(a_Block), Other))
		{
			return false;
		}

		auto & ObserverCache = a_Data.ObserverCache;
		const auto FindResult = ObserverCache.find(a_Position);

		if (FindResult == ObserverCache.end())
		{
			// Cache the last seen block for this position:
			ObserverCache.emplace(a_Position, Other);

			// Definitely should signal update:
			return true;
		}

		// The block this observer previously saw.
		const auto Previous = FindResult->second;

		// Update the last seen block:
		FindResult->second = Other;

		// Determine if to signal an update based on the block previously observed changed
		return Previous != Other;
	}

	static PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, Vector3i a_QueryPosition, BlockState a_QueryBlock, bool IsLinked)
	{
		return (IsOn(a_QueryBlock) && (a_QueryPosition == (a_Position + cBlockObserverHandler::GetSignalOutputOffset(a_Block)))) ? 15 : 0;
	}

	static void Update(cChunk & a_Chunk, cChunk & CurrentlyTicking, Vector3i a_Position, BlockState a_Block, const PowerLevel Power)
	{
		LOGREDSTONE("Evaluating Lenny the observer (%i %i %i)", a_Position.x, a_Position.y, a_Position.z);

		auto & Data = DataForChunk(a_Chunk);
		auto DelayInfo = Data.GetMechanismDelayInfo(a_Position);

		if (DelayInfo == nullptr)
		{
			if (!ShouldPowerOn(a_Chunk, a_Position, a_Block, Data))
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

		cChunkInterface ChunkInterface(a_Chunk.GetWorld()->GetChunkMap());
		cBlockObserverHandler::Toggle(ChunkInterface, a_Position);

		if (ShouldPowerOn)
		{
			// Remain on for 1 tick before resetting
			*DelayInfo = std::make_pair(1, false);
		}
		else
		{
			// We've reset. Erase delay data in preparation for detecting further updates
			Data.m_MechanismDelays.erase(a_Position);
		}

		UpdateAdjustedRelative(a_Chunk, CurrentlyTicking, a_Position, cBlockObserverHandler::GetSignalOutputOffset(a_Block));
	}

	static void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_Block);
	}
};
