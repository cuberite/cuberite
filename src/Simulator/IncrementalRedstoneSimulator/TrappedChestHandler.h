
#pragma once

#include "../../BlockEntities/ChestEntity.h"





namespace TrappedChestHandler
{
	inline unsigned char GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType, bool IsLinked)
	{
		UNUSED(a_BlockType);
		UNUSED(a_QueryPosition);
		UNUSED(a_QueryBlockType);
		UNUSED(IsLinked);

		return DataForChunk(a_Chunk).GetCachedPowerData(a_Position).PowerLevel;
	}

	inline unsigned char GetPowerLevel(cChunk & a_Chunk, Vector3i a_Position)
	{
		int NumberOfPlayers = 0;
		VERIFY(
			!a_Chunk.DoWithChestAt(a_Position, [&](cChestEntity & a_Chest)
			{
				ASSERT(a_Chest.GetBlockType() == E_BLOCK_TRAPPED_CHEST);
				NumberOfPlayers = a_Chest.GetNumberOfPlayers();
				return true;
			})
		);

		return static_cast<unsigned char>(std::min(NumberOfPlayers, 15));
	}

	inline void Update(cChunk & a_Chunk, cChunk & CurrentlyTicking, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData)
	{
		// LOGD("Evaluating tricky the trapped chest (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		const auto Power = GetPowerLevel(a_Chunk, a_Position);
		const auto PreviousPower = DataForChunk(a_Chunk).ExchangeUpdateOncePowerData(a_Position, PoweringData(a_BlockType, Power));

		if (Power != PreviousPower.PowerLevel)
		{
			UpdateAdjustedRelatives(a_Chunk, CurrentlyTicking, a_Position, RelativeAdjacents);
		}
	}

	inline void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		UNUSED(Callback);
	}
};
