#pragma once

#include "../../Chunk.h"

inline auto & DataForChunk(const cChunk & a_Chunk)
{
	return *static_cast<cIncrementalRedstoneSimulatorChunkData *>(a_Chunk.GetRedstoneSimulatorData());
}

template <typename... ArrayTypes>
inline void UpdateAdjustedRelative(const cChunk & From, const cChunk & To, const Vector3i Position, const Vector3i Offset)
{
	DataForChunk(To).WakeUp(cIncrementalRedstoneSimulatorChunkData::RebaseRelativePosition(From, To, Position + Offset));

	for (const auto & LinkedOffset : cSimulator::GetLinkedOffsets(Offset))
	{
		DataForChunk(To).WakeUp(cIncrementalRedstoneSimulatorChunkData::RebaseRelativePosition(From, To, Position + LinkedOffset));
	}
}

template <typename ArrayType>
inline void UpdateAdjustedRelatives(const cChunk & From, const cChunk & To, const Vector3i Position, const ArrayType & Relative)
{
	for (const auto & Offset : Relative)
	{
		UpdateAdjustedRelative(From, To, Position, Offset);
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

// Warning shouldn't trigger for inline variables, this is fixed in clang 7
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#endif

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

#ifdef __clang__
#pragma clang diagnostic pop
#endif
