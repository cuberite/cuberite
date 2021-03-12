
#pragma once

#include "World.h"
#include "../../Blocks/BlockDaylightSensor.h"





namespace DaylightSensorHandler
{
	static PowerLevel GetPowerLevel(const cChunk & a_Chunk, const Vector3i a_Position)
	{
		if (Block::DaylightDetector::Inverted(a_Chunk.GetBlock(a_Position)))
		{
			// Inverted sensor directly returns darkened skylight, no fancy tricks:
			return 15 - a_Chunk.GetSkyLightAltered(a_Position);
		}

		// The [0, 1) proportion of the current day that has elapsed.
		const auto ProportionOfDay = a_Chunk.GetWorld()->GetTimeOfDay().count() * (static_cast<float>(M_PI) / 12000.f);

		// The curved value of darkened skylight, with outputs somewhat similar to Vanilla.
		const auto RawOutput = a_Chunk.GetSkyLightAltered(a_Position) * (0.6f * std::sin(ProportionOfDay) + 0.5f);

		// Saturate the amplified sine curve at 0 and 15:
		return static_cast<PowerLevel>(std::clamp(RawOutput, 0.f, 15.f));
	}

	static PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, Vector3i a_QueryPosition, BlockState a_QueryBlock, bool IsLinked)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_QueryPosition);
		UNUSED(a_QueryBlock);

		// Daylight sensors only output to immediately surrounding blocks:
		return IsLinked ? 0 : cBlockDaylightSensorHandler::GetPowerLevel(a_Block);
	}

	static void Update(cChunk & a_Chunk, cChunk & CurrentlyTicking, Vector3i a_Position, BlockState a_Block, const PowerLevel Power)
	{
		LOGREDSTONE("Evaluating Darryl the daylight sensor (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		// What the sensor should output according to the time-power function.
		const auto PowerLevel = GetPowerLevel(a_Chunk, a_Position);

		// Only update the output if the power level has changed:
		if (PowerLevel != cBlockDaylightSensorHandler::GetPowerLevel(a_Block))
		{
			a_Chunk.SetBlock(a_Position, Block::DaylightDetector::DaylightDetector(Block::DaylightDetector::Inverted(a_Block), PowerLevel));
			UpdateAdjustedRelatives(a_Chunk, CurrentlyTicking, a_Position, RelativeAdjacents);
		}
	}

	static void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_Block);
		UNUSED(Callback);
	}
};
