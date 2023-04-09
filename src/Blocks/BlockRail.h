#pragma once

#include "BlockHandler.h"
#include "BlockSlab.h"
#include "BlockStairs.h"
#include "BlockType.h"
#include "Blocks/Mixins.h"
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
	public cClearMetaOnDrop<cBlockHandler>
{
	using Super = cClearMetaOnDrop<cBlockHandler>;

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

	static constexpr bool IsBlockRail(BlockState a_Block)
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

	// Todo: Add this, once the BlockState are implemented
	/*
	static constexpr void SetPowered(cChunkInterface & a_ChunkInterface, Vector3i a_Position, bool a_Powered)
	{
		using namespace Block;
		auto Self = a_ChunkInterface.GetBlock(a_Position);
		switch (Self.Type())
		{
			case BlockType::Rail: break;   // Nothing to do
			case BlockType::ActivatorRail: a_ChunkInterface.FastSetBlock(a_Position, ActivatorRail::ActivatorRail(a_Powered, ActivatorRail::Shape(Self))); break;
			case BlockType::DetectorRail:  a_ChunkInterface.FastSetBlock(a_Position, DetectorRail::DetectorRail(a_Powered, DetectorRail::Shape(Self))); break;
			case BlockType::PoweredRail:   a_ChunkInterface.FastSetBlock(a_Position, PoweredRail::PoweredRail(a_Powered, PoweredRail::Shape(Self))); break;
			default: break;
		}
	}
	*/

	static constexpr enum Shape GetShapeFromRail(BlockState a_Block)
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

	static constexpr BlockState GetRailFromShape(BlockState a_Block, Shape a_Shape)
	{
		using namespace Block;

		switch (a_Block.Type())
		{
			case BlockType::Rail:
			{
				switch (a_Shape)
				{
					case Shape::NorthSouth:      return Rail::Rail(Rail::Shape::NorthSouth);
					case Shape::EastWest:        return Rail::Rail(Rail::Shape::EastWest);
					case Shape::AscendingEast:   return Rail::Rail(Rail::Shape::AscendingEast);
					case Shape::AscendingWest:   return Rail::Rail(Rail::Shape::AscendingWest);
					case Shape::AscendingNorth:  return Rail::Rail(Rail::Shape::AscendingNorth);
					case Shape::AscendingSouth:  return Rail::Rail(Rail::Shape::AscendingSouth);
					case Shape::SouthEast:       return Rail::Rail(Rail::Shape::SouthEast);
					case Shape::SouthWest:       return Rail::Rail(Rail::Shape::SouthWest);
					case Shape::NorthWest:       return Rail::Rail(Rail::Shape::NorthWest);
					case Shape::NorthEast:       return Rail::Rail(Rail::Shape::NorthEast);
					case Shape::None:            return a_Block;
				}
				break;
			}
			case BlockType::ActivatorRail:
			{
				switch (a_Shape)
				{
					case Shape::NorthSouth:      return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::NorthSouth);
					case Shape::EastWest:        return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::EastWest);
					case Shape::AscendingEast:   return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingEast);
					case Shape::AscendingWest:   return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingWest);
					case Shape::AscendingNorth:  return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingNorth);
					case Shape::AscendingSouth:  return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingSouth);
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
					case Shape::NorthSouth:      return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::NorthSouth);
					case Shape::EastWest:        return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::EastWest);
					case Shape::AscendingEast:   return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingEast);
					case Shape::AscendingWest:   return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingWest);
					case Shape::AscendingNorth:  return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingNorth);
					case Shape::AscendingSouth:  return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingSouth);
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
					case Shape::NorthSouth:      return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::NorthSouth);
					case Shape::EastWest:        return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::EastWest);
					case Shape::AscendingEast:   return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingEast);
					case Shape::AscendingWest:   return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingWest);
					case Shape::AscendingNorth:  return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingNorth);
					case Shape::AscendingSouth:  return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingSouth);
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





	static NIBBLETYPE FindMeta(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos, BLOCKTYPE a_RailType)
	{
		NIBBLETYPE Meta = 0;
		char RailsCnt = 0;
		bool Neighbors[8];  // 0 - EAST, 1 - WEST, 2 - NORTH, 3 - SOUTH, 4 - EAST UP, 5 - WEST UP, 6 - NORTH UP, 7 - SOUTH UP
		memset(Neighbors, 0, sizeof(Neighbors));
		Neighbors[0] = (IsUnstable(a_ChunkInterface, a_BlockPos + Vector3i(1, 0, 0)) || !IsNotConnected(a_ChunkInterface, a_BlockPos, BLOCK_FACE_EAST, E_PURE_DOWN));
		Neighbors[1] = (IsUnstable(a_ChunkInterface, a_BlockPos + Vector3i(-1, 0, 0)) || !IsNotConnected(a_ChunkInterface, a_BlockPos, BLOCK_FACE_WEST, E_PURE_DOWN));
		Neighbors[2] = (IsUnstable(a_ChunkInterface, a_BlockPos + Vector3i(0, 0, -1)) || !IsNotConnected(a_ChunkInterface, a_BlockPos, BLOCK_FACE_NORTH, E_PURE_DOWN));
		Neighbors[3] = (IsUnstable(a_ChunkInterface, a_BlockPos + Vector3i(0, 0, 1)) || !IsNotConnected(a_ChunkInterface, a_BlockPos, BLOCK_FACE_SOUTH, E_PURE_DOWN));
		Neighbors[4] = (IsUnstable(a_ChunkInterface, a_BlockPos + Vector3i(1, 1, 0)) || !IsNotConnected(a_ChunkInterface, a_BlockPos + Vector3i(0, 1, 0), BLOCK_FACE_EAST, E_PURE_NONE));
		Neighbors[5] = (IsUnstable(a_ChunkInterface, a_BlockPos + Vector3i(-1, 1, 0)) || !IsNotConnected(a_ChunkInterface, a_BlockPos + Vector3i(0, 1, 0), BLOCK_FACE_WEST, E_PURE_NONE));
		Neighbors[6] = (IsUnstable(a_ChunkInterface, a_BlockPos + Vector3i(0, 1, -1)) || !IsNotConnected(a_ChunkInterface, a_BlockPos + Vector3i(0, 1, 0), BLOCK_FACE_NORTH, E_PURE_NONE));
		Neighbors[7] = (IsUnstable(a_ChunkInterface, a_BlockPos + Vector3i(0, 1, 1)) || !IsNotConnected(a_ChunkInterface, a_BlockPos + Vector3i(0, 1, 0), BLOCK_FACE_SOUTH, E_PURE_NONE));
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
				return E_META_RAIL_ASCEND_ZP;
			}
			else if (Neighbors[6])
			{
				return E_META_RAIL_ASCEND_ZM;
			}
			else if (Neighbors[5])
			{
				return E_META_RAIL_ASCEND_XM;
			}
			else if (Neighbors[4])
			{
				return E_META_RAIL_ASCEND_XP;
			}
			else if (Neighbors[0] || Neighbors[1])
			{
				return E_META_RAIL_XM_XP;
			}
			else if (Neighbors[2] || Neighbors[3])
			{
				return E_META_RAIL_ZM_ZP;
			}
			ASSERT(!"Weird neighbor count");
			return Meta;
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
			const bool CanCurve = a_RailType == E_BLOCK_RAIL;

			if (Neighbors[3] && Neighbors[0] && CanCurve)
			{
				return E_META_RAIL_CURVED_ZP_XP;
			}
			else if (Neighbors[3] && Neighbors[1] && CanCurve)
			{
				return E_META_RAIL_CURVED_ZP_XM;
			}
			else if (Neighbors[2] && Neighbors[0] && CanCurve)
			{
				return E_META_RAIL_CURVED_ZM_XP;
			}
			else if (Neighbors[2] && Neighbors[1] && CanCurve)
			{
				return E_META_RAIL_CURVED_ZM_XM;
			}
			else if (Neighbors[7] && Neighbors[2])
			{
				return E_META_RAIL_ASCEND_ZP;
			}
			else if (Neighbors[3] && Neighbors[6])
			{
				return E_META_RAIL_ASCEND_ZM;
			}
			else if (Neighbors[5] && Neighbors[0])
			{
				return E_META_RAIL_ASCEND_XM;
			}
			else if (Neighbors[4] && Neighbors[1])
			{
				return E_META_RAIL_ASCEND_XP;
			}
			else if (Neighbors[0] && Neighbors[1])
			{
				return E_META_RAIL_XM_XP;
			}
			else if (Neighbors[2] && Neighbors[3])
			{
				return E_META_RAIL_ZM_ZP;
			}

			if (CanCurve)
			{
				ASSERT(!"Weird neighbor count");
			}
		}
		return Meta;
	}

private:

	static bool CanBeSupportedBy(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
	{
		if (cBlockSlabHandler::IsAnySlabType(a_BlockType))
		{
			return (a_BlockMeta & E_META_WOODEN_SLAB_UPSIDE_DOWN);
		}
		else if (cBlockStairsHandler::IsAnyStairType(a_BlockType))
		{
			return (a_BlockMeta & E_BLOCK_STAIRS_UPSIDE_DOWN);
		}
		return cBlockInfo::FullyOccupiesVoxel(a_BlockType);
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

	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, NIBBLETYPE a_Meta) const override
	{
		BLOCKTYPE BelowBlock;
		NIBBLETYPE BelowBlockMeta;
		a_Chunk.GetBlockTypeMeta(a_Position.addedY(-1), BelowBlock, BelowBlockMeta);

		if ((a_Position.y <= 0) || !CanBeSupportedBy(BelowBlock, BelowBlockMeta))
		{
			return false;
		}

		switch (a_Meta)
		{
			case E_META_RAIL_ASCEND_XP:
			case E_META_RAIL_ASCEND_XM:
			case E_META_RAIL_ASCEND_ZM:
			case E_META_RAIL_ASCEND_ZP:
			{
				// Mapping between the meta and the neighbors that need checking
				a_Meta -= E_META_RAIL_ASCEND_XP;  // Base index at zero
				static const Vector3i Coords[] =
				{
					{ 1, 0,  0},  // east,  XP
					{-1, 0,  0},  // west,  XM
					{ 0, 0, -1},  // north, ZM
					{ 0, 0,  1},  // south, ZP
				} ;
				BLOCKTYPE  BlockType;
				NIBBLETYPE BlockMeta;
				if (!a_Chunk.UnboundedRelGetBlock(a_Position + Coords[a_Meta], BlockType, BlockMeta))
				{
					// Too close to the edge, cannot simulate
					return true;
				}
				return cBlockInfo::FullyOccupiesVoxel(BlockType);
			}
		}

		return true;
	}

	static bool CanThisRailCurve(BlockState a_Block)
	{
		return a_Block.Type() == BlockType::Rail;
	}

#define IS_UNSTABLE(RailType) \
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
		if (!IsBlockRail(a_ChunkInterface.GetBlock(a_Pos)))
		{
			return false;
		}
		NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(a_Pos);
		switch (Meta)
		{
			case E_META_RAIL_ZM_ZP:
			{
				if (
					IsNotConnected(a_ChunkInterface, a_Pos, BLOCK_FACE_NORTH, E_PURE_DOWN) ||
					IsNotConnected(a_ChunkInterface, a_Pos, BLOCK_FACE_SOUTH, E_PURE_DOWN)
				)
				{
					return true;
				}
				break;
			}

			case E_META_RAIL_XM_XP:
			{
				if (
					IsNotConnected(a_ChunkInterface, a_Pos, BLOCK_FACE_EAST, E_PURE_DOWN) ||
					IsNotConnected(a_ChunkInterface, a_Pos, BLOCK_FACE_WEST, E_PURE_DOWN)
				)
				{
					return true;
				}
				break;
			}

			case E_META_RAIL_ASCEND_XP:
			{
				if (
					IsNotConnected(a_ChunkInterface, a_Pos + Vector3i(0, 1, 0), BLOCK_FACE_EAST) ||
					IsNotConnected(a_ChunkInterface, a_Pos, BLOCK_FACE_WEST)
				)
				{
					return true;
				}
				break;
			}

			case E_META_RAIL_ASCEND_XM:
			{
				if (
					IsNotConnected(a_ChunkInterface, a_Pos, BLOCK_FACE_EAST) ||
					IsNotConnected(a_ChunkInterface, a_Pos + Vector3i(0, 1, 0), BLOCK_FACE_WEST)
				)
				{
					return true;
				}
				break;
			}

			case E_META_RAIL_ASCEND_ZM:
			{
				if (
					IsNotConnected(a_ChunkInterface, a_Pos + Vector3i(0, 1, 0), BLOCK_FACE_NORTH) ||
					IsNotConnected(a_ChunkInterface, a_Pos, BLOCK_FACE_SOUTH)
				)
				{
					return true;
				}
				break;
			}

			case E_META_RAIL_ASCEND_ZP:
			{
				if (
					IsNotConnected(a_ChunkInterface, a_Pos, BLOCK_FACE_NORTH) ||
					IsNotConnected(a_ChunkInterface, a_Pos + Vector3i(0, 1, 0), BLOCK_FACE_SOUTH)
				)
				{
					return true;
				}
				break;
			}

			case E_META_RAIL_CURVED_ZP_XP:
			{
				if (
					IsNotConnected(a_ChunkInterface, a_Pos, BLOCK_FACE_SOUTH) ||
					IsNotConnected(a_ChunkInterface, a_Pos, BLOCK_FACE_EAST)
				)
				{
					return true;
				}
				break;
			}

			case E_META_RAIL_CURVED_ZP_XM:
			{
				if (
					IsNotConnected(a_ChunkInterface, a_Pos, BLOCK_FACE_SOUTH) ||
					IsNotConnected(a_ChunkInterface, a_Pos, BLOCK_FACE_WEST)
				)
				{
					return true;
				}
				break;
			}

			case E_META_RAIL_CURVED_ZM_XM:
			{
				if (
					IsNotConnected(a_ChunkInterface, a_Pos, BLOCK_FACE_NORTH) ||
					IsNotConnected(a_ChunkInterface, a_Pos, BLOCK_FACE_WEST)
				)
				{
					return true;
				}
				break;
			}

			case E_META_RAIL_CURVED_ZM_XP:
			{
				if (
					IsNotConnected(a_ChunkInterface, a_Pos, BLOCK_FACE_NORTH) ||
					IsNotConnected(a_ChunkInterface, a_Pos, BLOCK_FACE_EAST)
				)
				{
					return true;
				}
				break;
			}
		}
		return false;
	}
#undef IS_UNSTABLE

	static bool IsNotConnected(cChunkInterface  & a_ChunkInterface, Vector3i a_Pos, eBlockFace a_BlockFace, char a_Pure = 0)
	{
		a_Pos = AddFaceDirection(a_Pos, a_BlockFace, false);

		NIBBLETYPE Meta;

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
					Meta = a_ChunkInterface.GetBlockMeta(a_Pos - Vector3i(0, 1, 0));
				}
			}
			else
			{
				Meta = a_ChunkInterface.GetBlockMeta(a_Pos + Vector3i(0, 1, 0));
			}
		}
		else
		{
			Meta = a_ChunkInterface.GetBlockMeta(a_Pos);
		}

		switch (a_BlockFace)
		{
			case BLOCK_FACE_NORTH:
			{
				if (
					(Meta == E_META_RAIL_ZM_ZP) ||
					(Meta == E_META_RAIL_ASCEND_ZM) ||
					(Meta == E_META_RAIL_ASCEND_ZP) ||
					(Meta == E_META_RAIL_CURVED_ZP_XP) ||
					(Meta == E_META_RAIL_CURVED_ZP_XM)
				)
				{
					return false;
				}
				break;
			}

			case BLOCK_FACE_SOUTH:
			{
				if (
					(Meta == E_META_RAIL_ZM_ZP) ||
					(Meta == E_META_RAIL_ASCEND_ZM) ||
					(Meta == E_META_RAIL_ASCEND_ZP) ||
					(Meta == E_META_RAIL_CURVED_ZM_XP) ||
					(Meta == E_META_RAIL_CURVED_ZM_XM)
				)
				{
					return false;
				}
				break;
			}

			case BLOCK_FACE_EAST:
			{
				if (
					(Meta == E_META_RAIL_XM_XP) ||
					(Meta == E_META_RAIL_ASCEND_XP) ||
					(Meta == E_META_RAIL_ASCEND_XM) ||
					(Meta == E_META_RAIL_CURVED_ZP_XM) ||
					(Meta == E_META_RAIL_CURVED_ZM_XM)
				)
				{
					return false;
				}
				break;
			}
			case BLOCK_FACE_WEST:
			{
				if (
					(Meta == E_META_RAIL_XM_XP) ||
					(Meta == E_META_RAIL_ASCEND_XP) ||
					(Meta == E_META_RAIL_ASCEND_XM) ||
					(Meta == E_META_RAIL_CURVED_ZP_XP) ||
					(Meta == E_META_RAIL_CURVED_ZM_XP)
				)
				{
					return false;
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


	virtual void OnBroken(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		Vector3i a_BlockPos,
		BLOCKTYPE a_OldBlockType, NIBBLETYPE a_OldBlockMeta,
		const cEntity * a_Digger
	) const override
	{
		Super::OnBroken(a_ChunkInterface, a_WorldInterface, a_BlockPos, a_OldBlockType, a_OldBlockMeta, a_Digger);

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


	virtual void OnNeighborChanged(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos, eBlockFace a_WhichNeighbor) const override
	{
		const auto Meta = a_ChunkInterface.GetBlockMeta(a_BlockPos);
		const auto NewMeta = FindMeta(a_ChunkInterface, a_BlockPos, m_BlockType);
		if ((Meta != NewMeta) && IsUnstable(a_ChunkInterface, a_BlockPos))
		{
			a_ChunkInterface.FastSetBlock(a_BlockPos, m_BlockType, (m_BlockType == E_BLOCK_RAIL) ? NewMeta : NewMeta | (Meta & 0x08));
		}

		Super::OnNeighborChanged(a_ChunkInterface, a_BlockPos, a_WhichNeighbor);
	}


	virtual void OnPlaced(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		Vector3i a_BlockPos,
		BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta
	) const override
	{
		Super::OnPlaced(a_ChunkInterface, a_WorldInterface, a_BlockPos, a_BlockType, a_BlockMeta);

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


	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) const override
	{
		// Bit 0x08 is a flag when a_Meta is in the range 0x00--0x05 and 0x0A--0x0F.
		// Bit 0x08 specifies direction when a_Meta is in the range 0x06-0x09.
		if ((a_Meta < 0x06) || (a_Meta > 0x09))
		{
			//  Save powered rail flag.
			NIBBLETYPE OtherMeta = a_Meta & 0x08;
			// Rotates according to table; 0x07 == 0111.
			// Rails can either be flat (North / South) or Ascending (Asc. East)
			switch (a_Meta & 0x07)
			{
				case 0x00: return 0x01 + OtherMeta;  // North / South -> East / West
				case 0x01: return 0x00 + OtherMeta;  // East / West   -> North / South

				case 0x02: return 0x04 + OtherMeta;  // Asc. East   -> Asc. North
				case 0x04: return 0x03 + OtherMeta;  // Asc. North  -> Asc. West
				case 0x03: return 0x05 + OtherMeta;  // Asc. West   -> Asc. South
				case 0x05: return 0x02 + OtherMeta;  // Asc. South  -> Asc. East
			}
		}
		else
		{
			switch (a_Meta)
			{
				// Corner Directions
				case 0x06: return 0x09;  // Northwest Cnr. -> Southwest Cnr.
				case 0x07: return 0x06;  // Northeast Cnr. -> Northwest Cnr.
				case 0x08: return 0x07;  // Southeast Cnr. -> Northeast Cnr.
				case 0x09: return 0x08;  // Southwest Cnr. -> Southeast Cnr.
			}
		}
		// To avoid a compiler warning;
		return a_Meta;
	}


	virtual BlockState RotateCCW(BlockState a_Block) const
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::Rail:
			{
				switch (Rail::Shape(a_Block))
				{

					case Rail::Shape::NorthSouth:     return Rail::Rail(Rail::Shape::EastWest);
					case Rail::Shape::EastWest:       return Rail::Rail(Rail::Shape::NorthSouth);
					case Rail::Shape::AscendingEast:  return Rail::Rail(Rail::Shape::AscendingNorth);
					case Rail::Shape::AscendingWest:  return Rail::Rail(Rail::Shape::AscendingSouth);
					case Rail::Shape::AscendingNorth: return Rail::Rail(Rail::Shape::AscendingWest);
					case Rail::Shape::AscendingSouth: return Rail::Rail(Rail::Shape::AscendingEast);
					case Rail::Shape::SouthEast:      return Rail::Rail(Rail::Shape::NorthEast);
					case Rail::Shape::SouthWest:      return Rail::Rail(Rail::Shape::SouthEast);
					case Rail::Shape::NorthWest:      return Rail::Rail(Rail::Shape::SouthWest);
					case Rail::Shape::NorthEast:      return Rail::Rail(Rail::Shape::NorthWest);
				}
				break;
			}
			case BlockType::ActivatorRail:
			{
				switch (ActivatorRail::Shape(a_Block))
				{
					case ActivatorRail::Shape::NorthSouth:     return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::EastWest);
					case ActivatorRail::Shape::EastWest:       return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::NorthSouth);
					case ActivatorRail::Shape::AscendingEast:  return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingNorth);
					case ActivatorRail::Shape::AscendingWest:  return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingSouth);
					case ActivatorRail::Shape::AscendingNorth: return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingWest);
					case ActivatorRail::Shape::AscendingSouth: return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingEast);
				}
				break;
			}
			case BlockType::DetectorRail:
			{
				switch (DetectorRail::Shape(a_Block))
				{
					case DetectorRail::Shape::NorthSouth:     return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::EastWest);
					case DetectorRail::Shape::EastWest:       return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::NorthSouth);
					case DetectorRail::Shape::AscendingEast:  return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingNorth);
					case DetectorRail::Shape::AscendingWest:  return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingSouth);
					case DetectorRail::Shape::AscendingNorth: return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingWest);
					case DetectorRail::Shape::AscendingSouth: return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingEast);
				}
				break;
			}
			case BlockType::PoweredRail:
			{
				switch (PoweredRail::Shape(a_Block))
				{
					case PoweredRail::Shape::NorthSouth:     return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::EastWest);
					case PoweredRail::Shape::EastWest:       return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::NorthSouth);
					case PoweredRail::Shape::AscendingEast:  return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingNorth);
					case PoweredRail::Shape::AscendingWest:  return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingSouth);
					case PoweredRail::Shape::AscendingNorth: return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingWest);
					case PoweredRail::Shape::AscendingSouth: return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingEast);
				}
				break;
			}
			default: return a_Block;
		}
		return a_Block;
	}


	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta) const override
	{
		// Bit 0x08 is a flag for value in the range 0x00--0x05 and specifies direction for values withint 0x006--0x09.
		if ((a_Meta < 0x06) || (a_Meta > 0x09))
		{
			//  Save powered rail flag.
			NIBBLETYPE OtherMeta = a_Meta & 0x08;
			// Rotates according to table; 0x07 == 0111.
			// Rails can either be flat (North / South) or Ascending (Asc. East)
			switch (a_Meta & 0x07)
			{
				case 0x00: return 0x01 + OtherMeta;  // North / South -> East / West
				case 0x01: return 0x00 + OtherMeta;  // East / West   -> North / South

				case 0x02: return 0x05 + OtherMeta;  // Asc. East   -> Asc. South
				case 0x05: return 0x03 + OtherMeta;  // Asc. South  -> Asc. West
				case 0x03: return 0x04 + OtherMeta;  // Asc. West   -> Asc. North
				case 0x04: return 0x02 + OtherMeta;  // Asc. North  -> Asc. East
			}
		}
		else
		{
			switch (a_Meta)
			{
				// Corner Directions
				case 0x06: return 0x07;  // Northwest Cnr. -> Northeast Cnr.
				case 0x07: return 0x08;  // Northeast Cnr. -> Southeast Cnr.
				case 0x08: return 0x09;  // Southeast Cnr. -> Southwest Cnr.
				case 0x09: return 0x06;  // Southwest Cnr. -> Northwest Cnr.
			}
		}
		// To avoid a compiler warning;
		return a_Meta;
	}


	virtual BlockState RotateCW(BlockState a_Block) const
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::Rail:
			{
				switch (Rail::Shape(a_Block))
				{
					case Rail::Shape::NorthSouth:     return Rail::Rail(Rail::Shape::EastWest);
					case Rail::Shape::EastWest:       return Rail::Rail(Rail::Shape::NorthSouth);
					case Rail::Shape::AscendingEast:  return Rail::Rail(Rail::Shape::AscendingSouth);
					case Rail::Shape::AscendingWest:  return Rail::Rail(Rail::Shape::AscendingNorth);
					case Rail::Shape::AscendingNorth: return Rail::Rail(Rail::Shape::AscendingEast);
					case Rail::Shape::AscendingSouth: return Rail::Rail(Rail::Shape::AscendingWest);
					case Rail::Shape::SouthEast:      return Rail::Rail(Rail::Shape::SouthWest);
					case Rail::Shape::SouthWest:      return Rail::Rail(Rail::Shape::NorthWest);
					case Rail::Shape::NorthWest:      return Rail::Rail(Rail::Shape::NorthEast);
					case Rail::Shape::NorthEast:      return Rail::Rail(Rail::Shape::SouthEast);
				}
				break;
			}
			case BlockType::ActivatorRail:
			{
				switch (ActivatorRail::Shape(a_Block))
				{
					case ActivatorRail::Shape::NorthSouth:     return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::EastWest);
					case ActivatorRail::Shape::EastWest:       return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::NorthSouth);
					case ActivatorRail::Shape::AscendingEast:  return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingSouth);
					case ActivatorRail::Shape::AscendingWest:  return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingNorth);
					case ActivatorRail::Shape::AscendingNorth: return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingEast);
					case ActivatorRail::Shape::AscendingSouth: return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingWest);
				}
				break;
			}
			case BlockType::DetectorRail:
			{
				switch (DetectorRail::Shape(a_Block))
				{
					case DetectorRail::Shape::NorthSouth:     return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::EastWest);
					case DetectorRail::Shape::EastWest:       return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::NorthSouth);
					case DetectorRail::Shape::AscendingEast:  return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingSouth);
					case DetectorRail::Shape::AscendingWest:  return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingNorth);
					case DetectorRail::Shape::AscendingNorth: return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingEast);
					case DetectorRail::Shape::AscendingSouth: return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingWest);
				}
				break;
			}
			case BlockType::PoweredRail:
			{
				switch (PoweredRail::Shape(a_Block))
				{
					case PoweredRail::Shape::NorthSouth:     return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::EastWest);
					case PoweredRail::Shape::EastWest:       return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::NorthSouth);
					case PoweredRail::Shape::AscendingEast:  return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingSouth);
					case PoweredRail::Shape::AscendingWest:  return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingNorth);
					case PoweredRail::Shape::AscendingNorth: return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingEast);
					case PoweredRail::Shape::AscendingSouth: return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingWest);
				}
				break;
			}
			default: return a_Block;
		}
		return a_Block;
	}


	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta) const override
	{
		// MirrorXY basically flips the ZP and ZM parts of the meta
		if (m_BlockType == E_BLOCK_RAIL)
		{
			// Basic rails can have curves and thus their meta behaves differently from specialized rails:
			switch (a_Meta)
			{
				case E_META_RAIL_ASCEND_XM:    return E_META_RAIL_ASCEND_XM;
				case E_META_RAIL_ASCEND_XP:    return E_META_RAIL_ASCEND_XP;
				case E_META_RAIL_ASCEND_ZM:    return E_META_RAIL_ASCEND_ZP;
				case E_META_RAIL_ASCEND_ZP:    return E_META_RAIL_ASCEND_ZM;
				case E_META_RAIL_CURVED_ZM_XM: return E_META_RAIL_CURVED_ZP_XM;
				case E_META_RAIL_CURVED_ZM_XP: return E_META_RAIL_CURVED_ZP_XP;
				case E_META_RAIL_CURVED_ZP_XM: return E_META_RAIL_CURVED_ZM_XM;
				case E_META_RAIL_CURVED_ZP_XP: return E_META_RAIL_CURVED_ZM_XP;
				case E_META_RAIL_XM_XP:        return E_META_RAIL_XM_XP;
				case E_META_RAIL_ZM_ZP:        return E_META_RAIL_ZM_ZP;
			}
		}
		else
		{
			// Specialized rails don't have curves, instead they use bit 0x08 as a flag
			NIBBLETYPE flag = a_Meta & 0x08;
			switch (a_Meta & 0x07)
			{
				case E_META_RAIL_ASCEND_XM: return flag | E_META_RAIL_ASCEND_XM;
				case E_META_RAIL_ASCEND_XP: return flag | E_META_RAIL_ASCEND_XP;
				case E_META_RAIL_ASCEND_ZM: return flag | E_META_RAIL_ASCEND_ZP;
				case E_META_RAIL_ASCEND_ZP: return flag | E_META_RAIL_ASCEND_ZM;
				case E_META_RAIL_XM_XP:     return flag | E_META_RAIL_XM_XP;
				case E_META_RAIL_ZM_ZP:     return flag | E_META_RAIL_ZM_ZP;
			}
		}
		ASSERT(!"Unknown rail meta");
		return a_Meta;
	}


	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta) const override
	{
		// MirrorYZ basically flips the XP and XM parts of the meta
		if (m_BlockType == E_BLOCK_RAIL)
		{
			// Basic rails can have curves and thus their meta behaves differently from specialized rails:
			switch (a_Meta)
			{
				case E_META_RAIL_ASCEND_XM:    return E_META_RAIL_ASCEND_XP;
				case E_META_RAIL_ASCEND_XP:    return E_META_RAIL_ASCEND_XM;
				case E_META_RAIL_ASCEND_ZM:    return E_META_RAIL_ASCEND_ZM;
				case E_META_RAIL_ASCEND_ZP:    return E_META_RAIL_ASCEND_ZP;
				case E_META_RAIL_CURVED_ZM_XM: return E_META_RAIL_CURVED_ZM_XP;
				case E_META_RAIL_CURVED_ZM_XP: return E_META_RAIL_CURVED_ZM_XM;
				case E_META_RAIL_CURVED_ZP_XM: return E_META_RAIL_CURVED_ZP_XP;
				case E_META_RAIL_CURVED_ZP_XP: return E_META_RAIL_CURVED_ZP_XM;
				case E_META_RAIL_XM_XP:        return E_META_RAIL_XM_XP;
				case E_META_RAIL_ZM_ZP:        return E_META_RAIL_ZM_ZP;
			}
		}
		else
		{
			// Specialized rails don't have curves, instead they use bit 0x08 as a flag
			NIBBLETYPE flag = a_Meta & 0x08;
			switch (a_Meta & 0x07)
			{
				case E_META_RAIL_ASCEND_XM: return flag | E_META_RAIL_ASCEND_XP;
				case E_META_RAIL_ASCEND_XP: return flag | E_META_RAIL_ASCEND_XM;
				case E_META_RAIL_ASCEND_ZM: return flag | E_META_RAIL_ASCEND_ZM;
				case E_META_RAIL_ASCEND_ZP: return flag | E_META_RAIL_ASCEND_ZP;
				case E_META_RAIL_XM_XP:     return flag | E_META_RAIL_XM_XP;
				case E_META_RAIL_ZM_ZP:     return flag | E_META_RAIL_ZM_ZP;
			}
		}
		ASSERT(!"Unknown rail meta");
		return a_Meta;
	}

	// Todo: use override once BlockState is merged - 12xx12
	BlockState MirrorXY(BlockState a_Block) const
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::Rail:
			{
				switch (Rail::Shape(a_Block))
				{
					case Rail::Shape::AscendingNorth: return Rail::Rail(Rail::Shape::AscendingSouth);
					case Rail::Shape::AscendingSouth: return Rail::Rail(Rail::Shape::AscendingNorth);
					case Rail::Shape::SouthEast:      return Rail::Rail(Rail::Shape::NorthEast);
					case Rail::Shape::SouthWest:      return Rail::Rail(Rail::Shape::NorthWest);
					case Rail::Shape::NorthWest:      return Rail::Rail(Rail::Shape::SouthWest);
					case Rail::Shape::NorthEast:      return Rail::Rail(Rail::Shape::SouthEast);
					default: return a_Block;
				}
			}
			case BlockType::ActivatorRail:
			{
				switch (ActivatorRail::Shape(a_Block))
				{
					case ActivatorRail::Shape::AscendingNorth: return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingSouth);
					case ActivatorRail::Shape::AscendingSouth: return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingNorth);
					default: return a_Block;
				}
			}
			case BlockType::DetectorRail:
			{
				switch (DetectorRail::Shape(a_Block))
				{
					case DetectorRail::Shape::AscendingNorth: return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingSouth);
					case DetectorRail::Shape::AscendingSouth: return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingNorth);
					default: return a_Block;
				}
			}
			case BlockType::PoweredRail:
			{
				switch (PoweredRail::Shape(a_Block))
				{
					case PoweredRail::Shape::AscendingNorth: return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingSouth);
					case PoweredRail::Shape::AscendingSouth: return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingNorth);
					default: return a_Block;
				}
			}
			default: return a_Block;
		}
	}

	// Todo: use override once BlockState is merged - 12xx12
	BlockState MirrorYZ(BlockState a_Block) const
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::Rail:
			{
				switch (Rail::Shape(a_Block))
				{
					case Rail::Shape::AscendingEast:  return Rail::Rail(Rail::Shape::AscendingWest);
					case Rail::Shape::AscendingWest:  return Rail::Rail(Rail::Shape::AscendingEast);
					case Rail::Shape::SouthEast:      return Rail::Rail(Rail::Shape::SouthWest);
					case Rail::Shape::SouthWest:      return Rail::Rail(Rail::Shape::SouthEast);
					case Rail::Shape::NorthWest:      return Rail::Rail(Rail::Shape::NorthEast);
					case Rail::Shape::NorthEast:      return Rail::Rail(Rail::Shape::NorthWest);
					default: return a_Block;
				}
			}
			case BlockType::ActivatorRail:
			{
				switch (ActivatorRail::Shape(a_Block))
				{
					case ActivatorRail::Shape::AscendingEast:  return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingWest);
					case ActivatorRail::Shape::AscendingWest:  return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingEast);
					default: return a_Block;
				}
			}
			case BlockType::DetectorRail:
			{
				switch (DetectorRail::Shape(a_Block))
				{
					case DetectorRail::Shape::AscendingEast:  return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingWest);
					case DetectorRail::Shape::AscendingWest:  return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingEast);
					default: return a_Block;
				}
			}
			case BlockType::PoweredRail:
			{
				switch (PoweredRail::Shape(a_Block))
				{
					case PoweredRail::Shape::AscendingEast:  return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingWest);
					case PoweredRail::Shape::AscendingWest:  return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingEast);
					default: return a_Block;
				}
			}
			default: return a_Block;
		}
	}


	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;
