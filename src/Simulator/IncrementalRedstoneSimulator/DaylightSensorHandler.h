
#pragma once





namespace DaylightSensorHandler
{
	inline unsigned char GetPowerLevel(const cChunk & Chunk, const Vector3i Position)
	{
		int TimeOfDay = Chunk.GetWorld()->GetTimeOfDay();
		bool IsInverted = (Chunk.GetBlock(Position) == E_BLOCK_DAYLIGHT_SENSOR) ? 0 : 1;
		int Output = Clamp(FloorC(14 * std::sin((TimeOfDay * M_PI) / 12000) + 8), 0, 15);

		if (IsInverted)
		{
			Output = 15 - Output;
		}

		return static_cast<unsigned char>(Output);
	}

	inline PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType, bool IsLinked)
	{
		UNUSED(a_Chunk);
		UNUSED(a_BlockType);
		UNUSED(a_QueryPosition);

		// Daylight sensors output to all surrounding blocks
		// Retrieve and return the cached power calculated by Update for performance
		return IsLinked ? 0 : DataForChunk(a_Chunk).GetCachedPowerData(a_Position);
	}

	inline void Update(cChunk & a_Chunk, cChunk & CurrentlyTicking, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, const PowerLevel Power)
	{
		auto & ChunkData = DataForChunk(a_Chunk);

		const auto PreviousPower = ChunkData.GetCachedPowerData(a_Position);
		const auto PowerLevel = GetPowerLevel(a_Chunk, a_Position);

		// We don't need to update the daylight sensor often
		ChunkData.m_MechanismDelays[a_Position] = std::make_pair(10, true);

		// Only update the power level if the power level has changed - it has gone
		// up a 'step' on the time-power function
		if (PowerLevel != PreviousPower)
		{
			ChunkData.SetCachedPowerData(a_Position, PowerLevel);
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
