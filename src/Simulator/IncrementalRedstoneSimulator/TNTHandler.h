
#pragma once

#include "RedstoneHandler.h"





class cTNTHandler : public cRedstoneHandler
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
		// LOGD("Evaluating explodinator the trinitrotoluene (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);
		if (a_PoweringData.PowerLevel != 0)
		{
			a_World.BroadcastSoundEffect("entity.tnt.primed", a_Position, 0.5f, 0.6f);
			a_World.SetBlock(a_Position.x, a_Position.y, a_Position.z, E_BLOCK_AIR, 0);
			a_World.SpawnPrimedTNT(a_Position + Vector3d(0.5, 0.5, 0.5));  // 80 ticks to boom
		}
		return {};
	}

	virtual cVector3iArray GetValidSourcePositions(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_World);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		return GetAdjustedRelatives(a_Position, GetRelativeAdjacents());;
	}
};
