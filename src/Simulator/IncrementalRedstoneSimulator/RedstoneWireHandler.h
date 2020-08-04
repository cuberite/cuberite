
#pragma once

#include "RedstoneHandler.h"





class cRedstoneWireHandler final : public cRedstoneHandler
{
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

	template <class OffsetCallback>
	static bool ForTerracingConnectionOffsets(cChunk & a_Chunk, const Vector3i a_Position, OffsetCallback Callback)
	{
		const auto YPTerraceBlock = a_Chunk.GetBlock(a_Position + OffsetYP);
		const bool IsYPTerracingBlocked = cBlockInfo::IsSolid(YPTerraceBlock) && !cBlockInfo::IsTransparent(YPTerraceBlock);

		for (const auto Adjacent : RelativeLaterals)
		{
			// All laterals are counted as terracing, duh
			if (Callback(Adjacent))
			{
				return true;
			}

			if (
				BLOCKTYPE YPBlock;

				// A block above us blocks all YP terracing, so the check is static in the loop
				!IsYPTerracingBlocked &&
				a_Chunk.UnboundedRelGetBlockType(a_Position + Adjacent + OffsetYP, YPBlock) &&
				(YPBlock == E_BLOCK_REDSTONE_WIRE)
			)
			{
				if (Callback(Adjacent + OffsetYP))
				{
					return true;
				}
			}

			if (
				BLOCKTYPE YMTerraceBlock, YMDiagonalBlock;

				// IsYMTerracingBlocked (i.e. check block above lower terracing position, a.k.a. just the plain adjacent)
				a_Chunk.UnboundedRelGetBlockType(a_Position + Adjacent, YMTerraceBlock) &&
				(!cBlockInfo::IsSolid(YMTerraceBlock) || cBlockInfo::IsTransparent(YMTerraceBlock)) &&

				a_Chunk.UnboundedRelGetBlockType(a_Position + Adjacent + OffsetYM, YMDiagonalBlock) &&
				(YMDiagonalBlock == E_BLOCK_REDSTONE_WIRE)
			)
			{
				if (Callback(Adjacent + OffsetYM))
				{
					return true;
				}
			}
		}

		return false;
	}

	virtual unsigned char GetPowerDeliveredToPosition(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType) const override
	{
		if (a_QueryPosition == (a_Position + OffsetYP))
		{
			// Wires do not power things above them
			return 0;
		}

		if (a_QueryBlockType == E_BLOCK_REDSTONE_WIRE)
		{
			// For mechanisms, wire of power one will still power them
			// But for wire-to-wire connections, power level decreases by 1
			return (a_Meta != 0) ? --a_Meta : a_Meta;
		}

		// Wires always deliver power to the block underneath, and any directly connecting mechanisms
		if (
			NIBBLETYPE QueryMeta;

			(a_QueryPosition == (a_Position + OffsetYM)) ||
			(a_Chunk.UnboundedRelGetBlockMeta(a_QueryPosition, QueryMeta) && IsDirectlyConnectingMechanism(a_QueryBlockType, QueryMeta, a_QueryPosition - a_Position))
		)
		{
			return a_Meta;
		}

		/*
		Okay, we do not directly connect to the wire.
		If there are no DC mechanisms at all, the wire powers all laterals. Great, we fall out the loop.
		If there is one DC mechanism, the wire "goes straight" along the axis of the wire and mechanism.
		The only possible way for us to be powered is for us to be on the opposite end, with the wire pointing towards us.
		If there is more than one DC, no non-DCs are powered.
		*/

		Vector3i PotentialOffset;
		bool FoundOneBorderingMechanism = false;

		if (
			ForTerracingConnectionOffsets(a_Chunk, a_Position, [&a_Chunk, a_Position, &FoundOneBorderingMechanism, &PotentialOffset](const Vector3i Offset)
			{
				BLOCKTYPE Block;
				NIBBLETYPE Meta;

				if (
					!a_Chunk.UnboundedRelGetBlock(Offset + a_Position, Block, Meta) ||
					!IsDirectlyConnectingMechanism(Block, Meta, Offset)
				)
				{
					return false;
				}

				if (FoundOneBorderingMechanism)
				{
					// Case 3
					return true;
				}

				// Potential case 2
				FoundOneBorderingMechanism = true;
				PotentialOffset = { -Offset.x, 0, -Offset.z };

				return false;
			})
		)
		{
			// Case 3
			return 0;
		}

		if (FoundOneBorderingMechanism && (a_QueryPosition != (a_Position + PotentialOffset)))
		{
			// Case 2 fail
			return 0;
		}

		// Case 1
		// Case 2 success

		return a_Meta;
	}

	virtual void Update(cChunk & a_Chunk, cChunk & CurrentlyTicking, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) const override
	{
		// LOGD("Evaluating dusty the wire (%d %d %d) %i", a_Position.x, a_Position.y, a_Position.z, a_PoweringData.PowerLevel);

		if (a_Meta != a_PoweringData.PowerLevel)
		{
			a_Chunk.SetMeta(a_Position, a_PoweringData.PowerLevel);

			// Notify block below us to update:
			UpdateAdjustedRelative(a_Chunk, CurrentlyTicking, a_Position + OffsetYM);

			// Notify all terracing positions:
			ForTerracingConnectionOffsets(a_Chunk, a_Position, [&a_Chunk, &CurrentlyTicking, a_Position](const Vector3i Offset)
			{
				UpdateAdjustedRelative(a_Chunk, CurrentlyTicking, a_Position + Offset);
				return false;
			});
		}
	}

	virtual void ForValidSourcePositions(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, SourceCallback Callback) const override
	{
		UNUSED(a_Chunk);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);

		Callback(a_Position + OffsetYP);
		Callback(a_Position + OffsetYM);

		ForTerracingConnectionOffsets(a_Chunk, a_Position, [&Callback, a_Position](const Vector3i Offset)
		{
			Callback(a_Position + Offset);
			return false;
		});
	}
};
