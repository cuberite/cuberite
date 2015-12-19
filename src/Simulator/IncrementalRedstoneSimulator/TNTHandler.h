
#pragma once

#include "RedstoneHandler.h"
#include "Blocks/BlockButton.h"
#include "Blocks/BlockLever.h"





class cTNTHandler : public cRedstoneHandler
{
	typedef cRedstoneHandler super;
public:

	cTNTHandler(cWorld & a_World) :
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
		LOGD("Evaluating explodinator the trinitrotoluene (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);
		if (a_PoweringData.PowerLevel != 0)
		{
			m_World.BroadcastSoundEffect("game.tnt.primed", static_cast<double>(a_Position.x), static_cast<double>(a_Position.y), static_cast<double>(a_Position.z), 0.5f, 0.6f);
			m_World.SetBlock(a_Position.x, a_Position.y, a_Position.z, E_BLOCK_AIR, 0);
			m_World.SpawnPrimedTNT(a_Position.x + 0.5, a_Position.y + 0.5, a_Position.z + 0.5);  // 80 ticks to boom
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
