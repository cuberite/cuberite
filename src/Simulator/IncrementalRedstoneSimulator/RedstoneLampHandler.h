
#pragma once

#include "IncrementalRedstoneSimulator.h"





class cRedstoneLampHandler : public cRedstoneHandler
{
public:

	cRedstoneLampHandler(cWorld & a_World) :
		cRedstoneHandler(a_World)
	{
	}

	inline static bool IsOn(BLOCKTYPE a_BlockType)
	{
		return (a_BlockType == E_BLOCK_REDSTONE_LAMP_ON);
	}

	virtual unsigned char GetPowerDeliveredToPosition(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, const Vector3i & a_QueryPosition, BLOCKTYPE a_QueryBlockType) override
	{
		return 0;
	}

	virtual unsigned char GetPowerLevel(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) override
	{
		return 0;
	}

	virtual cVector3iArray Update(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) override
	{
		// LOGD("Evaluating lamp (%i %i %i)", a_Position.x, a_Position.y, a_Position.z);

		if (a_PoweringData.PowerLevel > 0)
		{
			if (!IsOn(a_BlockType))
			{
				m_World.SetBlock(a_Position.x, a_Position.y, a_Position.z, E_BLOCK_REDSTONE_LAMP_ON, 0);
			}
		}
		else
		{
			if (IsOn(a_BlockType))
			{
				m_World.SetBlock(a_Position.x, a_Position.y, a_Position.z, E_BLOCK_REDSTONE_LAMP_OFF, 0);
			}
		}

		return {};
	}

	virtual cVector3iArray GetValidSourcePositions(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		UNUSED(a_BlockType);
		return GetAdjustedRelatives(a_Position, GetRelativeAdjacents());
	}
};
