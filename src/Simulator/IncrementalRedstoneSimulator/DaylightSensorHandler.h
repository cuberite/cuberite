
#pragma once





namespace DaylightSensorHandler
{
	inline unsigned char GetPowerLevel(const cChunk & a_Chunk, const Vector3i a_Position)
	{
		int TimeOfDay = a_Chunk.GetWorld()->GetTimeOfDay();
		int IsSunny = a_Chunk.GetWorld()->IsWeatherSunnyAt(a_Position.x, a_Position.z);
		bool IsInverted = (a_Chunk.GetBlock(a_Position) == E_BLOCK_DAYLIGHT_SENSOR) ? 0 : 1;

		// Find which is lower - the predicted daylight or the actual daylight
		// Actual daylight is too high for power level at night, but predicted
		// daylight is too high when sensor is covered
		NIBBLETYPE Output = std::min(
			a_Chunk.GetSkyLightAltered(a_Position),
			static_cast<NIBBLETYPE>(Clamp(FloorC(14 * std::sin((TimeOfDay * M_PI) / 12000) + 8), 0, 15))
		);

		if (IsInverted)
		{
			Output = 15 - Output;
		}

		if (!IsSunny)
		{
			std::max(Output - 3, 0);
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
		return IsLinked ? 0 : a_Chunk.GetMeta(a_Position);
	}

	inline void Update(cChunk & a_Chunk, cChunk & CurrentlyTicking, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, const PowerLevel Power)
	{
		// LOGD("Evaluating Darryl the daylight sensor (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		auto & ChunkData = DataForChunk(a_Chunk);
		const auto PowerLevel = GetPowerLevel(a_Chunk, a_Position);

		// We don't need to update the daylight sensor often
		ChunkData.m_MechanismDelays[a_Position] = std::make_pair(5, true);

		// Only update the power level if the power level has changed - it has gone
		// up a 'step' on the time-power function
		// a_Meta stores current power level
		if (PowerLevel != a_Meta)
		{
			a_Chunk.SetMeta(a_Position, PowerLevel);
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
