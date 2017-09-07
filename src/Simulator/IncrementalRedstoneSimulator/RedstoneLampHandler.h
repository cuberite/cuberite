
#pragma once






class cRedstoneLampHandler : public cRedstoneHandler
{
public:

	inline static bool IsOn(BLOCKTYPE a_BlockType)
	{
		return (a_BlockType == E_BLOCK_REDSTONE_LAMP_ON);
	}

	virtual unsigned char GetPowerDeliveredToPosition(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType) const override
	{
		return 0;
	}

	virtual unsigned char GetPowerLevel(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const override
	{
		return 0;
	}

	virtual cVector3iArray Update(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) const override
	{
		// LOGD("Evaluating lamp (%i %i %i)", a_Position.x, a_Position.y, a_Position.z);

		if (a_PoweringData.PowerLevel > 0)
		{
			if (!IsOn(a_BlockType))
			{
				a_World.SetBlock(a_Position.x, a_Position.y, a_Position.z, E_BLOCK_REDSTONE_LAMP_ON, 0);
			}
		}
		else
		{
			if (IsOn(a_BlockType))
			{
				a_World.SetBlock(a_Position.x, a_Position.y, a_Position.z, E_BLOCK_REDSTONE_LAMP_OFF, 0);
			}
		}

		return {};
	}

	virtual cVector3iArray GetValidSourcePositions(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_World);
		UNUSED(a_Meta);
		UNUSED(a_BlockType);
		return GetAdjustedRelatives(a_Position, GetRelativeAdjacents());
	}
};
