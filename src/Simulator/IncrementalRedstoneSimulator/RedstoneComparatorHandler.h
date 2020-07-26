
#pragma once

#include "RedstoneHandler.h"
#include "../../Blocks/BlockComparator.h"





class cRedstoneComparatorHandler : public cRedstoneHandler
{
public:

	static unsigned char GetFrontPowerLevel(NIBBLETYPE a_Meta, unsigned char a_HighestSidePowerLevel, unsigned char a_HighestRearPowerLevel)
	{
		if (cBlockComparatorHandler::IsInSubtractionMode(a_Meta))
		{
			// Subtraction mode
			return static_cast<unsigned char>(std::max(static_cast<char>(a_HighestRearPowerLevel) - a_HighestSidePowerLevel, 0));
		}
		else
		{
			// Comparison mode
			return (std::max(a_HighestSidePowerLevel, a_HighestRearPowerLevel) == a_HighestSidePowerLevel) ? 0 : a_HighestRearPowerLevel;
		}
	}

	virtual unsigned char GetPowerDeliveredToPosition(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType) const override
	{
		UNUSED(a_QueryPosition);
		UNUSED(a_QueryBlockType);

		return (
			(cBlockComparatorHandler::GetFrontCoordinate(a_Position, a_Meta & 0x3) == a_QueryPosition) ?
			DataForChunk(a_Chunk).GetCachedPowerData(a_Position).PowerLevel : 0
		);
	}

	static unsigned char GetPowerLevel(cChunk & a_Chunk, Vector3i Position, BLOCKTYPE BlockType, NIBBLETYPE Meta)
	{
		UInt8 SignalStrength = 0;
		auto RearCoordinate = cBlockComparatorHandler::GetRearCoordinate(Position, Meta & 0x3);

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

		BLOCKTYPE RearType;
		NIBBLETYPE RearMeta;
		RearChunk->GetBlockTypeMeta(RearCoordinate, RearType, RearMeta);

		auto PotentialSourceHandler = cIncrementalRedstoneSimulator::GetComponentHandler(RearType);
		if (PotentialSourceHandler == nullptr)
		{
			return SignalStrength;
		}

		return std::max(
			SignalStrength,
			PotentialSourceHandler->GetPowerDeliveredToPosition(
				*RearChunk, RearCoordinate, RearType, RearMeta,
				cIncrementalRedstoneSimulatorChunkData::RebaseRelativePosition(a_Chunk, *RearChunk, Position), BlockType
			)
		);
	}

	virtual void Update(cChunk & a_Chunk, cChunk & CurrentlyTicking, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) const override
	{
		// Note that a_PoweringData here contains the maximum * side * power level, as specified by GetValidSourcePositions
		// LOGD("Evaluating ALU the comparator (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		auto & Data = DataForChunk(a_Chunk);
		auto DelayInfo = Data.GetMechanismDelayInfo(a_Position);

		// Delay is used here to prevent an infinite loop (#3168)
		if (DelayInfo == nullptr)
		{
			const auto RearPower = GetPowerLevel(a_Chunk, a_Position, a_BlockType, a_Meta);
			const auto FrontPower = GetFrontPowerLevel(a_Meta, a_PoweringData.PowerLevel, RearPower);
			const auto PreviousFrontPower = Data.GetCachedPowerData(a_Position);
			const bool ShouldUpdate = (FrontPower != PreviousFrontPower.PowerLevel);  // "Business logic" (:P) - determined by side and rear power levels

			if (ShouldUpdate)
			{
				Data.m_MechanismDelays[a_Position] = std::make_pair(1, bool());
			}
		}
		else
		{
			int DelayTicks;
			std::tie(DelayTicks, std::ignore) = *DelayInfo;

			if (DelayTicks == 0)
			{
				const auto RearPower = GetPowerLevel(a_Chunk, a_Position, a_BlockType, a_Meta);
				const auto FrontPower = GetFrontPowerLevel(a_Meta, a_PoweringData.PowerLevel, RearPower);
				const auto NewMeta = (FrontPower > 0) ? (a_Meta | 0x8) : (a_Meta & 0x7);

				// Don't care about the previous power level so return value ignored
				Data.ExchangeUpdateOncePowerData(a_Position, PoweringData(a_PoweringData.PoweringBlock, FrontPower));

				a_Chunk.SetMeta(a_Position, NewMeta);
				Data.m_MechanismDelays.erase(a_Position);

				// Assume that an update (to front power) is needed:
				UpdateAdjustedRelatives(a_Chunk, CurrentlyTicking, cBlockComparatorHandler::GetFrontCoordinate(a_Position, a_Meta & 0x3));
			}
		}
	}

	virtual void ForValidSourcePositions(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, SourceCallback Callback) const override
	{
		UNUSED(a_Chunk);
		UNUSED(a_BlockType);

		Callback(cBlockComparatorHandler::GetSideCoordinate(a_Position, a_Meta & 0x3, false));
		Callback(cBlockComparatorHandler::GetSideCoordinate(a_Position, a_Meta & 0x3, true));
	}
};
