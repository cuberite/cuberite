#pragma once

#include "BlockHandler.h"
#include "BlockSlab.h"
#include "BlockStairs.h"
#include "BlockType.h"
#include "../BlockInfo.h"
#include "../Chunk.h"
#include "ChunkDef.h"
#include "../Entities/Player.h"




enum ENUM_PURE
{
	E_PURE_UPDOWN = 0,
	E_PURE_DOWN = 1,
	E_PURE_NONE = 2,
	E_PURE_UNSET
};





class cBlockRailHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	enum class Shape
	{
		NorthSouth,
		EastWest,
		AscendingEast,
		AscendingWest,
		AscendingNorth,
		AscendingSouth,
		SouthEast,
		SouthWest,
		NorthWest,
		NorthEast,
		None
	};

	static inline bool IsBlockRail(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::Rail:
			case BlockType::ActivatorRail:
			case BlockType::DetectorRail:
			case BlockType::PoweredRail:
				return true;
			default: return false;
		}
	}

	static inline void SetPowered(cChunkInterface & a_ChunkInterface, Vector3i a_Position, bool a_Powered)
	{
		using namespace Block;
		auto Self = a_ChunkInterface.GetBlock(a_Position);
		switch (Self.Type())
		{
			case BlockType::Rail: break;   // Nothing to do
			case BlockType::ActivatorRail: a_ChunkInterface.FastSetBlock(a_Position, ActivatorRail::ActivatorRail(a_Powered, ActivatorRail::Shape(Self), ActivatorRail::Waterlogged(Self))); break;
			case BlockType::DetectorRail:  a_ChunkInterface.FastSetBlock(a_Position, DetectorRail::DetectorRail(a_Powered, DetectorRail::Shape(Self), DetectorRail::Waterlogged(Self))); break;
			case BlockType::PoweredRail:   a_ChunkInterface.FastSetBlock(a_Position, PoweredRail::PoweredRail(a_Powered, PoweredRail::Shape(Self), PoweredRail::Waterlogged(Self))); break;
			default: break;
		}
	}

	static inline enum Shape GetShapeFromRail(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::Rail:
			{
				switch (Rail::Shape(a_Block))
				{
					case Rail::Shape::NorthSouth:     return Shape::NorthSouth;
					case Rail::Shape::EastWest:       return Shape::EastWest;
					case Rail::Shape::AscendingEast:  return Shape::AscendingEast;
					case Rail::Shape::AscendingWest:  return Shape::AscendingWest;
					case Rail::Shape::AscendingNorth: return Shape::AscendingNorth;
					case Rail::Shape::AscendingSouth: return Shape::AscendingSouth;
					case Rail::Shape::SouthEast:      return Shape::SouthEast;
					case Rail::Shape::SouthWest:      return Shape::SouthWest;
					case Rail::Shape::NorthWest:      return Shape::NorthWest;
					case Rail::Shape::NorthEast:      return Shape::NorthEast;
				}
				break;
			}
			case BlockType::ActivatorRail:
			{
				switch (ActivatorRail::Shape(a_Block))
				{
					case ActivatorRail::Shape::NorthSouth:     return Shape::NorthSouth;
					case ActivatorRail::Shape::EastWest:       return Shape::EastWest;
					case ActivatorRail::Shape::AscendingEast:  return Shape::AscendingEast;
					case ActivatorRail::Shape::AscendingWest:  return Shape::AscendingWest;
					case ActivatorRail::Shape::AscendingNorth: return Shape::AscendingNorth;
					case ActivatorRail::Shape::AscendingSouth: return Shape::AscendingSouth;
				}
				break;
			}
			case BlockType::DetectorRail:
			{
				switch (DetectorRail::Shape(a_Block))
				{
					case DetectorRail::Shape::NorthSouth:     return Shape::NorthSouth;
					case DetectorRail::Shape::EastWest:       return Shape::EastWest;
					case DetectorRail::Shape::AscendingEast:  return Shape::AscendingEast;
					case DetectorRail::Shape::AscendingWest:  return Shape::AscendingWest;
					case DetectorRail::Shape::AscendingNorth: return Shape::AscendingNorth;
					case DetectorRail::Shape::AscendingSouth: return Shape::AscendingSouth;
				}
				break;
			}
			case BlockType::PoweredRail:
			{
				switch (PoweredRail::Shape(a_Block))
				{
					case PoweredRail::Shape::NorthSouth:     return Shape::NorthSouth;
					case PoweredRail::Shape::EastWest:       return Shape::EastWest;
					case PoweredRail::Shape::AscendingEast:  return Shape::AscendingEast;
					case PoweredRail::Shape::AscendingWest:  return Shape::AscendingWest;
					case PoweredRail::Shape::AscendingNorth: return Shape::AscendingNorth;
					case PoweredRail::Shape::AscendingSouth: return Shape::AscendingSouth;
				}
				break;
			}
			default: return Shape::None;
		}
		return Shape::None;
	}

	static inline BlockState GetRailFromShape(BlockState a_Block, Shape a_Shape)
	{
		using namespace Block;

		switch (a_Block.Type())
		{
			case BlockType::Rail:
			{
				switch (a_Shape)
				{
					case Shape::NorthSouth:      return Rail::Rail(Rail::Shape::NorthSouth, Rail::Waterlogged(a_Block));
					case Shape::EastWest:        return Rail::Rail(Rail::Shape::EastWest, Rail::Waterlogged(a_Block));
					case Shape::AscendingEast:   return Rail::Rail(Rail::Shape::AscendingEast, Rail::Waterlogged(a_Block));
					case Shape::AscendingWest:   return Rail::Rail(Rail::Shape::AscendingWest, Rail::Waterlogged(a_Block));
					case Shape::AscendingNorth:  return Rail::Rail(Rail::Shape::AscendingNorth, Rail::Waterlogged(a_Block));
					case Shape::AscendingSouth:  return Rail::Rail(Rail::Shape::AscendingSouth, Rail::Waterlogged(a_Block));
					case Shape::SouthEast:       return Rail::Rail(Rail::Shape::SouthEast, Rail::Waterlogged(a_Block));
					case Shape::SouthWest:       return Rail::Rail(Rail::Shape::SouthWest, Rail::Waterlogged(a_Block));
					case Shape::NorthWest:       return Rail::Rail(Rail::Shape::NorthWest, Rail::Waterlogged(a_Block));
					case Shape::NorthEast:       return Rail::Rail(Rail::Shape::NorthEast, Rail::Waterlogged(a_Block));
					case Shape::None:            return a_Block;
				}
				break;
			}
			case BlockType::ActivatorRail:
			{
				switch (a_Shape)
				{
					case Shape::NorthSouth:      return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::NorthSouth, ActivatorRail::Waterlogged(a_Block));
					case Shape::EastWest:        return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::EastWest, ActivatorRail::Waterlogged(a_Block));
					case Shape::AscendingEast:   return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingEast, ActivatorRail::Waterlogged(a_Block));
					case Shape::AscendingWest:   return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingWest, ActivatorRail::Waterlogged(a_Block));
					case Shape::AscendingNorth:  return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingNorth, ActivatorRail::Waterlogged(a_Block));
					case Shape::AscendingSouth:  return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingSouth, ActivatorRail::Waterlogged(a_Block));
					case Shape::None:            return a_Block;
					case Shape::SouthEast:
					case Shape::SouthWest:
					case Shape::NorthWest:
					case Shape::NorthEast:
					{
						UNREACHABLE("Got unknown shape in cBlockRailHandler!");
						return a_Block;
					}
				}
				break;
			}
			case BlockType::DetectorRail:
			{
				switch (a_Shape)
				{
					case Shape::NorthSouth:      return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::NorthSouth, DetectorRail::Waterlogged(a_Block));
					case Shape::EastWest:        return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::EastWest, DetectorRail::Waterlogged(a_Block));
					case Shape::AscendingEast:   return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingEast, DetectorRail::Waterlogged(a_Block));
					case Shape::AscendingWest:   return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingWest, DetectorRail::Waterlogged(a_Block));
					case Shape::AscendingNorth:  return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingNorth, DetectorRail::Waterlogged(a_Block));
					case Shape::AscendingSouth:  return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingSouth, DetectorRail::Waterlogged(a_Block));
					case Shape::None:            return a_Block;
					case Shape::SouthEast:
					case Shape::SouthWest:
					case Shape::NorthWest:
					case Shape::NorthEast:
					{
						UNREACHABLE("Got unknown shape in cBlockRailHandler!");
						return a_Block;
					}
				}
				break;
			}
			case BlockType::PoweredRail:
			{
				switch (a_Shape)
				{
					case Shape::NorthSouth:      return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::NorthSouth, PoweredRail::Waterlogged(a_Block));
					case Shape::EastWest:        return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::EastWest, PoweredRail::Waterlogged(a_Block));
					case Shape::AscendingEast:   return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingEast, PoweredRail::Waterlogged(a_Block));
					case Shape::AscendingWest:   return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingWest, PoweredRail::Waterlogged(a_Block));
					case Shape::AscendingNorth:  return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingNorth, PoweredRail::Waterlogged(a_Block));
					case Shape::AscendingSouth:  return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingSouth, PoweredRail::Waterlogged(a_Block));
					case Shape::None:            return a_Block;
					case Shape::SouthEast:
					case Shape::SouthWest:
					case Shape::NorthWest:
					case Shape::NorthEast:
					{
						UNREACHABLE("Got unknown shape in cBlockRailHandler!");
						return a_Block;
					}
				}
				break;
			}
			default: UNREACHABLE("Got unknown block type in cBlockRailHandler!");
		}
		return a_Block;
	}





	BlockState static GetRailFromRotation(BlockState a_RailType, double a_Yaw)
	{
		switch (RotationToBlockFace(a_Yaw))
		{
			case BLOCK_FACE_WEST:
			case BLOCK_FACE_EAST:
			{
				return GetRailFromShape(a_RailType, Shape::EastWest);
			}
			case BLOCK_FACE_NORTH:
			case BLOCK_FACE_SOUTH:
			{
				return GetRailFromShape(a_RailType, Shape::NorthSouth);
			}
			case BLOCK_FACE_YM:
			case BLOCK_FACE_YP:
			case BLOCK_FACE_NONE:
			{
				UNREACHABLE("Got unknown rotation!");
			}
		}
		UNREACHABLE("Got unknown rotation!");
		return Block::Air::Air();
	}





	BlockState static FindBlock(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos, BlockState a_OldBlock)
	{
		Shape NewShape = Shape::None;

		char RailsCnt = 0;
		bool Neighbors[8];  // 0 - EAST, 1 - WEST, 2 - NORTH, 3 - SOUTH, 4 - EAST UP, 5 - WEST UP, 6 - NORTH UP, 7 - SOUTH UP
		memset(Neighbors, 0, sizeof(Neighbors));
		Neighbors[0] = (IsUnstable(a_ChunkInterface, a_BlockPos + Vector3i( 1, 0,  0)) || !IsNotConnected(a_ChunkInterface, a_BlockPos, BLOCK_FACE_EAST, E_PURE_DOWN));
		Neighbors[1] = (IsUnstable(a_ChunkInterface, a_BlockPos + Vector3i(-1, 0,  0)) || !IsNotConnected(a_ChunkInterface, a_BlockPos, BLOCK_FACE_WEST, E_PURE_DOWN));
		Neighbors[2] = (IsUnstable(a_ChunkInterface, a_BlockPos + Vector3i( 0, 0, -1)) || !IsNotConnected(a_ChunkInterface, a_BlockPos, BLOCK_FACE_NORTH, E_PURE_DOWN));
		Neighbors[3] = (IsUnstable(a_ChunkInterface, a_BlockPos + Vector3i( 0, 0,  1)) || !IsNotConnected(a_ChunkInterface, a_BlockPos, BLOCK_FACE_SOUTH, E_PURE_DOWN));
		Neighbors[4] = (IsUnstable(a_ChunkInterface, a_BlockPos + Vector3i( 1, 1,  0)) || !IsNotConnected(a_ChunkInterface, a_BlockPos + Vector3i(0, 1, 0), BLOCK_FACE_EAST, E_PURE_NONE));
		Neighbors[5] = (IsUnstable(a_ChunkInterface, a_BlockPos + Vector3i(-1, 1,  0)) || !IsNotConnected(a_ChunkInterface, a_BlockPos + Vector3i(0, 1, 0), BLOCK_FACE_WEST, E_PURE_NONE));
		Neighbors[6] = (IsUnstable(a_ChunkInterface, a_BlockPos + Vector3i( 0, 1, -1)) || !IsNotConnected(a_ChunkInterface, a_BlockPos + Vector3i(0, 1, 0), BLOCK_FACE_NORTH, E_PURE_NONE));
		Neighbors[7] = (IsUnstable(a_ChunkInterface, a_BlockPos + Vector3i( 0, 1,  1)) || !IsNotConnected(a_ChunkInterface, a_BlockPos + Vector3i(0, 1, 0), BLOCK_FACE_SOUTH, E_PURE_NONE));

		if (IsUnstable(a_ChunkInterface, a_BlockPos + Vector3i(1, -1, 0)) || !IsNotConnected(a_ChunkInterface, a_BlockPos - Vector3i(0, 1, 0), BLOCK_FACE_EAST))
		{
			Neighbors[0] = true;
		}
		if (IsUnstable(a_ChunkInterface, a_BlockPos - Vector3i(1, 1, 0)) || !IsNotConnected(a_ChunkInterface, a_BlockPos - Vector3i(0, 1, 0), BLOCK_FACE_WEST))
		{
			Neighbors[1] = true;
		}
		if (IsUnstable(a_ChunkInterface, a_BlockPos - Vector3i(0, 1, 1)) || !IsNotConnected(a_ChunkInterface, a_BlockPos - Vector3i(0, 1, 0), BLOCK_FACE_NORTH))
		{
			Neighbors[2] = true;
		}
		if (IsUnstable(a_ChunkInterface, a_BlockPos + Vector3i(0, -1, 1)) || !IsNotConnected(a_ChunkInterface, a_BlockPos - Vector3i(0, 1, 0), BLOCK_FACE_SOUTH))
		{
			Neighbors[3] = true;
		}

		for (int i = 0; i < 8; i++)
		{
			if (Neighbors[i])
			{
				RailsCnt++;
			}
		}

		if (RailsCnt == 1)
		{
			if (Neighbors[7])
			{
				NewShape = Shape::AscendingSouth;
			}
			else if (Neighbors[6])
			{
				NewShape = Shape::AscendingNorth;
			}
			else if (Neighbors[5])
			{
				NewShape = Shape::AscendingWest;
			}
			else if (Neighbors[4])
			{
				NewShape = Shape::AscendingEast;
			}
			else if (Neighbors[0] || Neighbors[1])
			{
				NewShape = Shape::EastWest;
			}
			else if (Neighbors[2] || Neighbors[3])
			{
				NewShape = Shape::NorthSouth;
			}
			else
			{
				ASSERT(!"Weird neighbor count");
			}
		}

		for (int i = 0; i < 4; i++)
		{
			if (Neighbors[i + 4])
			{
				Neighbors[i] = true;
			}
		}

		if (RailsCnt > 1)
		{
			bool CanCurve = CanThisRailCurve(a_OldBlock);

			if (Neighbors[3] && Neighbors[0] && CanCurve)
			{
				NewShape = Shape::SouthEast;
			}
			else if (Neighbors[3] && Neighbors[1] && CanCurve)
			{
				NewShape = Shape::SouthWest;
			}
			else if (Neighbors[2] && Neighbors[0] && CanCurve)
			{
				NewShape = Shape::NorthEast;
			}
			else if (Neighbors[2] && Neighbors[1] && CanCurve)
			{
				NewShape = Shape::NorthWest;
			}
			else if (Neighbors[7] && Neighbors[2])
			{
				NewShape = Shape::AscendingSouth;
			}
			else if (Neighbors[3] && Neighbors[6])
			{
				NewShape = Shape::AscendingNorth;
			}
			else if (Neighbors[5] && Neighbors[0])
			{
				NewShape = Shape::AscendingWest;
			}
			else if (Neighbors[4] && Neighbors[1])
			{
				NewShape = Shape::AscendingEast;
			}
			else if (Neighbors[0] && Neighbors[1])
			{
				NewShape = Shape::EastWest;
			}
			else if (Neighbors[2] && Neighbors[3])
			{
				NewShape = Shape::NorthSouth;
			}
			else if (CanCurve)
			{
				FLOG("{}", a_BlockPos);
				ASSERT(!"Weird neighbor count");
			}
		}
		return GetRailFromShape(a_OldBlock, NewShape);
	}

private:

	static bool CanBeSupportedBy(BlockState a_Block)
	{
		if (cBlockSlabHandler::IsAnySlabType(a_Block))
		{
			return cBlockSlabHandler::IsSlabUpsideDown(a_Block);
		}
		else if (cBlockStairsHandler::IsAnyStairType(a_Block))
		{
			return cBlockStairsHandler::IsStairsUpsideDown(a_Block);
		}
		return cBlockInfo::FullyOccupiesVoxel(a_Block);
	}

	virtual void OnPlaced(
			cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
			Vector3i a_BlockPos,
			BlockState a_Block
	) const override
	{
		Super::OnPlaced(a_ChunkInterface, a_WorldInterface, a_BlockPos, a_Block);

		// Alert diagonal rails:
		NeighborChanged(a_ChunkInterface, a_BlockPos + Vector3i( 1,  1,  0), BLOCK_FACE_NONE);
		NeighborChanged(a_ChunkInterface, a_BlockPos + Vector3i(-1,  1,  0), BLOCK_FACE_NONE);
		NeighborChanged(a_ChunkInterface, a_BlockPos + Vector3i( 0, +1,  1), BLOCK_FACE_NONE);
		NeighborChanged(a_ChunkInterface, a_BlockPos + Vector3i( 0, +1, -1), BLOCK_FACE_NONE);
		NeighborChanged(a_ChunkInterface, a_BlockPos + Vector3i( 1, -1,  0), BLOCK_FACE_NONE);
		NeighborChanged(a_ChunkInterface, a_BlockPos + Vector3i(-1, -1,  0), BLOCK_FACE_NONE);
		NeighborChanged(a_ChunkInterface, a_BlockPos + Vector3i( 0, -1,  1), BLOCK_FACE_NONE);
		NeighborChanged(a_ChunkInterface, a_BlockPos + Vector3i( 0, -1, -1), BLOCK_FACE_NONE);
	}





	virtual void OnBroken(
			cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
			Vector3i a_BlockPos,
			BlockState a_OldBlock,
			const cEntity * a_Digger
	) const override
	{
		Super::OnBroken(a_ChunkInterface, a_WorldInterface, a_BlockPos, a_OldBlock, a_Digger);

		// Alert diagonal rails:
		NeighborChanged(a_ChunkInterface, a_BlockPos + Vector3i( 1,  1,  0), BLOCK_FACE_NONE);
		NeighborChanged(a_ChunkInterface, a_BlockPos + Vector3i(-1,  1,  0), BLOCK_FACE_NONE);
		NeighborChanged(a_ChunkInterface, a_BlockPos + Vector3i( 0, +1,  1), BLOCK_FACE_NONE);
		NeighborChanged(a_ChunkInterface, a_BlockPos + Vector3i( 0, +1, -1), BLOCK_FACE_NONE);
		NeighborChanged(a_ChunkInterface, a_BlockPos + Vector3i( 1, -1,  0), BLOCK_FACE_NONE);
		NeighborChanged(a_ChunkInterface, a_BlockPos + Vector3i(-1, -1,  0), BLOCK_FACE_NONE);
		NeighborChanged(a_ChunkInterface, a_BlockPos + Vector3i( 0, -1,  1), BLOCK_FACE_NONE);
		NeighborChanged(a_ChunkInterface, a_BlockPos + Vector3i( 0, -1, -1), BLOCK_FACE_NONE);
	}



#define GETOFFSET(RailType) \
{ \
	switch (Rail::Shape(Self)) \
	{ \
		case Rail::Shape::AscendingEast:  Offset = { 1, 0,  0}; break; \
		case Rail::Shape::AscendingWest:  Offset = {-1, 0,  0}; break; \
		case Rail::Shape::AscendingNorth: Offset = { 0, 0, -1}; break; \
		case Rail::Shape::AscendingSouth: Offset = { 0, 0,  1}; break; \
		default: break; \
	} \
	break; \
}

	virtual bool CanBeAt(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Self) const override
	{
		BlockState BelowBlock = a_Chunk.GetBlock(a_Position.addedY(-1));

		if ((a_Position.y <= 0) || !CanBeSupportedBy(BelowBlock))
		{
			return false;
		}

		auto Shape = GetShapeFromRail(a_Self);

		switch (Shape)
		{
			case Shape::AscendingEast:
			case Shape::AscendingWest:
			case Shape::AscendingNorth:
			case Shape::AscendingSouth:
			{
				// Mapping between the meta and the neighbors that need checking
				size_t Index = static_cast<size_t>(Shape) - static_cast<size_t>(Shape::AscendingEast);  // Base index at zero
				static const std::array<Vector3i, 4> Coords =
				{
					Vector3i( 1, 0,  0),  // east,  XP
					Vector3i(-1, 0,  0),  // west,  XM
					Vector3i( 0, 0, -1),  // north, ZM
					Vector3i( 0, 0,  1),  // south, ZP
				} ;
				BlockState Block;
				if (!a_Chunk.UnboundedRelGetBlock(a_Position + Coords[Index], Block))
				{
					// Too close to the edge, cannot simulate
					return true;
				}
				return cBlockInfo::FullyOccupiesVoxel(Block);
			}
		}
		return true;
	}

	static bool CanThisRailCurve(BlockState a_Block)
	{
		return a_Block.Type() == BlockType::Rail;
	}

#define ISUNSTABLE(RailType) \
	case RailType::Shape::NorthSouth: \
	{ \
		FirstFaceToCheck = BLOCK_FACE_NORTH; \
		SecondFaceToCheck = BLOCK_FACE_SOUTH; \
		FirstStrategy = E_PURE_DOWN; \
		SecondStrategy = E_PURE_DOWN; \
		break; \
	} \
	case RailType::Shape::EastWest: \
	{ \
		FirstFaceToCheck = BLOCK_FACE_EAST; \
		SecondFaceToCheck = BLOCK_FACE_WEST; \
		FirstStrategy = E_PURE_DOWN; \
		SecondStrategy = E_PURE_DOWN; \
		break; \
	} \
	case RailType::Shape::AscendingEast: \
	{ \
		FirstPos + Vector3i(0, 1, 0); \
		FirstFaceToCheck = BLOCK_FACE_EAST; \
		SecondFaceToCheck = BLOCK_FACE_WEST; \
		FirstStrategy = E_PURE_UPDOWN; \
		SecondStrategy = E_PURE_UPDOWN; \
		break; \
	} \
	case RailType::Shape::AscendingWest: \
	{ \
		SecondPos + Vector3i(0, 1, 0); \
		FirstFaceToCheck = BLOCK_FACE_EAST; \
		SecondFaceToCheck = BLOCK_FACE_WEST; \
		FirstStrategy = E_PURE_UPDOWN; \
		SecondStrategy = E_PURE_UPDOWN; \
		break; \
	} \
	case RailType::Shape::AscendingNorth: \
	{ \
		FirstPos + Vector3i(0, 1, 0); \
		FirstFaceToCheck = BLOCK_FACE_NORTH; \
		SecondFaceToCheck = BLOCK_FACE_SOUTH; \
		FirstStrategy = E_PURE_UPDOWN; \
		SecondStrategy = E_PURE_UPDOWN; \
		break; \
	} \
	case RailType::Shape::AscendingSouth: \
	{ \
		SecondPos + Vector3i(0, 1, 0); \
		FirstFaceToCheck = BLOCK_FACE_NORTH; \
		SecondFaceToCheck = BLOCK_FACE_SOUTH; \
		FirstStrategy = E_PURE_UPDOWN; \
		SecondStrategy = E_PURE_UPDOWN; \
		break; \
	} \

	static bool IsUnstable(cChunkInterface & a_ChunkInterface, Vector3i a_Pos)
	{
		using namespace Block;
		auto Self = a_ChunkInterface.GetBlock(a_Pos);
		if (!IsBlockRail(Self))
		{
			return false;
		}
		auto FirstPos = a_Pos;
		auto SecondPos = a_Pos;
		eBlockFace FirstFaceToCheck = BLOCK_FACE_NONE;
		eBlockFace SecondFaceToCheck = BLOCK_FACE_NONE;
		ENUM_PURE FirstStrategy = E_PURE_UNSET;
		ENUM_PURE SecondStrategy = E_PURE_UNSET;
		switch (Self.Type())
		{
			case BlockType::Rail:
			{
				switch (Rail::Shape(Self))
				{
					ISUNSTABLE(Rail)
					case Rail::Shape::SouthEast:
					{
						FirstFaceToCheck = BLOCK_FACE_SOUTH;
						SecondFaceToCheck = BLOCK_FACE_EAST;
						FirstStrategy = E_PURE_UPDOWN;
						SecondStrategy = E_PURE_UPDOWN;
						break;
					}
					case Rail::Shape::SouthWest:
					{
						FirstFaceToCheck = BLOCK_FACE_SOUTH;
						SecondFaceToCheck = BLOCK_FACE_WEST;
						FirstStrategy = E_PURE_UPDOWN;
						SecondStrategy = E_PURE_UPDOWN;
						break;
					}
					case Rail::Shape::NorthWest:
					{
						FirstFaceToCheck = BLOCK_FACE_NORTH;
						SecondFaceToCheck = BLOCK_FACE_WEST;
						FirstStrategy = E_PURE_UPDOWN;
						SecondStrategy = E_PURE_UPDOWN;
						break;
					}
					case Rail::Shape::NorthEast:
					{
						FirstFaceToCheck = BLOCK_FACE_NORTH;
						SecondFaceToCheck = BLOCK_FACE_EAST;
						FirstStrategy = E_PURE_UPDOWN;
						SecondStrategy = E_PURE_UPDOWN;
						break;
					}
				}
				break;
			}
			case BlockType::ActivatorRail:
			{
				switch (ActivatorRail::Shape(Self))
				{
					ISUNSTABLE(ActivatorRail)
				}
				break;
			}
			case BlockType::DetectorRail:
			{
				switch (DetectorRail::Shape(Self))
				{
					ISUNSTABLE(DetectorRail)
				}
				break;
			}
			case BlockType::PoweredRail:
			{
				switch (PoweredRail::Shape(Self))
				{
					ISUNSTABLE(PoweredRail)
				}
				break;
			}
			default: return false;
		}

		if (
			(FirstFaceToCheck == BLOCK_FACE_NONE) ||
			(SecondFaceToCheck == BLOCK_FACE_NONE) ||
			(FirstStrategy == E_PURE_UNSET) ||
			(SecondStrategy == E_PURE_UNSET)
		)
		{
			return false;
		}

		return
		(
			IsNotConnected(a_ChunkInterface, FirstPos,  FirstFaceToCheck, FirstStrategy) ||
			IsNotConnected(a_ChunkInterface, SecondPos, SecondFaceToCheck, SecondStrategy)
		);
	}


	static bool IsNotConnected(cChunkInterface  & a_ChunkInterface, Vector3i a_Pos, eBlockFace a_BlockFace, enum ENUM_PURE a_Pure = ENUM_PURE::E_PURE_UPDOWN)
	{
		using namespace Block;
		auto Self = a_ChunkInterface.GetBlock(a_Pos);
		a_Pos = AddFaceDirection(a_Pos, a_BlockFace, false);
		BlockState Other = 0;
		if (!IsBlockRail(a_ChunkInterface.GetBlock(a_Pos)))
		{
			if (!IsBlockRail(a_ChunkInterface.GetBlock(a_Pos + Vector3i(0, 1, 0))) || (a_Pure != E_PURE_UPDOWN))
			{
				if (!IsBlockRail(a_ChunkInterface.GetBlock(a_Pos - Vector3i(0, 1, 0))) || (a_Pure == E_PURE_NONE))
				{
					return true;
				}
				else
				{
					Other = a_ChunkInterface.GetBlock(a_Pos - Vector3i(0, 1, 0));
				}
			}
			else
			{
				Other = a_ChunkInterface.GetBlock(a_Pos + Vector3i(0, 1, 0));
			}
		}
		else
		{
			Other = a_ChunkInterface.GetBlock(a_Pos);
		}

		switch (a_BlockFace)
		{
			case BLOCK_FACE_NORTH:
			{
				switch (Self.Type())
				{
					case BlockType::Rail:
					{
						switch (Rail::Shape(Self))
						{
							case Rail::Shape::NorthSouth:
							case Rail::Shape::AscendingNorth:
							case Rail::Shape::AscendingSouth:
							case Rail::Shape::SouthEast:
							case Rail::Shape::SouthWest:
								return false;
							case Rail::Shape::EastWest:
							case Rail::Shape::AscendingEast:
							case Rail::Shape::AscendingWest:
							case Rail::Shape::NorthWest:
							case Rail::Shape::NorthEast:
							{
								return true;
							}
						}
						break;
					}
					case BlockType::ActivatorRail:
					{
						switch (ActivatorRail::Shape(Self))
						{
							case ActivatorRail::Shape::NorthSouth:
							case ActivatorRail::Shape::AscendingNorth:
							case ActivatorRail::Shape::AscendingSouth:
								return false;
							case ActivatorRail::Shape::EastWest:
							case ActivatorRail::Shape::AscendingEast:
							case ActivatorRail::Shape::AscendingWest:
							{
								return true;
							}
						}
						break;
					}
					case BlockType::DetectorRail:
					{
						switch (DetectorRail::Shape(Self))
						{
							case DetectorRail::Shape::NorthSouth:
							case DetectorRail::Shape::AscendingNorth:
							case DetectorRail::Shape::AscendingSouth:
								return false;
							case DetectorRail::Shape::EastWest:
							case DetectorRail::Shape::AscendingEast:
							case DetectorRail::Shape::AscendingWest:
							{
								return true;
							}
						}
						break;
					}
					case BlockType::PoweredRail:
					{
						switch (PoweredRail::Shape(Self))
						{
							case PoweredRail::Shape::NorthSouth:
							case PoweredRail::Shape::AscendingNorth:
							case PoweredRail::Shape::AscendingSouth:
								return false;
							case PoweredRail::Shape::EastWest:
							case PoweredRail::Shape::AscendingEast:
							case PoweredRail::Shape::AscendingWest:
							{
								return true;
							}
						}
						break;
					}
					default: return false;
				}
				break;
			}

			case BLOCK_FACE_SOUTH:
			{
				switch (Self.Type())
				{
					case BlockType::Rail:
					{
						switch (Rail::Shape(Self))
						{
							case Rail::Shape::NorthSouth:
							case Rail::Shape::AscendingNorth:
							case Rail::Shape::AscendingSouth:
							case Rail::Shape::NorthEast:
							case Rail::Shape::NorthWest:
								return false;
							case Rail::Shape::EastWest:
							case Rail::Shape::AscendingEast:
							case Rail::Shape::AscendingWest:
							case Rail::Shape::SouthEast:
							case Rail::Shape::SouthWest:
							{
								return true;
							}
						}
						break;
					}
					case BlockType::ActivatorRail:
					{
						switch (ActivatorRail::Shape(Self))
						{
							case ActivatorRail::Shape::NorthSouth:
							case ActivatorRail::Shape::AscendingNorth:
							case ActivatorRail::Shape::AscendingSouth:
								return false;
							case ActivatorRail::Shape::EastWest:
							case ActivatorRail::Shape::AscendingEast:
							case ActivatorRail::Shape::AscendingWest:
							{
								return true;
							}
						}
						break;
					}
					case BlockType::DetectorRail:
					{
						switch (DetectorRail::Shape(Self))
						{
							case DetectorRail::Shape::NorthSouth:
							case DetectorRail::Shape::AscendingNorth:
							case DetectorRail::Shape::AscendingSouth:
								return false;
							case DetectorRail::Shape::EastWest:
							case DetectorRail::Shape::AscendingEast:
							case DetectorRail::Shape::AscendingWest:
							{
								return true;
							}
						}
						break;
					}
					case BlockType::PoweredRail:
					{
						switch (PoweredRail::Shape(Self))
						{
							case PoweredRail::Shape::NorthSouth:
							case PoweredRail::Shape::AscendingNorth:
							case PoweredRail::Shape::AscendingSouth:
								return false;
							case PoweredRail::Shape::EastWest:
							case PoweredRail::Shape::AscendingEast:
							case PoweredRail::Shape::AscendingWest:
							{
								return true;
							}
						}
						break;
					}
					default: return false;
				}
				break;
			}

			case BLOCK_FACE_EAST:
			{
				switch (Self.Type())
				{
					case BlockType::Rail:
					{
						switch (Rail::Shape(Self))
						{
							case Rail::Shape::EastWest:
							case Rail::Shape::AscendingEast:
							case Rail::Shape::AscendingWest:
							case Rail::Shape::SouthWest:
							case Rail::Shape::NorthWest:
								return false;
							case Rail::Shape::NorthSouth:
							case Rail::Shape::AscendingNorth:
							case Rail::Shape::AscendingSouth:
							case Rail::Shape::SouthEast:
							case Rail::Shape::NorthEast:
							{
								return true;
							}
						}
						break;
					}
					case BlockType::ActivatorRail:
					{
						switch (ActivatorRail::Shape(Self))
						{
							case ActivatorRail::Shape::EastWest:
							case ActivatorRail::Shape::AscendingEast:
							case ActivatorRail::Shape::AscendingWest:
								return false;
							case ActivatorRail::Shape::NorthSouth:
							case ActivatorRail::Shape::AscendingNorth:
							case ActivatorRail::Shape::AscendingSouth:
							{
								return true;
							}
						}
						break;
					}
					case BlockType::DetectorRail:
					{
						switch (DetectorRail::Shape(Self))
						{
							case DetectorRail::Shape::EastWest:
							case DetectorRail::Shape::AscendingEast:
							case DetectorRail::Shape::AscendingWest:
								return false;
							case DetectorRail::Shape::NorthSouth:
							case DetectorRail::Shape::AscendingNorth:
							case DetectorRail::Shape::AscendingSouth:
							{
								return true;
							}
						}
						break;
					}
					case BlockType::PoweredRail:
					{
						switch (PoweredRail::Shape(Self))
						{
							case PoweredRail::Shape::EastWest:
							case PoweredRail::Shape::AscendingEast:
							case PoweredRail::Shape::AscendingWest:
								return false;
							case PoweredRail::Shape::NorthSouth:
							case PoweredRail::Shape::AscendingNorth:
							case PoweredRail::Shape::AscendingSouth:
							{
								return true;
							}
						}
						break;
					}
					default: return false;
				}
				break;
			}
			case BLOCK_FACE_WEST:
			{
				switch (Self.Type())
				{
					case BlockType::Rail:
					{
						switch (Rail::Shape(Self))
						{
							case Rail::Shape::EastWest:
							case Rail::Shape::AscendingEast:
							case Rail::Shape::AscendingWest:
							case Rail::Shape::SouthEast:
							case Rail::Shape::NorthEast:
								return false;
							case Rail::Shape::NorthSouth:
							case Rail::Shape::AscendingNorth:
							case Rail::Shape::AscendingSouth:
							case Rail::Shape::SouthWest:
							case Rail::Shape::NorthWest:
							{
								return true;
							}
						}
						break;
					}
					case BlockType::ActivatorRail:
					{
						switch (ActivatorRail::Shape(Self))
						{
							case ActivatorRail::Shape::EastWest:
							case ActivatorRail::Shape::AscendingEast:
							case ActivatorRail::Shape::AscendingWest:
								return false;
							case ActivatorRail::Shape::NorthSouth:
							case ActivatorRail::Shape::AscendingNorth:
							case ActivatorRail::Shape::AscendingSouth:
							{
								return true;
							}
						}
						break;
					}
					case BlockType::DetectorRail:
					{
						switch (DetectorRail::Shape(Self))
						{
							case DetectorRail::Shape::EastWest:
							case DetectorRail::Shape::AscendingEast:
							case DetectorRail::Shape::AscendingWest:
								return false;
							case DetectorRail::Shape::NorthSouth:
							case DetectorRail::Shape::AscendingNorth:
							case DetectorRail::Shape::AscendingSouth:
							{
								return true;
							}
						}
						break;
					}
					case BlockType::PoweredRail:
					{
						switch (PoweredRail::Shape(Self))
						{
							case PoweredRail::Shape::EastWest:
							case PoweredRail::Shape::AscendingEast:
							case PoweredRail::Shape::AscendingWest:
								return false;
							case PoweredRail::Shape::NorthSouth:
							case PoweredRail::Shape::AscendingNorth:
							case PoweredRail::Shape::AscendingSouth:
							{
								return true;
							}
						}
						break;
					}
					default: return false;
				}
				break;
			}
			case BLOCK_FACE_NONE:
			case BLOCK_FACE_YM:
			case BLOCK_FACE_YP:
			{
				break;
			}
		}
		return true;
	}


	virtual void OnNeighborChanged(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos, eBlockFace a_WhichNeighbor) const override
	{
		const auto OldBlock = a_ChunkInterface.GetBlock(a_BlockPos);
		const auto NewBlock = FindBlock(a_ChunkInterface, a_BlockPos, OldBlock);
		if ((OldBlock != NewBlock) && IsUnstable(a_ChunkInterface, a_BlockPos))
		{
			a_ChunkInterface.FastSetBlock(a_BlockPos, NewBlock);
		}

		Super::OnNeighborChanged(a_ChunkInterface, a_BlockPos, a_WhichNeighbor);
	}

	virtual BlockState RotateCCW(BlockState a_Block) const override
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::Rail:
			{
				switch (Rail::Shape(a_Block))
				{

					case Rail::Shape::NorthSouth:     return Rail::Rail(Rail::Shape::EastWest, Rail::Waterlogged(a_Block));
					case Rail::Shape::EastWest:       return Rail::Rail(Rail::Shape::NorthSouth, Rail::Waterlogged(a_Block));
					case Rail::Shape::AscendingEast:  return Rail::Rail(Rail::Shape::AscendingNorth, Rail::Waterlogged(a_Block));
					case Rail::Shape::AscendingWest:  return Rail::Rail(Rail::Shape::AscendingSouth, Rail::Waterlogged(a_Block));
					case Rail::Shape::AscendingNorth: return Rail::Rail(Rail::Shape::AscendingWest, Rail::Waterlogged(a_Block));
					case Rail::Shape::AscendingSouth: return Rail::Rail(Rail::Shape::AscendingEast, Rail::Waterlogged(a_Block));
					case Rail::Shape::SouthEast:      return Rail::Rail(Rail::Shape::NorthEast, Rail::Waterlogged(a_Block));
					case Rail::Shape::SouthWest:      return Rail::Rail(Rail::Shape::SouthEast, Rail::Waterlogged(a_Block));
					case Rail::Shape::NorthWest:      return Rail::Rail(Rail::Shape::SouthWest, Rail::Waterlogged(a_Block));
					case Rail::Shape::NorthEast:      return Rail::Rail(Rail::Shape::NorthWest, Rail::Waterlogged(a_Block));
				}
				break;
			}
			case BlockType::ActivatorRail:
			{
				switch (ActivatorRail::Shape(a_Block))
				{
					case ActivatorRail::Shape::NorthSouth:     return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::EastWest, ActivatorRail::Waterlogged(a_Block));
					case ActivatorRail::Shape::EastWest:       return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::NorthSouth, ActivatorRail::Waterlogged(a_Block));
					case ActivatorRail::Shape::AscendingEast:  return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingNorth, ActivatorRail::Waterlogged(a_Block));
					case ActivatorRail::Shape::AscendingWest:  return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingSouth, ActivatorRail::Waterlogged(a_Block));
					case ActivatorRail::Shape::AscendingNorth: return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingWest, ActivatorRail::Waterlogged(a_Block));
					case ActivatorRail::Shape::AscendingSouth: return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingEast, ActivatorRail::Waterlogged(a_Block));
				}
				break;
			}
			case BlockType::DetectorRail:
			{
				switch (DetectorRail::Shape(a_Block))
				{
					case DetectorRail::Shape::NorthSouth:     return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::EastWest, DetectorRail::Waterlogged(a_Block));
					case DetectorRail::Shape::EastWest:       return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::NorthSouth, DetectorRail::Waterlogged(a_Block));
					case DetectorRail::Shape::AscendingEast:  return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingNorth, DetectorRail::Waterlogged(a_Block));
					case DetectorRail::Shape::AscendingWest:  return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingSouth, DetectorRail::Waterlogged(a_Block));
					case DetectorRail::Shape::AscendingNorth: return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingWest, DetectorRail::Waterlogged(a_Block));
					case DetectorRail::Shape::AscendingSouth: return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingEast, DetectorRail::Waterlogged(a_Block));
				}
				break;
			}
			case BlockType::PoweredRail:
			{
				switch (PoweredRail::Shape(a_Block))
				{
					case PoweredRail::Shape::NorthSouth:     return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::EastWest, PoweredRail::Waterlogged(a_Block));
					case PoweredRail::Shape::EastWest:       return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::NorthSouth, PoweredRail::Waterlogged(a_Block));
					case PoweredRail::Shape::AscendingEast:  return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingNorth, PoweredRail::Waterlogged(a_Block));
					case PoweredRail::Shape::AscendingWest:  return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingSouth, PoweredRail::Waterlogged(a_Block));
					case PoweredRail::Shape::AscendingNorth: return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingWest, PoweredRail::Waterlogged(a_Block));
					case PoweredRail::Shape::AscendingSouth: return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingEast, PoweredRail::Waterlogged(a_Block));
				}
				break;
			}
			default: return a_Block;
		}
		return a_Block;
	}


	virtual BlockState RotateCW(BlockState a_Block) const override
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::Rail:
			{
				switch (Rail::Shape(a_Block))
				{
					case Rail::Shape::NorthSouth:     return Rail::Rail(Rail::Shape::EastWest, Rail::Waterlogged(a_Block));
					case Rail::Shape::EastWest:       return Rail::Rail(Rail::Shape::NorthSouth, Rail::Waterlogged(a_Block));
					case Rail::Shape::AscendingEast:  return Rail::Rail(Rail::Shape::AscendingSouth, Rail::Waterlogged(a_Block));
					case Rail::Shape::AscendingWest:  return Rail::Rail(Rail::Shape::AscendingNorth, Rail::Waterlogged(a_Block));
					case Rail::Shape::AscendingNorth: return Rail::Rail(Rail::Shape::AscendingEast, Rail::Waterlogged(a_Block));
					case Rail::Shape::AscendingSouth: return Rail::Rail(Rail::Shape::AscendingWest, Rail::Waterlogged(a_Block));
					case Rail::Shape::SouthEast:      return Rail::Rail(Rail::Shape::SouthWest, Rail::Waterlogged(a_Block));
					case Rail::Shape::SouthWest:      return Rail::Rail(Rail::Shape::NorthWest, Rail::Waterlogged(a_Block));
					case Rail::Shape::NorthWest:      return Rail::Rail(Rail::Shape::NorthEast, Rail::Waterlogged(a_Block));
					case Rail::Shape::NorthEast:      return Rail::Rail(Rail::Shape::SouthEast, Rail::Waterlogged(a_Block));
				}
				break;
			}
			case BlockType::ActivatorRail:
			{
				switch (ActivatorRail::Shape(a_Block))
				{
					case ActivatorRail::Shape::NorthSouth:     return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::EastWest, ActivatorRail::Waterlogged(a_Block));
					case ActivatorRail::Shape::EastWest:       return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::NorthSouth, ActivatorRail::Waterlogged(a_Block));
					case ActivatorRail::Shape::AscendingEast:  return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingSouth, ActivatorRail::Waterlogged(a_Block));
					case ActivatorRail::Shape::AscendingWest:  return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingNorth, ActivatorRail::Waterlogged(a_Block));
					case ActivatorRail::Shape::AscendingNorth: return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingEast, ActivatorRail::Waterlogged(a_Block));
					case ActivatorRail::Shape::AscendingSouth: return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingWest, ActivatorRail::Waterlogged(a_Block));
				}
				break;
			}
			case BlockType::DetectorRail:
			{
				switch (DetectorRail::Shape(a_Block))
				{
					case DetectorRail::Shape::NorthSouth:     return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::EastWest, DetectorRail::Waterlogged(a_Block));
					case DetectorRail::Shape::EastWest:       return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::NorthSouth, DetectorRail::Waterlogged(a_Block));
					case DetectorRail::Shape::AscendingEast:  return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingSouth, DetectorRail::Waterlogged(a_Block));
					case DetectorRail::Shape::AscendingWest:  return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingNorth, DetectorRail::Waterlogged(a_Block));
					case DetectorRail::Shape::AscendingNorth: return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingEast, DetectorRail::Waterlogged(a_Block));
					case DetectorRail::Shape::AscendingSouth: return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingWest, DetectorRail::Waterlogged(a_Block));
				}
				break;
			}
			case BlockType::PoweredRail:
			{
				switch (PoweredRail::Shape(a_Block))
				{
					case PoweredRail::Shape::NorthSouth:     return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::EastWest, PoweredRail::Waterlogged(a_Block));
					case PoweredRail::Shape::EastWest:       return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::NorthSouth, PoweredRail::Waterlogged(a_Block));
					case PoweredRail::Shape::AscendingEast:  return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingSouth, PoweredRail::Waterlogged(a_Block));
					case PoweredRail::Shape::AscendingWest:  return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingNorth, PoweredRail::Waterlogged(a_Block));
					case PoweredRail::Shape::AscendingNorth: return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingEast, PoweredRail::Waterlogged(a_Block));
					case PoweredRail::Shape::AscendingSouth: return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingWest, PoweredRail::Waterlogged(a_Block));
				}
				break;
			}
			default: return a_Block;
		}
		return a_Block;
	}


	virtual BlockState MirrorXY(BlockState a_Block) const override
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::Rail:
			{
				switch (Rail::Shape(a_Block))
				{
					case Rail::Shape::AscendingNorth: return Rail::Rail(Rail::Shape::AscendingSouth, Rail::Waterlogged(a_Block));
					case Rail::Shape::AscendingSouth: return Rail::Rail(Rail::Shape::AscendingNorth, Rail::Waterlogged(a_Block));
					case Rail::Shape::SouthEast:      return Rail::Rail(Rail::Shape::NorthEast, Rail::Waterlogged(a_Block));
					case Rail::Shape::SouthWest:      return Rail::Rail(Rail::Shape::NorthWest, Rail::Waterlogged(a_Block));
					case Rail::Shape::NorthWest:      return Rail::Rail(Rail::Shape::SouthWest, Rail::Waterlogged(a_Block));
					case Rail::Shape::NorthEast:      return Rail::Rail(Rail::Shape::SouthEast, Rail::Waterlogged(a_Block));
					default: return a_Block;
				}
			}
			case BlockType::ActivatorRail:
			{
				switch (ActivatorRail::Shape(a_Block))
				{
					case ActivatorRail::Shape::AscendingNorth: return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingSouth, ActivatorRail::Waterlogged(a_Block));
					case ActivatorRail::Shape::AscendingSouth: return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingNorth, ActivatorRail::Waterlogged(a_Block));
					default: return a_Block;
				}
			}
			case BlockType::DetectorRail:
			{
				switch (DetectorRail::Shape(a_Block))
				{
					case DetectorRail::Shape::AscendingNorth: return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingSouth, DetectorRail::Waterlogged(a_Block));
					case DetectorRail::Shape::AscendingSouth: return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingNorth, DetectorRail::Waterlogged(a_Block));
					default: return a_Block;
				}
			}
			case BlockType::PoweredRail:
			{
				switch (PoweredRail::Shape(a_Block))
				{
					case PoweredRail::Shape::AscendingNorth: return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingSouth, PoweredRail::Waterlogged(a_Block));
					case PoweredRail::Shape::AscendingSouth: return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingNorth, PoweredRail::Waterlogged(a_Block));
					default: return a_Block;
				}
			}
			default: return a_Block;
		}
	}


	virtual BlockState MirrorYZ(BlockState a_Block) const override
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::Rail:
			{
				switch (Rail::Shape(a_Block))
				{
					case Rail::Shape::AscendingEast:  return Rail::Rail(Rail::Shape::AscendingWest, Rail::Waterlogged(a_Block));
					case Rail::Shape::AscendingWest:  return Rail::Rail(Rail::Shape::AscendingEast, Rail::Waterlogged(a_Block));
					case Rail::Shape::SouthEast:      return Rail::Rail(Rail::Shape::SouthWest, Rail::Waterlogged(a_Block));
					case Rail::Shape::SouthWest:      return Rail::Rail(Rail::Shape::SouthEast, Rail::Waterlogged(a_Block));
					case Rail::Shape::NorthWest:      return Rail::Rail(Rail::Shape::NorthEast, Rail::Waterlogged(a_Block));
					case Rail::Shape::NorthEast:      return Rail::Rail(Rail::Shape::NorthWest, Rail::Waterlogged(a_Block));
					default: return a_Block;
				}
			}
			case BlockType::ActivatorRail:
			{
				switch (ActivatorRail::Shape(a_Block))
				{
					case ActivatorRail::Shape::AscendingEast:  return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingWest, ActivatorRail::Waterlogged(a_Block));
					case ActivatorRail::Shape::AscendingWest:  return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingEast, ActivatorRail::Waterlogged(a_Block));
					default: return a_Block;
				}
			}
			case BlockType::DetectorRail:
			{
				switch (DetectorRail::Shape(a_Block))
				{
					case DetectorRail::Shape::AscendingEast:  return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingWest, DetectorRail::Waterlogged(a_Block));
					case DetectorRail::Shape::AscendingWest:  return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingEast, DetectorRail::Waterlogged(a_Block));
					default: return a_Block;
				}
			}
			case BlockType::PoweredRail:
			{
				switch (PoweredRail::Shape(a_Block))
				{
					case PoweredRail::Shape::AscendingEast:  return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingWest, PoweredRail::Waterlogged(a_Block));
					case PoweredRail::Shape::AscendingWest:  return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingEast, PoweredRail::Waterlogged(a_Block));
					default: return a_Block;
				}
			}
			default: return a_Block;
		}
	}


	virtual ColourID GetMapBaseColourID() const override
	{
		return 0;
	}
} ;
