
#pragma once

#include "RedstoneHandler.h"
#include "Blocks/BlockComparator.h"





class cRedstoneComparatorHandler : public cRedstoneHandler
{
	typedef cRedstoneHandler super;
public:

	unsigned char GetFrontPowerLevel(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, unsigned char a_HighestSidePowerLevel, unsigned char a_HighestRearPowerLevel) const
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

	virtual unsigned char GetPowerDeliveredToPosition(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType) const override
	{
		UNUSED(a_QueryPosition);
		UNUSED(a_QueryBlockType);
		auto ChunkData = static_cast<cIncrementalRedstoneSimulator *>(a_World.GetRedstoneSimulator())->GetChunkData();

		return (
			(cBlockComparatorHandler::GetFrontCoordinate(a_Position, a_Meta & 0x3) == a_QueryPosition) ?
			ChunkData->GetCachedPowerData(a_Position).PowerLevel : 0
		);
	}

	virtual unsigned char GetPowerLevel(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Position);
		UNUSED(a_BlockType);

		UInt8 SignalStrength = 0;
		auto RearCoordinate = cBlockComparatorHandler::GetRearCoordinate(a_Position, a_Meta & 0x3);
		a_World.DoWithBlockEntityAt(RearCoordinate.x, RearCoordinate.y, RearCoordinate.z, [&](cBlockEntity & a_BlockEntity)
			{
				// Skip BlockEntities that don't have slots
				auto BlockEntityWithItems = dynamic_cast<cBlockEntityWithItems *>(&a_BlockEntity);
				if (BlockEntityWithItems == nullptr)
				{
					return false;
				}

				auto & Contents = BlockEntityWithItems->GetContents();
				float Fullness = 0;  // Is a floating-point type to allow later calculation to produce a non-truncated value

				for (int Slot = 0; Slot != Contents.GetNumSlots(); ++Slot)
				{
					Fullness += static_cast<float>(Contents.GetSlot(Slot).m_ItemCount) / Contents.GetSlot(Slot).GetMaxStackSize();
				}

				SignalStrength = (Fullness < 0.001 /* container empty? */) ? 0 : static_cast<UInt8>(1 + (Fullness / Contents.GetNumSlots()) * 14);
				return false;
			}
		);
		auto RearPower = SignalStrength;
		auto RearType = a_World.GetBlock(RearCoordinate);

		auto PotentialSourceHandler = cIncrementalRedstoneSimulator::GetComponentHandler(RearType);
		if (PotentialSourceHandler != nullptr)
		{
			NIBBLETYPE RearMeta = a_World.GetBlockMeta(RearCoordinate);
			RearPower = std::max(SignalStrength, PotentialSourceHandler->GetPowerDeliveredToPosition(a_World, RearCoordinate, RearType, RearMeta, a_Position, a_BlockType));
		}

		return RearPower;
	}

	virtual cVector3iArray Update(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) const override
	{
		// Note that a_PoweringData here contains the maximum * side * power level, as specified by GetValidSourcePositions
		// LOGD("Evaluating ALU the comparator (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);
		auto Data = static_cast<cIncrementalRedstoneSimulator *>(a_World.GetRedstoneSimulator())->GetChunkData();
		auto DelayInfo = Data->GetMechanismDelayInfo(a_Position);

		// Delay is used here to prevent an infinite loop (#3168)
		if (DelayInfo == nullptr)
		{
			auto RearPower = GetPowerLevel(a_World, a_Position, a_BlockType, a_Meta);
			auto FrontPower = GetFrontPowerLevel(a_World, a_Position, a_BlockType, a_Meta, a_PoweringData.PowerLevel, RearPower);
			auto PreviousFrontPower = Data->ExchangeUpdateOncePowerData(a_Position, PoweringData(a_PoweringData.PoweringBlock, FrontPower));

			bool ShouldBeOn = (RearPower > 0);  // Provide visual indication by examining * rear * power level
			bool ShouldUpdate = (FrontPower != PreviousFrontPower.PowerLevel);  // "Business logic" (:P) - determine by examining *side* power levels

			if (ShouldUpdate || (ShouldBeOn != cBlockComparatorHandler::IsOn(a_Meta)))
			{
				Data->m_MechanismDelays[a_Position] = std::make_pair(1, ShouldBeOn);
			}
		}
		else
		{
			int DelayTicks;
			bool ShouldPowerOn;
			std::tie(DelayTicks, ShouldPowerOn) = *DelayInfo;

			if (DelayTicks == 0)
			{
				a_World.SetBlockMeta(a_Position, ShouldPowerOn ? (a_Meta | 0x8) : (a_Meta & 0x7));
				Data->m_MechanismDelays.erase(a_Position);

				// Assume that an update (to front power) is needed.
				// Note: potential inconsistencies will arise as power data is updated before-delay due to limitations of the power data caching functionality (only stores one bool)
				// This means that other mechanisms like wires may get our new power data before our delay has finished
				// This also means that we have to manually update ourselves to be aware of any changes that happened in the previous redstone tick
				return StaticAppend(GetAdjustedRelatives(a_Position, GetRelativeLaterals()), cVector3iArray{ a_Position });
			}
		}

		return {};
	}

	virtual cVector3iArray GetValidSourcePositions(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_World);
		UNUSED(a_BlockType);
		return cVector3iArray {cBlockComparatorHandler::GetSideCoordinate(a_Position, a_Meta & 0x3, false), cBlockComparatorHandler::GetSideCoordinate(a_Position, a_Meta & 0x3, true)};
	}
};
