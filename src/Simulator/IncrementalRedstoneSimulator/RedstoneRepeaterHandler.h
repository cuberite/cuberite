
#pragma once

#include "../../Blocks/BlockRedstoneRepeater.h"





namespace RedstoneRepeaterHandler
{
	static bool IsOn(BlockState a_Block)
	{
		return Block::Repeater::Powered(a_Block);
	}

	/** Returns a pair with first element indicating if the block at the given position is an activated repeater.
	If it is activated, the second element is the repeater metadata. */
	static std::pair<bool, BlockState> IsOnRepeater(cChunk & Chunk, const Vector3i a_Position)
	{
		BlockState State = 0;
		if (!Chunk.UnboundedRelGetBlock(a_Position, State))
		{
			return std::make_pair(false, BlockState(0));
		}

		return std::make_pair(IsOn(State), State);
	}

	/** Determine, from the metadata of a repeater on our left side, if they lock us.
	To test a repeater on our right, simply invert the order of arguments provided.
	"Left" is relative to the direction the repeater output faces, naturally. */
	static bool DoesLhsLockMe(BlockState a_Other, BlockState a_Self)
	{
		auto OtherFace = Block::Repeater::Facing(a_Other);
		auto SelfFace = Block::Repeater::Facing(a_Self);
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
			((OtherFace == eBlockFace::BLOCK_FACE_XP) && (SelfFace == eBlockFace::BLOCK_FACE_ZM)) ||
			((OtherFace == eBlockFace::BLOCK_FACE_ZP) && (SelfFace == eBlockFace::BLOCK_FACE_XP)) ||
			((OtherFace == eBlockFace::BLOCK_FACE_XM) && (SelfFace == eBlockFace::BLOCK_FACE_ZP)) ||
			((OtherFace == eBlockFace::BLOCK_FACE_ZM) && (SelfFace == eBlockFace::BLOCK_FACE_XM));
	}

	/** Determine if a repeater is locked.
	A locked repeater is one with another powered repeater facing them, to their immediate left or right sides.
	"Left" is relative to the direction the repeater output faces, naturally. */
	static bool IsLocked(cChunk & Chunk, const Vector3i a_Position, BlockState a_Block)
	{
		// The left hand side offset. Will be negated to get the rhs offset
		const auto LhsOffset = cBlockRedstoneRepeaterHandler::GetLeftCoordinateOffset(a_Block);

		// Test the block to the left of us
		const auto Lhs = IsOnRepeater(Chunk, LhsOffset + a_Position);
		if (Lhs.first && DoesLhsLockMe(Lhs.second, a_Block))
		{
			return true;
		}

		// Test the right side, flipping the argument order to DoesLhsLockMe
		const auto Rhs = IsOnRepeater(Chunk, -LhsOffset + a_Position);
		return Rhs.first && DoesLhsLockMe(a_Block, Rhs.second);
	}

	static PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, Vector3i a_QueryPosition, BlockState a_QueryBlock, bool IsLinked)
	{
		if (!IsOn(a_Block))
		{
			return 0;
		}

		const auto FrontOffset = cBlockRedstoneRepeaterHandler::GetFrontCoordinateOffset(a_Chunk.GetBlock(a_Position));
		const auto FrontPosition = a_Position + FrontOffset;
		if (a_QueryPosition == FrontPosition)
		{
			return 15;
		}

		return 0;
	}

	static void Update(cChunk & a_Chunk, cChunk & CurrentlyTicking, Vector3i a_Position, BlockState a_Block, const PowerLevel Power)
	{
		LOGREDSTONE("Evaluating loopy the repeater (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		auto & Data = DataForChunk(a_Chunk);
		const auto DelayInfo = Data.GetMechanismDelayInfo(a_Position);

		// If the repeater is locked by another, ignore and forget all power changes:
		if (IsLocked(a_Chunk, a_Position, a_Block))
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
			if (ShouldBeOn != IsOn(a_Block))
			{
				Data.m_MechanismDelays[a_Position] = std::make_pair((Block::Repeater::Delay(a_Block)), ShouldBeOn);
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

		using namespace Block;
		auto NewBlock = Repeater::Repeater(Repeater::Delay(a_Block), Repeater::Facing(a_Block), Repeater::Locked(a_Block), ShouldPowerOn);
		a_Chunk.FastSetBlock(a_Position, NewBlock);
		Data.m_MechanismDelays.erase(a_Position);

		// While sleeping, we ignore any power changes and apply our saved ShouldBeOn when sleep expires
		// Now, we need to recalculate to be aware of any new changes that may e.g. cause a new output change
		// FastSetBlock doesn't wake simulators, so manually update ourselves:
		Update(a_Chunk, CurrentlyTicking, a_Position, NewBlock, Power);

		UpdateAdjustedRelative(a_Chunk, CurrentlyTicking, a_Position, cBlockRedstoneRepeaterHandler::GetFrontCoordinateOffset(a_Block));
	}

	static void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, ForEachSourceCallback & Callback)
	{
		Callback(cBlockRedstoneRepeaterHandler::GetRearCoordinateOffset(a_Block) + a_Position);
	}
};
