
#include "Globals.h"

#include "ForEachSourceCallback.h"
#include "../../BlockInfo.h"
#include "../../Chunk.h"
#include "IncrementalRedstoneSimulator.h"
#include "RedstoneHandler.h"





ForEachSourceCallback::ForEachSourceCallback(const cChunk & Chunk, const Vector3i Position, const BLOCKTYPE CurrentBlock) :
	m_Chunk(Chunk),
	m_Position(Position),
	m_CurrentBlock(CurrentBlock)
{
}





bool ForEachSourceCallback::ShouldQueryLinkedPosition(const Vector3i Location, const BLOCKTYPE Block)
{
	switch (Block)
	{
		case E_BLOCK_BLOCK_OF_REDSTONE:
		case E_BLOCK_TRAPPED_CHEST: return false;
		default: return cBlockInfo::IsSolid(Block);
	}
}





void ForEachSourceCallback::operator()(Vector3i Location)
{
	if (!cChunk::IsValidHeight(Location.y))
	{
		return;
	}

	const auto NeighbourChunk = m_Chunk.GetRelNeighborChunkAdjustCoords(Location);
	if ((NeighbourChunk == nullptr) || !NeighbourChunk->IsValid())
	{
		return;
	}

	const auto PotentialSourceBlock = NeighbourChunk->GetBlock(Location);
	const auto NeighbourRelativeQueryPosition = cIncrementalRedstoneSimulatorChunkData::RebaseRelativePosition(m_Chunk, *NeighbourChunk, m_Position);

	if (ShouldQueryLinkedPosition(Location, PotentialSourceBlock))
	{
		Power = std::max(Power, QueryLinkedPower(*NeighbourChunk, NeighbourRelativeQueryPosition, m_CurrentBlock, Location));
	}
	else
	{
		Power = std::max(Power, QueryPower(*NeighbourChunk, Location, PotentialSourceBlock, NeighbourRelativeQueryPosition, m_CurrentBlock, false));
	}
}





PoweringData ForEachSourceCallback::QueryPower(const cChunk & Chunk, const Vector3i SourcePosition, const BLOCKTYPE SourceBlock, const Vector3i QueryPosition, const BLOCKTYPE QueryBlock, const bool IsLinked)
{
	return
	{
		SourceBlock,
		RedstoneHandler::GetPowerDeliveredToPosition(
			Chunk, SourcePosition, SourceBlock,
			QueryPosition, QueryBlock, IsLinked
		)
	};
}





PoweringData ForEachSourceCallback::QueryLinkedPower(const cChunk & Chunk, const Vector3i QueryPosition, const BLOCKTYPE QueryBlock, const Vector3i SolidBlockPosition)
{
	PoweringData Power;

	for (const auto Offset : cSimulator::GetLinkedOffsets(SolidBlockPosition - QueryPosition))
	{
		auto SourcePosition = QueryPosition + Offset;
		if (!cChunk::IsValidHeight(SourcePosition.y))
		{
			continue;
		}

		const auto NeighbourChunk = Chunk.GetRelNeighborChunkAdjustCoords(SourcePosition);
		if ((NeighbourChunk == nullptr) || !NeighbourChunk->IsValid())
		{
			continue;
		}

		const auto NeighbourRelativeSolidBlockPosition = cIncrementalRedstoneSimulatorChunkData::RebaseRelativePosition(Chunk, *NeighbourChunk, SolidBlockPosition);
		Power = std::max(Power, QueryPower(*NeighbourChunk, SourcePosition, NeighbourChunk->GetBlock(SourcePosition), NeighbourRelativeSolidBlockPosition, QueryBlock, true));
	}

	return Power;
}
