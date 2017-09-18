
#pragma once

#include "RedstoneHandler.h"
#include "BlockEntities/ChestEntity.h"





class cTrappedChestHandler : public cRedstoneHandler
{
	typedef cRedstoneHandler super;
public:

	virtual unsigned char GetPowerDeliveredToPosition(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType) const override
	{
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		UNUSED(a_QueryPosition);
		UNUSED(a_QueryBlockType);

		return static_cast<cIncrementalRedstoneSimulator *>(a_World.GetRedstoneSimulator())->GetChunkData()->GetCachedPowerData(a_Position).PowerLevel;
	}

	virtual unsigned char GetPowerLevel(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_BlockType);
		UNUSED(a_Meta);

		int NumberOfPlayers = 0;
		VERIFY(!a_World.DoWithChestAt(a_Position.x, a_Position.y, a_Position.z, [&](cChestEntity & a_Chest)
			{
				ASSERT(a_Chest.GetBlockType() == E_BLOCK_TRAPPED_CHEST);
				NumberOfPlayers = a_Chest.GetNumberOfPlayers();
				return true;
			}
		));
		return static_cast<unsigned char>(std::min(NumberOfPlayers, 15));
	}

	virtual cVector3iArray Update(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) const override
	{
		// LOGD("Evaluating tricky the trapped chest (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		auto Power = GetPowerLevel(a_World, a_Position, a_BlockType, a_Meta);
		auto PreviousPower = static_cast<cIncrementalRedstoneSimulator *>(a_World.GetRedstoneSimulator())->GetChunkData()->ExchangeUpdateOncePowerData(a_Position, PoweringData(a_BlockType, Power));

		if (Power != PreviousPower.PowerLevel)
		{
			return GetAdjustedRelatives(a_Position, StaticAppend(GetRelativeLaterals(), cVector3iArray{ OffsetYM() }));
		}

		return {};
	}

	virtual cVector3iArray GetValidSourcePositions(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_World);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);

		return {};
	}
};
