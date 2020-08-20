
#include "Globals.h"

#include "ForEachSourceCallback.h"
#include "../../BlockInfo.h"
#include "../../Chunk.h"
#include "IncrementalRedstoneSimulator.h"
#include "RedstoneHandler.h"





ForEachSourceCallback::ForEachSourceCallback(const cChunk & Chunk, const Vector3i Position, const BLOCKTYPE CurrentBlock) :
	Power(0),
	m_Chunk(Chunk),
	m_Position(Position),
	m_CurrentBlock(CurrentBlock)
{
}





bool ForEachSourceCallback::ShouldQueryLinkedPosition(const Vector3i Location, const BLOCKTYPE Block)
{
	switch (Block)
	{
		// Normally we don't ask solid blocks for power because they don't have any (store, dirt, etc.)
		// However, these are mechanisms that are IsSolid, but still give power. Don't ignore them:
		case E_BLOCK_BLOCK_OF_REDSTONE:
		case E_BLOCK_OBSERVER:
		case E_BLOCK_TRAPPED_CHEST: return false;

		// If a mechanism asks for power from a block, redirect the query to linked positions if:
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
		Power = std::max(
			Power,
			RedstoneHandler::GetPowerDeliveredToPosition(
				*NeighbourChunk, Location, PotentialSourceBlock,
				NeighbourRelativeQueryPosition, m_CurrentBlock, false
			)
		);
	}
}





PowerLevel ForEachSourceCallback::QueryLinkedPower(const cChunk & Chunk, const Vector3i QueryPosition, const BLOCKTYPE QueryBlock, const Vector3i SolidBlockPosition)
{
	PowerLevel Power = 0;

	// Loop through all linked powerable offsets in the direction requested:
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

		// Conduit block's position, relative to NeighbourChunk.
		const auto NeighbourRelativeSolidBlockPosition = cIncrementalRedstoneSimulatorChunkData::RebaseRelativePosition(Chunk, *NeighbourChunk, SolidBlockPosition);

		// Do a standard power query, but the requester's position is actually the solid block that will conduct power:
		Power = std::max(
			Power,
			RedstoneHandler::GetPowerDeliveredToPosition(
				*NeighbourChunk, SourcePosition, NeighbourChunk->GetBlock(SourcePosition),
				NeighbourRelativeSolidBlockPosition, QueryBlock, true
			)
		);
	}

	return Power;
}
