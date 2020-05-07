
#pragma once

#include "RedstoneHandler.h"





class cRedstoneWireHandler:
	public cRedstoneHandler
{
	using Super = cRedstoneHandler;

public:

	inline static bool IsDirectlyConnectingMechanism(BLOCKTYPE a_Block, NIBBLETYPE a_BlockMeta, const Vector3i a_Offset)
	{
		switch (a_Block)
		{
			case E_BLOCK_REDSTONE_REPEATER_ON:
			case E_BLOCK_REDSTONE_REPEATER_OFF:
			{
				a_BlockMeta &= E_META_REDSTONE_REPEATER_FACING_MASK;
				if ((a_BlockMeta == E_META_REDSTONE_REPEATER_FACING_XP) || (a_BlockMeta == E_META_REDSTONE_REPEATER_FACING_XM))
				{
					// Wire connects to repeater if repeater is aligned along X
					// and wire is in front or behind it (#4639)
					return a_Offset.x != 0;
				}

				return a_Offset.z != 0;
			}
			case E_BLOCK_ACTIVE_COMPARATOR:
			case E_BLOCK_INACTIVE_COMPARATOR:
			case E_BLOCK_REDSTONE_TORCH_OFF:
			case E_BLOCK_REDSTONE_TORCH_ON:
			case E_BLOCK_REDSTONE_WIRE: return true;
			default: return false;
		}
	}

	cVector3iArray GetTerracingConnectionOffsets(cWorld & a_World, const Vector3i a_Position) const
	{
		cVector3iArray RelativePositions;
		auto YPTerraceBlock = a_World.GetBlock(a_Position + OffsetYP());
		bool IsYPTerracingBlocked = cBlockInfo::IsSolid(YPTerraceBlock) && !cBlockInfo::IsTransparent(YPTerraceBlock);

		for (const auto & Adjacent : GetRelativeLaterals())
		{
			if (
				!IsYPTerracingBlocked &&
				(a_World.GetBlock(a_Position + Adjacent + OffsetYP()) == E_BLOCK_REDSTONE_WIRE)
				)
			{
				RelativePositions.emplace_back(Adjacent + OffsetYP());
			}
			auto YMTerraceBlock = a_World.GetBlock(a_Position + Adjacent);
			if (
				// IsYMTerracingBlocked (i.e. check block above lower terracing position, a.k.a. just the plain adjacent)
				(!cBlockInfo::IsSolid(YMTerraceBlock) || cBlockInfo::IsTransparent(YMTerraceBlock)) &&
				(a_World.GetBlock(a_Position + Adjacent + OffsetYM()) == E_BLOCK_REDSTONE_WIRE)
			)
			{
				RelativePositions.emplace_back(Adjacent + OffsetYM());
			}
		}

		return RelativePositions;
	}

	virtual unsigned char GetPowerDeliveredToPosition(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType) const override
	{
		if (a_QueryPosition == (a_Position + OffsetYP()))
		{
			// Wires do not power things above them
			return 0;
		}

		if (a_QueryBlockType != E_BLOCK_REDSTONE_WIRE)
		{
			// For mechanisms, wire of power one will still power them
			a_Meta++;
		}

		if ((a_QueryPosition != (a_Position + OffsetYM())) && !IsDirectlyConnectingMechanism( a_QueryBlockType, a_Meta, a_QueryPosition - a_Position))
		{
			Vector3i PotentialOffset;
			bool FoundOneBorderingMechanism = false;

			for (const auto & Offset : StaticAppend(GetRelativeLaterals(), GetTerracingConnectionOffsets(a_World, a_Position)))
			{
				if (IsDirectlyConnectingMechanism(a_World.GetBlock(Offset + a_Position), a_Meta, Offset))
				{
					if (FoundOneBorderingMechanism)
					{
						return 0;
					}
					else
					{
						FoundOneBorderingMechanism = true;
						PotentialOffset = { -Offset.x, 0, -Offset.z };
					}
				}
			}

			if (FoundOneBorderingMechanism && (a_QueryPosition != (a_Position + PotentialOffset)))
			{
				return 0;
			}
		}

		return (a_Meta != 0) ? --a_Meta : a_Meta;
	}

	virtual unsigned char GetPowerLevel(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_World);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		return a_Meta;
	}

	virtual cVector3iArray Update(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) const override
	{
		UNUSED(a_BlockType);
		// LOGD("Evaluating dusty the wire (%d %d %d) %i", a_Position.x, a_Position.y, a_Position.z, a_PoweringData.PowerLevel);

		if (a_Meta != a_PoweringData.PowerLevel)
		{
			a_World.SetBlockMeta(a_Position, a_PoweringData.PowerLevel);
			return GetAdjustedRelatives(a_Position, StaticAppend(StaticAppend(GetRelativeLaterals(), GetTerracingConnectionOffsets(a_World, a_Position)), cVector3iArray{ OffsetYM() }));
		}

		return {};
	}

	virtual cVector3iArray GetValidSourcePositions(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_World);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);

		return GetAdjustedRelatives(a_Position, StaticAppend(GetRelativeAdjacents(), GetTerracingConnectionOffsets(a_World, a_Position)));
	}
};
