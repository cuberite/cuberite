
#pragma once

#include "../../Blocks/BlockComparator.h"





namespace RedstoneComparatorHandler
{
	static unsigned char GetFrontPowerLevel(BlockState a_Block, unsigned char a_HighestSidePowerLevel, unsigned char a_HighestRearPowerLevel)
	{
		if (cBlockComparatorHandler::IsInSubtractionMode(a_Block))
		{
			// Subtraction mode
			return static_cast<unsigned char>(std::max(static_cast<char>(a_HighestRearPowerLevel) - a_HighestSidePowerLevel, 0));
		}
		else
		{
			// Comparison mode
			return (a_HighestRearPowerLevel < a_HighestSidePowerLevel) ? 0 : a_HighestRearPowerLevel;
		}
	}

	static PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, Vector3i a_QueryPosition, BlockState a_QueryBlock, bool IsLinked)
	{
		UNUSED(a_QueryPosition);
		UNUSED(a_QueryBlock);

		return (
			(cBlockComparatorHandler::GetFrontCoordinate(a_Position, a_Block) == a_QueryPosition) ?
			DataForChunk(a_Chunk).GetCachedPowerData(a_Position) : 0
		);
	}

	static unsigned char GetPowerLevel(cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block)
	{
		UInt8 SignalStrength = 0;
		auto RearCoordinate = cBlockComparatorHandler::GetRearCoordinate(a_Position, a_Block);

		auto RearChunk = a_Chunk.GetRelNeighborChunkAdjustCoords(RearCoordinate);
		if ((RearChunk == nullptr) || !RearChunk->IsValid())
		{
			return SignalStrength;
		}

		RearChunk->DoWithBlockEntityAt(RearCoordinate, [&](cBlockEntity & a_BlockEntity)
		{
			// Skip BlockEntities that don't have slots
			auto BlockEntityWithItems = dynamic_cast<cBlockEntityWithItems *>(&a_BlockEntity);
			if (BlockEntityWithItems == nullptr)
			{
				return false;
			}

			// TODO: handle double chests

			auto & Contents = BlockEntityWithItems->GetContents();
			float Fullness = 0;  // Is a floating-point type to allow later calculation to produce a non-truncated value

			for (int Slot = 0; Slot != Contents.GetNumSlots(); ++Slot)
			{
				Fullness += static_cast<float>(Contents.GetSlot(Slot).m_ItemCount) / Contents.GetSlot(Slot).GetMaxStackSize();
			}

			SignalStrength = (Fullness < 0.001 /* container empty? */) ? 0 : static_cast<UInt8>(1 + (Fullness / Contents.GetNumSlots()) * 14);
			return false;
		});

		const auto RearType = RearChunk->GetBlock(RearCoordinate);
		return std::max(
			SignalStrength,
			RedstoneHandler::GetPowerDeliveredToPosition(
				*RearChunk, RearCoordinate, RearType,
				cIncrementalRedstoneSimulatorChunkData::RebaseRelativePosition(a_Chunk, *RearChunk, a_Position), a_Block, false
			)
		);
	}

	static void Update(cChunk & a_Chunk, cChunk & CurrentlyTicking, Vector3i a_Position, BlockState a_Block, const PowerLevel Power)
	{
		// Note that Power here contains the maximum * side * power level, as specified by GetValidSourcePositions
		LOGREDSTONE("Evaluating ALU the comparator (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		auto & Data = DataForChunk(a_Chunk);
		auto DelayInfo = Data.GetMechanismDelayInfo(a_Position);

		// Delay is used here to prevent an infinite loop (#3168)
		if (DelayInfo == nullptr)
		{
			const auto RearPower = GetPowerLevel(a_Chunk, a_Position, a_Block);
			const auto FrontPower = GetFrontPowerLevel(a_Block, Power, RearPower);
			const auto PreviousFrontPower = Data.GetCachedPowerData(a_Position);
			const bool ShouldUpdate = (FrontPower != PreviousFrontPower);  // "Business logic" (:P) - determined by side and rear power levels

			if (ShouldUpdate)
			{
				Data.m_MechanismDelays[a_Position] = std::make_pair(1, bool());
			}

			return;
		}

		int DelayTicks;
		std::tie(DelayTicks, std::ignore) = *DelayInfo;

		if (DelayTicks != 0)
		{
			return;
		}

		const auto RearPower = GetPowerLevel(a_Chunk, a_Position, a_Block);
		const auto FrontPower = GetFrontPowerLevel(a_Block, Power, RearPower);

		// Don't care about the previous power level so return value ignored
		Data.ExchangeUpdateOncePowerData(a_Position, FrontPower);

		using namespace Block;
		a_Chunk.SetBlock(a_Position, Comparator::Comparator(Comparator::Facing(a_Block), Comparator::Mode(a_Block), FrontPower > 0));
		Data.m_MechanismDelays.erase(a_Position);

		// Assume that an update (to front power) is needed:
		UpdateAdjustedRelative(a_Chunk, CurrentlyTicking, a_Position, cBlockComparatorHandler::GetFrontCoordinate(a_Position, a_Block) - a_Position);
	}

	static void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);

		Callback(cBlockComparatorHandler::GetSideCoordinate(a_Position, a_Block, false));
		Callback(cBlockComparatorHandler::GetSideCoordinate(a_Position, a_Block, true));
	}
};
