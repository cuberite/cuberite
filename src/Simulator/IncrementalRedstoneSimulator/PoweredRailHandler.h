
#pragma once





namespace PoweredRailHandler
{
	/** Get the offset along which the rail faces.
	Not in cBlockRailHandler since specific to powered rails. */
	inline Vector3i GetPoweredRailAdjacentXZCoordinateOffset(NIBBLETYPE a_Meta)
	{
		switch (a_Meta & 0x7)
		{
			case E_META_RAIL_ZM_ZP: return { 0, 0, 1 };
			case E_META_RAIL_XM_XP: return { 1, 0, 0 };
			case E_META_RAIL_ASCEND_XP: return { 1, 1, 0 };
			case E_META_RAIL_ASCEND_XM: return { 1, 1, 0 };
			case E_META_RAIL_ASCEND_ZM: return { 0, 1, 1 };
			case E_META_RAIL_ASCEND_ZP: return { 0, 1, 1 };
			default:
			{
				ASSERT(!"Impossible rail meta! wat wat wat");
				return { 0, 0, 0 };
			}
		}
	}

	inline PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType, bool IsLinked)
	{
		UNUSED(a_QueryBlockType);

		const auto Meta = a_Chunk.GetMeta(a_Position);
		const auto Offset = GetPoweredRailAdjacentXZCoordinateOffset(Meta);
		if (((Offset + a_Position) == a_QueryPosition) || ((-Offset + a_Position) == a_QueryPosition))
		{
			const auto Power = DataForChunk(a_Chunk).GetCachedPowerData(a_Position);
			return (Power <= 7) ? 0 : (Power - 1);
		}
		return 0;
	}

	inline void Update(cChunk & a_Chunk, cChunk & CurrentlyTickingChunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, const PowerLevel Power)
	{
		// LOGD("Evaluating tracky the rail (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		switch (a_BlockType)
		{
			case E_BLOCK_DETECTOR_RAIL:
			{
				/*
				if ((m_Chunk->GetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ) & 0x08) == 0x08)
				{
					SetAllDirsAsPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, a_MyType);
				}
				*/
				return;
			}
			case E_BLOCK_ACTIVATOR_RAIL:
			case E_BLOCK_POWERED_RAIL:
			{
				const auto Offset = GetPoweredRailAdjacentXZCoordinateOffset(a_Meta);
				if (Power != DataForChunk(a_Chunk).ExchangeUpdateOncePowerData(a_Position, Power))
				{
					a_Chunk.SetMeta(a_Position, (Power == 0) ? (a_Meta & 0x07) : (a_Meta | 0x08));

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

	inline void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Meta);

		if ((a_BlockType == E_BLOCK_POWERED_RAIL) || (a_BlockType == E_BLOCK_ACTIVATOR_RAIL))
		{
			InvokeForAdjustedRelatives(Callback, a_Position, RelativeAdjacents);
		}
	}
};
