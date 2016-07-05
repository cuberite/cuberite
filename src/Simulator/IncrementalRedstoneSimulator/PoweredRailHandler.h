
#pragma once

#include "RedstoneHandler.h"





class cPoweredRailHandler : public cRedstoneHandler
{
	typedef cRedstoneHandler super;
public:

	cPoweredRailHandler(cWorld & a_World) :
		super(a_World)
	{
	}

	static const Vector3i GetPoweredRailAdjacentXZCoordinateOffset(NIBBLETYPE a_Meta)  // Not in cBlockRailHandler since specific to powered rails
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

	virtual unsigned char GetPowerDeliveredToPosition(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, const Vector3i & a_QueryPosition, BLOCKTYPE a_QueryBlockType) override
	{
		UNUSED(a_QueryBlockType);

		auto Offset = GetPoweredRailAdjacentXZCoordinateOffset(a_Meta);
		if (((Offset + a_Position) == a_QueryPosition) || ((-Offset + a_Position) == a_QueryPosition))
		{
			auto Power = GetPowerLevel(a_Position, a_BlockType, a_Meta);
			return (Power <= 7) ? 0 : --Power;
		}
		return 0;
	}

	virtual unsigned char GetPowerLevel(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) override
	{
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		return static_cast<cIncrementalRedstoneSimulator *>(m_World.GetRedstoneSimulator())->GetChunkData()->GetCachedPowerData(a_Position).PowerLevel;
	}

	virtual cVector3iArray Update(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) override
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
				return {};
			}
			case E_BLOCK_ACTIVATOR_RAIL:
			case E_BLOCK_POWERED_RAIL:
			{
				auto Offset = GetPoweredRailAdjacentXZCoordinateOffset(a_Meta);
				if (a_PoweringData != static_cast<cIncrementalRedstoneSimulator *>(m_World.GetRedstoneSimulator())->GetChunkData()->ExchangeUpdateOncePowerData(a_Position, a_PoweringData))
				{
					m_World.SetBlockMeta(a_Position, (a_PoweringData.PowerLevel == 0) ? (a_Meta & 0x07) : (a_Meta | 0x08));
					return cVector3iArray{ { Offset + a_Position }, { -Offset + a_Position } };
				}

				return {};
			}
			default:
			{
				ASSERT(!"Unhandled type of rail in passed to rail handler!");
				return {};
			}
		}
	}

	virtual cVector3iArray GetValidSourcePositions(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) override
	{
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		return GetAdjustedRelatives(a_Position, GetRelativeAdjacents());
	}
};
