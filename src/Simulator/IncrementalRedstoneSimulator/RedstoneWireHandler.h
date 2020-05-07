
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
		auto RelativePositions = GetRelativeLaterals();
		const auto YPTerraceBlock = a_World.GetBlock(a_Position + OffsetYP());
		const bool IsYPTerracingBlocked = cBlockInfo::IsSolid(YPTerraceBlock) && !cBlockInfo::IsTransparent(YPTerraceBlock);

		for (const auto & Adjacent : GetRelativeLaterals())
		{
			if (
				// A block above us blocks all YP terracing, so the check is static in the loop
				!IsYPTerracingBlocked &&
				(a_World.GetBlock(a_Position + Adjacent + OffsetYP()) == E_BLOCK_REDSTONE_WIRE)
			)
			{
				RelativePositions.emplace_back(Adjacent + OffsetYP());
			}

			const auto YMTerraceBlock = a_World.GetBlock(a_Position + Adjacent);
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

		// Wires always deliver power to the block underneath, and any directly connecting mechanisms
		if (
			(a_QueryPosition != (a_Position + OffsetYM())) &&
			!IsDirectlyConnectingMechanism(a_QueryBlockType, a_World.GetBlockMeta(a_QueryPosition), a_QueryPosition - a_Position)
		)
		{
			/*
			Okay, we do not directly connect to the wire.
			If there are no DC mechanisms at all, the wire powers all laterals. Great, we fall out the loop.
			If there is one DC mechanism, the wire "goes straight" along the axis of the wire and mechanism.
			The only possible way for us to be powered is for us to be on the opposite end, with the wire pointing towards us.
			If there is more than one DC, no non-DCs are powered.
			*/

			Vector3i PotentialOffset;
			bool FoundOneBorderingMechanism = false;

			for (const auto & Offset : GetTerracingConnectionOffsets(a_World, a_Position))
			{
				BLOCKTYPE Block;
				NIBBLETYPE Meta;

				if (
					!a_World.GetBlockTypeMeta(Offset + a_Position, Block, Meta) ||
					!IsDirectlyConnectingMechanism(Block, Meta, Offset)
				)
				{
					continue;
				}

				if (FoundOneBorderingMechanism)
				{
					// Case 3
					return 0;
				}

				// Potential case 2
				FoundOneBorderingMechanism = true;
				PotentialOffset = { -Offset.x, 0, -Offset.z };
			}

			if (FoundOneBorderingMechanism && (a_QueryPosition != (a_Position + PotentialOffset)))
			{
				// Case 2 fail
				return 0;
			}

			// Case 1
			// Case 2 success
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
