
#pragma once

#include "RedstoneHandler.h"
#include "Blocks/BlockPiston.h"





class cPistonHandler : public cRedstoneHandler
{
	typedef cRedstoneHandler super;
public:

	cPistonHandler(cWorld & a_World) :
		super(a_World)
	{
	}

	virtual unsigned char GetPowerDeliveredToPosition(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, const Vector3i & a_QueryPosition, BLOCKTYPE a_QueryBlockType) override
	{
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		UNUSED(a_QueryPosition);
		UNUSED(a_QueryBlockType);
		return 0;
	}

	virtual unsigned char GetPowerLevel(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		return 0;
	}

	virtual cVector3iArray Update(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) override
	{
		// LOGD("Evaluating pisty the piston (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		if (a_PoweringData.PowerLevel > 0)
		{
			cBlockPistonHandler::ExtendPiston(a_Position, &m_World);
		}
		else
		{
			cBlockPistonHandler::RetractPiston(a_Position, &m_World);
		}

		return {};
	}

	virtual cVector3iArray GetValidSourcePositions(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) override
	{
		UNUSED(a_BlockType);

		auto PositionsOffset = GetRelativeAdjacents();
		auto Face = cBlockPistonHandler::MetaDataToDirection(a_Meta);
		int OffsetX = 0, OffsetY = 0, OffsetZ = 0;

		AddFaceDirection(OffsetX, OffsetY, OffsetZ, Face);
		PositionsOffset.erase(std::remove(PositionsOffset.begin(), PositionsOffset.end(), Vector3i(OffsetX, OffsetY, OffsetZ)), PositionsOffset.end());

		return GetAdjustedRelatives(a_Position, PositionsOffset);
	}
};
