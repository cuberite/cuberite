
#pragma once

#include "RedstoneHandler.h"





class cPoweredRailHandler final : public cRedstoneHandler
{
public:

	static Vector3i GetPoweredRailAdjacentXZCoordinateOffset(NIBBLETYPE a_Meta)  // Not in cBlockRailHandler since specific to powered rails
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

	virtual unsigned char GetPowerDeliveredToPosition(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType) const override
	{
		UNUSED(a_QueryBlockType);

		auto Offset = GetPoweredRailAdjacentXZCoordinateOffset(a_Meta);
		if (((Offset + a_Position) == a_QueryPosition) || ((-Offset + a_Position) == a_QueryPosition))
		{
			auto Power = DataForChunk(a_Chunk).GetCachedPowerData(a_Position).PowerLevel;
			return (Power <= 7) ? 0 : --Power;
		}
		return 0;
	}

	virtual void Update(cChunk & a_Chunk, cChunk & CurrentlyTickingChunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) const override
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
				auto Offset = GetPoweredRailAdjacentXZCoordinateOffset(a_Meta);
				if (a_PoweringData != DataForChunk(a_Chunk).ExchangeUpdateOncePowerData(a_Position, a_PoweringData))
				{
					a_Chunk.SetMeta(a_Position, (a_PoweringData.PowerLevel == 0) ? (a_Meta & 0x07) : (a_Meta | 0x08));

					UpdateAdjustedRelatives(a_Chunk, CurrentlyTickingChunk, a_Position + Offset);
					UpdateAdjustedRelatives(a_Chunk, CurrentlyTickingChunk, a_Position + -Offset);
				}

				return;
			}
			default:
			{
				ASSERT(!"Unhandled type of rail in passed to rail handler!");
			}
		}
	}

	virtual void ForValidSourcePositions(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, SourceCallback Callback) const override
	{
		UNUSED(a_Chunk);
		UNUSED(a_Meta);

		if ((a_BlockType == E_BLOCK_POWERED_RAIL) || (a_BlockType == E_BLOCK_ACTIVATOR_RAIL))
		{
			InvokeForAdjustedRelatives(Callback, a_Position, RelativeAdjacents);
		}
	}
};
