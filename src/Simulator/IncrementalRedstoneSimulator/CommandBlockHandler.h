
#pragma once

#include "RedstoneHandler.h"
#include "BlockEntities/CommandBlockEntity.h"





class cCommandBlockHandler : public cRedstoneHandler
{
	typedef cRedstoneHandler super;
public:

	cCommandBlockHandler(cWorld & a_World) :
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
		// LOGD("Evaluating commander the cmdblck (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		auto Previous = static_cast<cIncrementalRedstoneSimulator *>(m_World.GetRedstoneSimulator())->GetChunkData()->ExchangeUpdateOncePowerData(a_Position, a_PoweringData);
		if ((Previous.PowerLevel != 0) || (a_PoweringData.PowerLevel == 0))
		{
			// If we're already powered or received an update of no power, don't activate
			return {};
		}

		class cSetPowerToCommandBlock : public cCommandBlockCallback
		{
		public:
			virtual bool Item(cCommandBlockEntity * a_CommandBlock) override
			{
				a_CommandBlock->Activate();
				return false;
			}
		} CmdBlockSP;

		m_World.DoWithCommandBlockAt(a_Position.x, a_Position.y, a_Position.z, CmdBlockSP);
		return {};
	}

	virtual cVector3iArray GetValidSourcePositions(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) override
	{
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		return GetAdjustedRelatives(a_Position, GetRelativeAdjacents());;
	}
};
