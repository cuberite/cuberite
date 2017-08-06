
#pragma once

#include "RedstoneHandler.h"





class cTNTHandler : public cRedstoneHandler
{
	typedef cRedstoneHandler super;
public:

	virtual unsigned char GetPowerDeliveredToPosition(cWorld & a_World, const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, const Vector3i & a_QueryPosition, BLOCKTYPE a_QueryBlockType) const override
	{
		UNUSED(a_World);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		UNUSED(a_QueryPosition);
		UNUSED(a_QueryBlockType);
		return 0;
	}

	virtual unsigned char GetPowerLevel(cWorld & a_World, const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_World);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		return 0;
	}

	virtual cVector3iArray Update(cWorld & a_World, const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) const override
	{
		// LOGD("Evaluating explodinator the trinitrotoluene (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);
		if (a_PoweringData.PowerLevel != 0)
		{
			a_World.BroadcastSoundEffect("entity.tnt.primed", static_cast<double>(a_Position.x), static_cast<double>(a_Position.y), static_cast<double>(a_Position.z), 0.5f, 0.6f);
			a_World.SetBlock(a_Position.x, a_Position.y, a_Position.z, E_BLOCK_AIR, 0);
			a_World.SpawnPrimedTNT(a_Position.x + 0.5, a_Position.y + 0.5, a_Position.z + 0.5);  // 80 ticks to boom
		}
		return {};
	}

	virtual cVector3iArray GetValidSourcePositions(cWorld & a_World, const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_World);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		return GetAdjustedRelatives(a_Position, GetRelativeAdjacents());;
	}
};
