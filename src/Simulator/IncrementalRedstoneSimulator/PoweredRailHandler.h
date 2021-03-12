
#pragma once
#include "../../Blocks/BlockRail.h"




namespace PoweredRailHandler
{

#define GETXZOFFSET(BlockType)\
{ \
	switch (BlockType::Shape(a_Block)) \
	{ \
		case BlockType::Shape::NorthSouth:     return { 0, 0, 1 }; \
		case BlockType::Shape::EastWest:       return { 1, 0, 0 }; \
		case BlockType::Shape::AscendingEast:  return { 1, 1, 0 }; \
		case BlockType::Shape::AscendingWest:  return { 1, 1, 0 }; \
		case BlockType::Shape::AscendingNorth: return { 0, 1, 1 }; \
		case BlockType::Shape::AscendingSouth: return { 0, 1, 1 }; \
	} \
}
	/** Get the offset along which the rail faces.
	Not in cBlockRailHandler since specific to powered rails. */
	static Vector3i GetPoweredRailAdjacentXZCoordinateOffset(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::ActivatorRail:
			{
				switch (ActivatorRail::Shape(a_Block))
				{
					case ActivatorRail::Shape::NorthSouth:     return { 0, 0, 1 };
					case ActivatorRail::Shape::EastWest:       return { 1, 0, 0 };
					case ActivatorRail::Shape::AscendingEast:  return { 1, 1, 0 };
					case ActivatorRail::Shape::AscendingWest:  return { 1, 1, 0 };
					case ActivatorRail::Shape::AscendingNorth: return { 0, 1, 1 };
					case ActivatorRail::Shape::AscendingSouth: return { 0, 1, 1 };
				}
				break;
			}
			case BlockType::DetectorRail:
			{
				switch (DetectorRail::Shape(a_Block))
				{
					case DetectorRail::Shape::NorthSouth:     return { 0, 0, 1 };
					case DetectorRail::Shape::EastWest:       return { 1, 0, 0 };
					case DetectorRail::Shape::AscendingEast:  return { 1, 1, 0 };
					case DetectorRail::Shape::AscendingWest:  return { 1, 1, 0 };
					case DetectorRail::Shape::AscendingNorth: return { 0, 1, 1 };
					case DetectorRail::Shape::AscendingSouth: return { 0, 1, 1 };
				}
				break;
			}
			case BlockType::PoweredRail:
			{
				switch (PoweredRail::Shape(a_Block))
				{
					case PoweredRail::Shape::NorthSouth:     return { 0, 0, 1 };
					case PoweredRail::Shape::EastWest:       return { 1, 0, 0 };
					case PoweredRail::Shape::AscendingEast:  return { 1, 1, 0 };
					case PoweredRail::Shape::AscendingWest:  return { 1, 1, 0 };
					case PoweredRail::Shape::AscendingNorth: return { 0, 1, 1 };
					case PoweredRail::Shape::AscendingSouth: return { 0, 1, 1 };
				}
				break;
			}
			default:
			{
				ASSERT(!"Impossible rail meta! wat wat wat");
				return {0, 0, 0};
			}
		}
		return {0, 0, 0};
	}

	static PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, Vector3i a_QueryPosition, BlockState a_QueryBlock, bool IsLinked)
	{
		UNUSED(a_QueryBlock);

		const auto Offset = GetPoweredRailAdjacentXZCoordinateOffset(a_Block);
		if (((Offset + a_Position) == a_QueryPosition) || ((-Offset + a_Position) == a_QueryPosition))
		{
			const auto Power = DataForChunk(a_Chunk).GetCachedPowerData(a_Position);
			return (Power <= 7) ? 0 : (Power - 1);
		}
		return 0;
	}

	static void Update(cChunk & a_Chunk, cChunk & CurrentlyTickingChunk, Vector3i a_Position, BlockState a_Block, const PowerLevel Power)
	{
		LOGREDSTONE("Evaluating tracky the rail (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		switch (a_Block.Type())
		{
			case BlockType::DetectorRail:
			{
				/*
				if ((m_Chunk->GetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ) & 0x08) == 0x08)
				{
					SetAllDirsAsPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, a_MyType);
				}
				*/
				return;
			}
			case BlockType::ActivatorRail:
			case BlockType::PoweredRail:
			{
				const auto Offset = GetPoweredRailAdjacentXZCoordinateOffset(a_Block);
				if (Power != DataForChunk(a_Chunk).ExchangeUpdateOncePowerData(a_Position, Power))
				{
					cChunkInterface ChunkInterface(a_Chunk.GetWorld()->GetChunkMap());
					cBlockRailHandler::SetPowered(ChunkInterface, a_Position, Power > 0);

					UpdateAdjustedRelative(a_Chunk, CurrentlyTickingChunk, a_Position, Offset);
					UpdateAdjustedRelative(a_Chunk, CurrentlyTickingChunk, a_Position, -Offset);
				}

				return;
			}
			default:
			{
				ASSERT(!"Unhandled type of rail in passed to rail handler!");
			}
		}
	}

	static void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);

		if ((a_Block.Type() == BlockType::PoweredRail) || (a_Block.Type() == BlockType::ActivatorRail))
		{
			InvokeForAdjustedRelatives(Callback, a_Position, RelativeAdjacents);
		}
	}
};
