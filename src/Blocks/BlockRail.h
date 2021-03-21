
#include "BlockHandler.h"

#pragma once





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
			case BlockType::ActivatorRail: a_ChunkInterface.FastSetBlock(a_Position, ActivatorRail::ActivatorRail(a_Powered, ActivatorRail::Shape(Self))); break;
			case BlockType::DetectorRail:  a_ChunkInterface.FastSetBlock(a_Position, DetectorRail::DetectorRail(a_Powered, DetectorRail::Shape(Self))); break;
			case BlockType::PoweredRail:   a_ChunkInterface.FastSetBlock(a_Position, PoweredRail::PoweredRail(a_Powered, PoweredRail::Shape(Self))); break;
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

private:

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface,
		cPlayer & a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BlockState & a_Block
	) const override
	{
		a_Block = FindBlock(a_ChunkInterface, a_PlacedBlockPos);
		return a_Player.GetWorld()->DoWithChunkAt(a_PlacedBlockPos,
			[this, a_PlacedBlockPos, &a_ChunkInterface](cChunk & a_Chunk)
			{
				auto RelPos = cChunkDef::AbsoluteToRelative(a_PlacedBlockPos);
				return CanBeAt(a_ChunkInterface, RelPos, a_Chunk);
			}
		);
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





	virtual void OnNeighborChanged(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos, eBlockFace a_WhichNeighbor) const override
	{
		const auto OldBlock = a_ChunkInterface.GetBlock(a_BlockPos);
		const auto NewBlock = FindBlock(a_ChunkInterface, a_BlockPos);
		if ((OldBlock != NewBlock) && IsUnstable(a_ChunkInterface, a_BlockPos))
		{
			a_ChunkInterface.FastSetBlock(a_BlockPos, NewBlock);
		}

		Super::OnNeighborChanged(a_ChunkInterface, a_BlockPos, a_WhichNeighbor);
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

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		if (a_RelPos.y <= 0)
		{
			return false;
		}
		if (!cBlockInfo::FullyOccupiesVoxel(a_Chunk.GetBlock(a_RelPos.addedY(-1))))
		{
			return false;
		}

		using namespace Block;

		auto Self = a_Chunk.GetBlock(a_RelPos);
		Vector3i Offset;
		switch (Self.Type())
		{
			case BlockType::Rail:          GETOFFSET(Rail)
			case BlockType::ActivatorRail: GETOFFSET(ActivatorRail)
			case BlockType::DetectorRail:  GETOFFSET(DetectorRail)
			case BlockType::PoweredRail:   GETOFFSET(PoweredRail)
			default: break;
		}

		if (Offset != Vector3i(0, 0, 0))
		{
			BlockState BelowBlock = 0;
			if (!a_Chunk.UnboundedRelGetBlock(a_RelPos + Offset, BelowBlock))
			{
				// Too close to the edge, cannot simulate
				return true;
			}
			return cBlockInfo::FullyOccupiesVoxel(BelowBlock);
		}
		return true;
	}





	BlockState FindBlock(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos) const
	{
		BlockState Meta = 0;
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
			if (Neighbors[3] && Neighbors[0] && CanThisRailCurve())
			{
				return E_META_RAIL_CURVED_ZP_XP;
			}
			else if (Neighbors[3] && Neighbors[1] && CanThisRailCurve())
			{
				return E_META_RAIL_CURVED_ZP_XM;
			}
			else if (Neighbors[2] && Neighbors[0] && CanThisRailCurve())
			{
				return E_META_RAIL_CURVED_ZM_XP;
			}
			else if (Neighbors[2] && Neighbors[1] && CanThisRailCurve())
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

			if (CanThisRailCurve())
			{
				ASSERT(!"Weird neighbor count");
			}
		}
		return Meta;
	}


	bool CanThisRailCurve(void) const
	{
		return m_BlockType == BlockType::Rail;
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
		AddFaceDirection(a_Pos.x, a_Pos.y, a_Pos.z, a_BlockFace, false);
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


	virtual BlockState RotateCCW(BlockState a_Block) const override
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
			}
			default: return a_Block;
		}
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


	virtual BlockState MirrorXY(BlockState a_Block) const override
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
				break;
			}
			case BlockType::ActivatorRail:
			{
				switch (ActivatorRail::Shape(a_Block))
				{
					case ActivatorRail::Shape::AscendingNorth: return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingSouth);
					case ActivatorRail::Shape::AscendingSouth: return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingNorth);
					default: return a_Block;
				}
				break;
			}
			case BlockType::DetectorRail:
			{
				switch (DetectorRail::Shape(a_Block))
				{
					case DetectorRail::Shape::AscendingNorth: return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingSouth);
					case DetectorRail::Shape::AscendingSouth: return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingNorth);
					default: return a_Block;
				}
				break;
			}
			case BlockType::PoweredRail:
			{
				switch (PoweredRail::Shape(a_Block))
				{
					case PoweredRail::Shape::AscendingNorth: return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingSouth);
					case PoweredRail::Shape::AscendingSouth: return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingNorth);
					default: return a_Block;
				}
				break;
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
					case Rail::Shape::AscendingEast:  return Rail::Rail(Rail::Shape::AscendingWest);
					case Rail::Shape::AscendingWest:  return Rail::Rail(Rail::Shape::AscendingEast);
					case Rail::Shape::SouthEast:      return Rail::Rail(Rail::Shape::SouthWest);
					case Rail::Shape::SouthWest:      return Rail::Rail(Rail::Shape::SouthEast);
					case Rail::Shape::NorthWest:      return Rail::Rail(Rail::Shape::NorthEast);
					case Rail::Shape::NorthEast:      return Rail::Rail(Rail::Shape::NorthWest);
					default: return a_Block;
				}
				break;
			}
			case BlockType::ActivatorRail:
			{
				switch (ActivatorRail::Shape(a_Block))
				{
					case ActivatorRail::Shape::AscendingEast:  return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingWest);
					case ActivatorRail::Shape::AscendingWest:  return ActivatorRail::ActivatorRail(ActivatorRail::Powered(a_Block), ActivatorRail::Shape::AscendingEast);
					default: return a_Block;
				}
				break;
			}
			case BlockType::DetectorRail:
			{
				switch (DetectorRail::Shape(a_Block))
				{
					case DetectorRail::Shape::AscendingEast:  return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingWest);
					case DetectorRail::Shape::AscendingWest:  return DetectorRail::DetectorRail(DetectorRail::Powered(a_Block), DetectorRail::Shape::AscendingEast);
					default: return a_Block;
				}
				break;
			}
			case BlockType::PoweredRail:
			{
				switch (PoweredRail::Shape(a_Block))
				{
					case PoweredRail::Shape::AscendingEast:  return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingWest);
					case PoweredRail::Shape::AscendingWest:  return PoweredRail::PoweredRail(PoweredRail::Powered(a_Block), PoweredRail::Shape::AscendingEast);
					default: return a_Block;
				}
				break;
			}
			default: return a_Block;
		}
	}


	virtual ColourID GetMapBaseColourID() const override
	{
		return 0;
	}
} ;
