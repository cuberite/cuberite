
#pragma once

#include "RedstoneHandler.h"
#include "Blocks/BlockDoor.h"





class cDoorHandler : public cRedstoneHandler
{
	typedef cRedstoneHandler super;
public:

	cDoorHandler(cWorld & a_World) :
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
		// LOGD("Evaluating dori the door (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		if (a_PoweringData != static_cast<cIncrementalRedstoneSimulator *>(m_World.GetRedstoneSimulator())->GetChunkData()->ExchangeUpdateOncePowerData(a_Position, a_PoweringData))
		{
			cChunkInterface ChunkInterface(m_World.GetChunkMap());
			cBlockDoorHandler::SetOpen(ChunkInterface, a_Position.x, a_Position.y, a_Position.z, (a_PoweringData.PowerLevel != 0));
			m_World.BroadcastSoundParticleEffect(EffectID::SFX_RANDOM_WOODEN_DOOR_OPEN, a_Position.x, a_Position.y, a_Position.z, 0);
		}

		return {};
	}

	virtual cVector3iArray GetValidSourcePositions(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) override
	{
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		return GetAdjustedRelatives(a_Position, GetRelativeAdjacents());;
	}
};
