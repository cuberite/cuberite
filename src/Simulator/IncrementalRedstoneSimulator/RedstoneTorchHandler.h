
#pragma once

#include "RedstoneHandler.h"





class cRedstoneTorchHandler : public cRedstoneHandler
{
public:

	inline static bool IsOn(BLOCKTYPE a_Block)
	{
		return (a_Block == E_BLOCK_REDSTONE_TORCH_ON);
	}

	inline static Vector3i GetOffsetAttachedTo(Vector3i a_Position, NIBBLETYPE a_Meta)
	{
		switch (a_Meta)
		{
			case E_META_TORCH_FLOOR: return { 0, -1, 0 };
			case E_META_TORCH_EAST: return { -1, 0, 0 };
			case E_META_TORCH_WEST: return { 1, 0, 0 };
			case E_META_TORCH_NORTH: return { 0, 0, 1 };
			case E_META_TORCH_SOUTH: return { 0, 0, -1 };
			default:
			{
				ASSERT(!"Unhandled torch metadata");
				return { 0, 0, 0 };
			}
		}
	}

	virtual unsigned char GetPowerDeliveredToPosition(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType) const override
	{
		if (
			IsOn(a_BlockType) &&
			(a_QueryPosition != (a_Position + GetOffsetAttachedTo(a_Position, a_Meta))) &&
			(cIncrementalRedstoneSimulator::IsMechanism(a_QueryBlockType) || (cBlockInfo::FullyOccupiesVoxel(a_QueryBlockType) && (a_QueryPosition == (a_Position + OffsetYP()))))
		)
		{
			return 15;
		}
		return 0;
	}

	virtual unsigned char GetPowerLevel(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const override
	{
		return IsOn(a_BlockType) ? 15 : 0;
	}

	virtual cVector3iArray Update(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) const override
	{
		// LOGD("Evaluating torchy the redstone torch (%i %i %i)", a_Position.x, a_Position.y, a_Position.z);

		auto Data = static_cast<cIncrementalRedstoneSimulator *>(a_World.GetRedstoneSimulator())->GetChunkData();
		auto DelayInfo = Data->GetMechanismDelayInfo(a_Position);

		if (DelayInfo == nullptr)
		{
			bool ShouldBeOn = (a_PoweringData.PowerLevel == 0);
			if (ShouldBeOn != IsOn(a_BlockType))
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
				a_World.SetBlock(a_Position.x, a_Position.y, a_Position.z, ShouldPowerOn ? E_BLOCK_REDSTONE_TORCH_ON : E_BLOCK_REDSTONE_TORCH_OFF, a_Meta);
				Data->m_MechanismDelays.erase(a_Position);

				cVector3iArray RelativePositions = GetRelativeAdjacents();
				RelativePositions.erase(std::remove(RelativePositions.begin(), RelativePositions.end(), GetOffsetAttachedTo(a_Position, a_Meta)), RelativePositions.end());
				return GetAdjustedRelatives(a_Position, RelativePositions);
			}
		}

		return {};
	}

	virtual cVector3iArray GetValidSourcePositions(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_World);
		UNUSED(a_BlockType);
		return { (a_Position + GetOffsetAttachedTo(a_Position, a_Meta)) };
	}
};
