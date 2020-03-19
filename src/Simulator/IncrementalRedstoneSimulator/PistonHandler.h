
#pragma once

#include "RedstoneHandler.h"
#include "../../Blocks/BlockPiston.h"





class cPistonHandler : public cRedstoneHandler
{
	typedef cRedstoneHandler super;
public:

	virtual unsigned char GetPowerDeliveredToPosition(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType) const override
	{
		UNUSED(a_World);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		UNUSED(a_QueryPosition);
		UNUSED(a_QueryBlockType);
		return 0;
	}

	virtual unsigned char GetPowerLevel(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_World);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		return 0;
	}

	virtual cVector3iArray Update(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) const override
	{
		// LOGD("Evaluating pisty the piston (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		bool ShouldBeExtended = (a_PoweringData.PowerLevel != 0);
		if (ShouldBeExtended == cBlockPistonHandler::IsExtended(a_Meta))
		{
			return {};
		}

		if (ShouldBeExtended)
		{
			cBlockPistonHandler::ExtendPiston(a_Position, a_World);
		}
		else
		{
			cBlockPistonHandler::RetractPiston(a_Position, a_World);
		}

		// It is necessary to delay after a signal to prevent an infinite loop (#3168)
		// However, that is present as a side effect of the implementation of piston animation in Blocks\BlockPiston.cpp

		return {};
	}

	virtual cVector3iArray GetValidSourcePositions(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_World);
		UNUSED(a_BlockType);

		auto PositionsOffset = GetRelativeAdjacents();
		auto Face = cBlockPistonHandler::MetaDataToDirection(a_Meta);
		int OffsetX = 0, OffsetY = 0, OffsetZ = 0;

		AddFaceDirection(OffsetX, OffsetY, OffsetZ, Face);
		PositionsOffset.erase(std::remove(PositionsOffset.begin(), PositionsOffset.end(), Vector3i(OffsetX, OffsetY, OffsetZ)), PositionsOffset.end());

		return GetAdjustedRelatives(a_Position, PositionsOffset);
	}
};
