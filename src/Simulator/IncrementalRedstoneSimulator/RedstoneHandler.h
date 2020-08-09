
#pragma once

#include "../../Chunk.h"
#include "ForEachSourceCallback.h"
#include "RedstoneSimulatorChunkData.h"





class cRedstoneHandler
{
public:

	cRedstoneHandler() = default;
	DISALLOW_COPY_AND_ASSIGN(cRedstoneHandler);

	using SourceCallback = ForEachSourceCallback &;

	virtual unsigned char GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType, bool IsLinked) const = 0;
	virtual void Update(cChunk & a_Chunk, cChunk & CurrentlyTicking, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) const = 0;
	virtual void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, SourceCallback Callback) const = 0;

	// Force a virtual destructor
	virtual ~cRedstoneHandler() {}

protected:

	inline static auto & DataForChunk(const cChunk & a_Chunk)
	{
		return *static_cast<cIncrementalRedstoneSimulatorChunkData *>(a_Chunk.GetRedstoneSimulatorData());
	}

	template <typename... ArrayTypes>
	static void UpdateAdjustedRelative(const cChunk & From, const cChunk & To, const Vector3i Position, const Vector3i Offset)
	{
		DataForChunk(To).WakeUp(cIncrementalRedstoneSimulatorChunkData::RebaseRelativePosition(From, To, Position + Offset));

		for (const auto LinkedOffset : cSimulator::GetLinkedOffsets(Offset))
		{
			DataForChunk(To).WakeUp(cIncrementalRedstoneSimulatorChunkData::RebaseRelativePosition(From, To, Position + LinkedOffset));
		}
	}

	template <typename ArrayType>
	static void UpdateAdjustedRelatives(const cChunk & From, const cChunk & To, const Vector3i Position, const ArrayType & Relative)
	{
		for (const auto Offset : Relative)
		{
			UpdateAdjustedRelative(From, To, Position, Offset);
		}
	}

	template <typename ArrayType>
	static void InvokeForAdjustedRelatives(SourceCallback Callback, const Vector3i Position, const ArrayType & Relative)
	{
		for (const auto Offset : Relative)
		{
			Callback(Position + Offset);
		}
	}

	inline static Vector3i OffsetYP{ 0, 1, 0 };

	inline static Vector3i OffsetYM{ 0, -1, 0 };

	inline static std::array<Vector3i, 6> RelativeAdjacents
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

	inline static std::array<Vector3i, 4> RelativeLaterals
	{
		{
			{ 1, 0, 0 },
			{ -1, 0, 0 },
			{ 0, 0, 1 },
			{ 0, 0, -1 },
		}
	};
};
