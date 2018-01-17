
#pragma once

#include "RedstoneHandler.h"
#include "Blocks/BlockRedstoneRepeater.h"





class cRedstoneRepeaterHandler : public cRedstoneHandler
{
	typedef cRedstoneHandler super;
public:

	inline static bool IsOn(BLOCKTYPE a_Block)
	{
		return (a_Block == E_BLOCK_REDSTONE_REPEATER_ON);
	}

	virtual unsigned char GetPowerDeliveredToPosition(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType) const override
	{
		return (
			(a_QueryPosition == (a_Position + cBlockRedstoneRepeaterHandler::GetFrontCoordinateOffset(a_Meta))) ?
			GetPowerLevel(a_World, a_Position, a_BlockType, a_Meta) : 0
		);
	}

	virtual unsigned char GetPowerLevel(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_World);
		UNUSED(a_Position);
		UNUSED(a_Meta);
		return IsOn(a_BlockType) ? 15 : 0;
	}

	virtual cVector3iArray Update(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) const override
	{
		// LOGD("Evaluating loopy the repeater (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);
		auto Data = static_cast<cIncrementalRedstoneSimulator *>(a_World.GetRedstoneSimulator())->GetChunkData();
		auto DelayInfo = Data->GetMechanismDelayInfo(a_Position);

		if (DelayInfo == nullptr)
		{
			bool ShouldBeOn = (a_PoweringData.PowerLevel != 0);
			if (ShouldBeOn != IsOn(a_BlockType))
			{
				Data->m_MechanismDelays[a_Position] = std::make_pair((((a_Meta & 0xC) >> 0x2) + 1), ShouldBeOn);
			}
		}
		else
		{
			int DelayTicks;
			bool ShouldPowerOn;
			std::tie(DelayTicks, ShouldPowerOn) = *DelayInfo;

			if (DelayTicks == 0)
			{
				a_World.SetBlock(a_Position.x, a_Position.y, a_Position.z, ShouldPowerOn ? E_BLOCK_REDSTONE_REPEATER_ON : E_BLOCK_REDSTONE_REPEATER_OFF, a_Meta);
				Data->m_MechanismDelays.erase(a_Position);
				return cVector3iArray{ cBlockRedstoneRepeaterHandler::GetFrontCoordinateOffset(a_Meta) + a_Position };
			}
		}

		return {};
	}

	virtual cVector3iArray GetValidSourcePositions(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const override
	{
		return { cBlockRedstoneRepeaterHandler::GetRearCoordinateOffset(a_Meta) + a_Position };
	}
};
