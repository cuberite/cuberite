
#pragma once

#include "../../Chunk.h"
#include "RedstoneSimulatorChunkData.h"





class cRedstoneHandler
{
public:

	cRedstoneHandler() = default;
	DISALLOW_COPY_AND_ASSIGN(cRedstoneHandler);

	using SourceCallback = cFunctionRef<void(Vector3i)>;

	virtual unsigned char GetPowerDeliveredToPosition(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType) const = 0;
	virtual void Update(cChunk & a_Chunk, cChunk & CurrentlyTicking, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) const = 0;
	virtual void ForValidSourcePositions(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, SourceCallback Callback) const = 0;

	// Force a virtual destructor
	virtual ~cRedstoneHandler() {}

protected:

	template <class Container>
	static Container StaticAppend(const Container & a_Lhs, const Container & a_Rhs)
	{
		Container ToReturn = a_Lhs;
		std::copy(a_Rhs.begin(), a_Rhs.end(), std::back_inserter(ToReturn));
		return ToReturn;
	}

	inline static Vector3i OffsetYP{ 0, 1, 0 };

	inline static Vector3i OffsetYM{ 0, -1, 0 };

	static cVector3iArray GetAdjustedRelatives(Vector3i a_Position, cVector3iArray a_Relatives)
	{
		for (auto & Entry : a_Relatives)
		{
			Entry += a_Position;
		}
		return a_Relatives;
	}

	inline static cIncrementalRedstoneSimulatorChunkData & DataForChunk(cChunk & a_Chunk)
	{
		return *static_cast<cIncrementalRedstoneSimulatorChunkData *>(a_Chunk.GetRedstoneSimulatorData());
	}

	template <typename... ArrayTypes>
	static void UpdateAdjustedRelative(cChunk & From, cChunk & To, const Vector3i Position)
	{
		DataForChunk(To).WakeUp(cIncrementalRedstoneSimulatorChunkData::RebaseRelativePosition(From, To, Position));
	}

	template <typename ArrayType, typename... ArrayTypes>
	static void UpdateAdjustedRelatives(cChunk & From, cChunk & To, const Vector3i Position, const ArrayType & Relative, const ArrayTypes &... Relatives)
	{
		for (const auto Offset : Relative)
		{
			DataForChunk(To).GetActiveBlocks().push(cIncrementalRedstoneSimulatorChunkData::RebaseRelativePosition(From, To, Position + Offset));
		}

		UpdateAdjustedRelatives(From, To, Position, Relatives...);
	}

	template <typename ArrayType, typename... ArrayTypes>
	static void InvokeForAdjustedRelatives(SourceCallback Callback, const Vector3i Position, const ArrayType & Relative, const ArrayTypes &... Relatives)
	{
		for (const auto Offset : Relative)
		{
			Callback(Position + Offset);
		}

		InvokeForAdjustedRelatives(Callback, Position, Relatives...);
	}

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

private:

	static void UpdateAdjustedRelatives(cChunk &, cChunk &, const Vector3i)
	{
	}

	static void InvokeForAdjustedRelatives(SourceCallback, const Vector3i)
	{
	}
};
