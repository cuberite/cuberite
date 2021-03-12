
#pragma once





enum ENUM_PURE
{
	E_PURE_UPDOWN = 0,
	E_PURE_DOWN = 1,
	E_PURE_NONE = 2
};





class cBlockRailHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

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
		if (!IsBlockRail(a_ChunkInterface.GetBlock(a_Pos)))
		{
			return false;
		}
		auto Self = a_ChunkInterface.GetBlock(a_Pos);
		auto FirstPos = a_Pos;
		auto SecondPos = a_Pos;
		eBlockFace FirstFaceToCheck;
		eBlockFace SecondFaceToCheck;
		ENUM_PURE FirstStrategy;
		ENUM_PURE SecondStrategy;
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
			}
			case BlockType::PoweredRail:
			{
				switch (PoweredRail::Shape(Self))
				{
					ISUNSTABLE(PoweredRail)
				}
			}
			default: return false;
		}
		return
		(
			IsNotConnected(a_ChunkInterface, FirstPos, FirstFaceToCheck, FirstStrategy) ||
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
					}
					default: return false;
				}
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
					}
					default: return false;
				}
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
					}
					default: return false;
				}
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
		/*
		// Bit 0x08 is a flag when a_Meta is in the range 0x00--0x05 and 0x0A--0x0F.
		// Bit 0x08 specifies direction when a_Meta is in the range 0x06-0x09.
		if ((a_Meta < 0x06) || (a_Meta > 0x09))
		{
			//  Save powered rail flag.
			BlockState OtherMeta = a_Meta & 0x08;
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
		*/
	}


	virtual BlockState RotateCW(BlockState a_Block) const override
	{
		/*
		// Bit 0x08 is a flag for value in the range 0x00--0x05 and specifies direction for values withint 0x006--0x09.
		if ((a_Meta < 0x06) || (a_Meta > 0x09))
		{
			//  Save powered rail flag.
			BlockState OtherMeta = a_Meta & 0x08;
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
		*/
	}


	virtual BlockState MirrorXY(BlockState a_Block) const override
	{
		/*
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
			BlockState flag = a_Meta & 0x08;
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
		*/
	}


	virtual BlockState MirrorYZ(BlockState a_Block) const override
	{
		/*
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
			BlockState flag = a_Meta & 0x08;
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
		*/
	}


	virtual ColourID GetMapBaseColourID() const override
	{
		return 0;
	}
} ;
