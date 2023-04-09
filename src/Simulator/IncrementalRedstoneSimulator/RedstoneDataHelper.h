#pragma once

#include "../../Chunk.h"

inline auto & DataForChunk(const cChunk & a_Chunk)
{
	return *static_cast<cIncrementalRedstoneSimulatorChunkData *>(a_Chunk.GetRedstoneSimulatorData());
}

inline void UpdateAdjustedRelative(const cChunk & a_Chunk, const cChunk & a_TickingChunk, const Vector3i a_Position, const Vector3i a_Offset)
{
	const auto PositionToWake = a_Position + a_Offset;

	if (!cChunkDef::IsValidHeight(PositionToWake))
	{
		// If an offset position is not a valid height, its linked offset positions won't be either.
		return;
	}

	auto & ChunkData = DataForChunk(a_TickingChunk);

	// Schedule the block in the requested direction to update:
	ChunkData.WakeUp(cIncrementalRedstoneSimulatorChunkData::RebaseRelativePosition(a_Chunk, a_TickingChunk, PositionToWake));

	// To follow Vanilla behaviour, update all linked positions:
	for (const auto & LinkedOffset : cSimulator::GetLinkedOffsets(a_Offset))
	{
		if (const auto LinkedPositionToWake = a_Position + LinkedOffset; cChunkDef::IsValidHeight(LinkedPositionToWake))
		{
			ChunkData.WakeUp(cIncrementalRedstoneSimulatorChunkData::RebaseRelativePosition(a_Chunk, a_TickingChunk, LinkedPositionToWake));
		}
	}
}

template <typename ArrayType>
inline void UpdateAdjustedRelatives(const cChunk & a_Chunk, const cChunk & a_TickingChunk, const Vector3i a_Position, const ArrayType & a_Relative)
{
	for (const auto & Offset : a_Relative)
	{
		UpdateAdjustedRelative(a_Chunk, a_TickingChunk, a_Position, Offset);
	}
}

template <typename ArrayType>
inline void InvokeForAdjustedRelatives(ForEachSourceCallback & Callback, const Vector3i Position, const ArrayType & Relative)
{
	for (const auto & Offset : Relative)
	{
		Callback(Position + Offset);
	}
}

inline constexpr Vector3i OffsetYP{ 0, 1, 0 };

inline constexpr Vector3i OffsetYM{ 0, -1, 0 };

inline constexpr std::array<Vector3i, 6> RelativeAdjacents
{
	{
		{ 1, 0, 0 },
		{ -1, 0, 0 },
		{ 0, 1, 0 },
		{ 0, -1, 0 },
		{ 0, 0, 1 },
		{ 0, 0, -1 },
	}
};

inline constexpr std::array<Vector3i, 4> RelativeLaterals
{
	{
		{ 1, 0, 0 },
		{ -1, 0, 0 },
		{ 0, 0, 1 },
		{ 0, 0, -1 },
	}
};
