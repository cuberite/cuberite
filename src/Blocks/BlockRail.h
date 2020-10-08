
#pragma once





enum ENUM_PURE
{
	E_PURE_UPDOWN = 0,
	E_PURE_DOWN = 1,
	E_PURE_NONE = 2
};





class cBlockRailHandler final :
	public cClearMetaOnDrop<cBlockHandler>
{
	using Super = cClearMetaOnDrop<cBlockHandler>;

public:

	using Super::Super;

private:

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface,
		cPlayer & a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) const override
	{
		a_BlockType = m_BlockType;
		a_BlockMeta = FindMeta(a_ChunkInterface, a_PlacedBlockPos);
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
		const auto NewMeta = FindMeta(a_ChunkInterface, a_BlockPos);
		if ((Meta != NewMeta) && IsUnstable(a_ChunkInterface, a_BlockPos))
		{
			a_ChunkInterface.FastSetBlock(a_BlockPos, m_BlockType, (m_BlockType == E_BLOCK_RAIL) ? NewMeta : NewMeta | (Meta & 0x08));
		}

		Super::OnNeighborChanged(a_ChunkInterface, a_BlockPos, a_WhichNeighbor);
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

		NIBBLETYPE Meta = a_Chunk.GetMeta(a_RelPos);
		switch (Meta)
		{
			case E_META_RAIL_ASCEND_XP:
			case E_META_RAIL_ASCEND_XM:
			case E_META_RAIL_ASCEND_ZM:
			case E_META_RAIL_ASCEND_ZP:
			{
				// Mapping between the meta and the neighbors that need checking
				Meta -= E_META_RAIL_ASCEND_XP;  // Base index at zero
				static const Vector3i Coords[] =
				{
					{ 1, 0,  0},  // east,  XP
					{-1, 0,  0},  // west,  XM
					{ 0, 0, -1},  // north, ZM
					{ 0, 0,  1},  // south, ZP
				} ;
				BLOCKTYPE  BlockType;
				NIBBLETYPE BlockMeta;
				if (!a_Chunk.UnboundedRelGetBlock(a_RelPos + Coords[Meta], BlockType, BlockMeta))
				{
					// Too close to the edge, cannot simulate
					return true;
				}
				return cBlockInfo::FullyOccupiesVoxel(BlockType);
			}
		}
		return true;
	}





	NIBBLETYPE FindMeta(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos) const
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
		return m_BlockType == E_BLOCK_RAIL;
	}


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


	static bool IsNotConnected(cChunkInterface  & a_ChunkInterface, Vector3i a_Pos, eBlockFace a_BlockFace, char a_Pure = 0)
	{
		AddFaceDirection(a_Pos.x, a_Pos.y, a_Pos.z, a_BlockFace, false);
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


	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;
