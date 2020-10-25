
#pragma once

#include "../../Blocks/BlockRedstoneRepeater.h"





namespace RedstoneRepeaterHandler
{
	inline bool IsOn(BLOCKTYPE a_Block)
	{
		return (a_Block == E_BLOCK_REDSTONE_REPEATER_ON);
	}

	/** Returns a pair with first element indicating if the block at the given position is an activated repeater.
	If it is activated, the second element is the repeater metadata. */
	inline std::pair<bool, NIBBLETYPE> IsOnRepeater(cChunk & Chunk, const Vector3i a_Position)
	{
		BLOCKTYPE Type;
		NIBBLETYPE Meta;

		if (!Chunk.UnboundedRelGetBlock(a_Position, Type, Meta))
		{
			return std::make_pair(false, 0);
		}

		return std::make_pair(IsOn(Type), Meta);
	}

	/** Determine, from the metadata of a repeater on our left side, if they lock us.
	To test a repeater on our right, simply invert the order of arguments provided.
	"Left" is relative to the direction the repeater output faces, naturally. */
	inline bool DoesLhsLockMe(NIBBLETYPE a_MetaLhs, NIBBLETYPE a_MyMeta)
	{
		// Get the direction bits
		a_MetaLhs &= E_META_REDSTONE_REPEATER_FACING_MASK;
		a_MyMeta &= E_META_REDSTONE_REPEATER_FACING_MASK;

		/*
		Check for a valid locking configuration, where they are perpendicular and one snuggles into the other.

		Order of comparisons:
			XP >^ ZM
			ZP |_ XP
			XM <| ZP
			ZP ^< xM

		Key:
			^ Facing up
			_ Facing right
			| Facing down
			< Facing left
		*/
		return
			((a_MetaLhs == E_META_REDSTONE_REPEATER_FACING_XP) && (a_MyMeta == E_META_REDSTONE_REPEATER_FACING_ZM)) ||
			((a_MetaLhs == E_META_REDSTONE_REPEATER_FACING_ZP) && (a_MyMeta == E_META_REDSTONE_REPEATER_FACING_XP)) ||
			((a_MetaLhs == E_META_REDSTONE_REPEATER_FACING_XM) && (a_MyMeta == E_META_REDSTONE_REPEATER_FACING_ZP)) ||
			((a_MetaLhs == E_META_REDSTONE_REPEATER_FACING_ZM) && (a_MyMeta == E_META_REDSTONE_REPEATER_FACING_XM))
		;
	}

	/** Determine if a repeater is locked.
	A locked repeater is one with another powered repeater facing them, to their immediate left or right sides.
	"Left" is relative to the direction the repeater output faces, naturally. */
	inline bool IsLocked(cChunk & Chunk, const Vector3i a_Position, const NIBBLETYPE a_Meta)
	{
		// The left hand side offset. Will be negated to get the rhs offset
		const auto LhsOffset = cBlockRedstoneRepeaterHandler::GetLeftCoordinateOffset(a_Meta);

		// Test the block to the left of us
		const auto Lhs = IsOnRepeater(Chunk, LhsOffset + a_Position);
		if (Lhs.first && DoesLhsLockMe(Lhs.second, a_Meta))
		{
			return true;
		}

		// Test the right side, flipping the argument order to DoesLhsLockMe
		const auto Rhs = IsOnRepeater(Chunk, -LhsOffset + a_Position);
		return Rhs.first && DoesLhsLockMe(a_Meta, Rhs.second);
	}

	inline PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType, bool IsLinked)
	{
		if (!IsOn(a_BlockType))
		{
			return 0;
		}

		const auto FrontOffset = cBlockRedstoneRepeaterHandler::GetFrontCoordinateOffset(a_Chunk.GetMeta(a_Position));
		const auto FrontPosition = a_Position + FrontOffset;
		if (a_QueryPosition == FrontPosition)
		{
			return 15;
		}

		return 0;
	}

	inline void Update(cChunk & a_Chunk, cChunk & CurrentlyTicking, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, const PowerLevel Power)
	{
		// LOGD("Evaluating loopy the repeater (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		auto & Data = DataForChunk(a_Chunk);
		const auto DelayInfo = Data.GetMechanismDelayInfo(a_Position);

		// If the repeater is locked by another, ignore and forget all power changes:
		if (IsLocked(a_Chunk, a_Position, a_Meta))
		{
			if (DelayInfo != nullptr)
			{
				Data.m_MechanismDelays.erase(a_Position);
			}

			return;
		}

		if (DelayInfo == nullptr)
		{
			bool ShouldBeOn = (Power != 0);
			if (ShouldBeOn != IsOn(a_BlockType))
			{
				Data.m_MechanismDelays[a_Position] = std::make_pair((((a_Meta & 0xC) >> 0x2) + 1), ShouldBeOn);
			}

			return;
		}

		int DelayTicks;
		bool ShouldPowerOn;
		std::tie(DelayTicks, ShouldPowerOn) = *DelayInfo;

		if (DelayTicks != 0)
		{
			return;
		}

		const auto NewType = ShouldPowerOn ? E_BLOCK_REDSTONE_REPEATER_ON : E_BLOCK_REDSTONE_REPEATER_OFF;
		a_Chunk.FastSetBlock(a_Position, NewType, a_Meta);
		Data.m_MechanismDelays.erase(a_Position);

		// While sleeping, we ignore any power changes and apply our saved ShouldBeOn when sleep expires
		// Now, we need to recalculate to be aware of any new changes that may e.g. cause a new output change
		// FastSetBlock doesn't wake simulators, so manually update ourselves:
		Update(a_Chunk, CurrentlyTicking, a_Position, NewType, a_Meta, Power);

		UpdateAdjustedRelative(a_Chunk, CurrentlyTicking, a_Position, cBlockRedstoneRepeaterHandler::GetFrontCoordinateOffset(a_Meta));
	}

	inline void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, ForEachSourceCallback & Callback)
	{
		Callback(cBlockRedstoneRepeaterHandler::GetRearCoordinateOffset(a_Meta) + a_Position);
	}
};
