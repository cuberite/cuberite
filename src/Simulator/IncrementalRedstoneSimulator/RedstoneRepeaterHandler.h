
#pragma once

#include "RedstoneHandler.h"
#include "../../Blocks/BlockRedstoneRepeater.h"





class cRedstoneRepeaterHandler:
	public cRedstoneHandler
{
	using Super = cRedstoneHandler;

public:

	virtual unsigned char GetPowerDeliveredToPosition(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType) const override
	{
		return (
			(a_QueryPosition == (a_Position + cBlockRedstoneRepeaterHandler::GetFrontCoordinateOffset(a_Meta))) ?
			GetPowerLevel(a_World, a_Position, a_BlockType, a_Meta) : 0
		);
	}

	virtual unsigned char GetPowerLevel(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_World);
		UNUSED(a_Position);
		UNUSED(a_Meta);
		return IsOn(a_BlockType) ? 15 : 0;
	}

	virtual cVector3iArray Update(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) const override
	{
		// LOGD("Evaluating loopy the repeater (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);
		auto Data = static_cast<cIncrementalRedstoneSimulator *>(a_World.GetRedstoneSimulator())->GetChunkData();
		auto DelayInfo = Data->GetMechanismDelayInfo(a_Position);

		// If the repeater is locked by another, ignore and forget all power changes:
		if (IsLocked(a_World, a_Position, a_Meta))
		{
			if (DelayInfo != nullptr)
			{
				Data->m_MechanismDelays.erase(a_Position);
			}

			return {};
		}

		if (DelayInfo == nullptr)
		{
			bool ShouldBeOn = (a_PoweringData.PowerLevel != 0);
			if (ShouldBeOn != IsOn(a_BlockType))
			{
				Data->m_MechanismDelays[a_Position] = std::make_pair((((a_Meta & 0xC) >> 0x2) + 1), ShouldBeOn);
			}
		}
		else
		{
			int DelayTicks;
			bool ShouldPowerOn;
			std::tie(DelayTicks, ShouldPowerOn) = *DelayInfo;

			if (DelayTicks == 0)
			{
				a_World.SetBlock(a_Position.x, a_Position.y, a_Position.z, ShouldPowerOn ? E_BLOCK_REDSTONE_REPEATER_ON : E_BLOCK_REDSTONE_REPEATER_OFF, a_Meta);
				Data->m_MechanismDelays.erase(a_Position);
				return cVector3iArray{ cBlockRedstoneRepeaterHandler::GetFrontCoordinateOffset(a_Meta) + a_Position };
			}
		}

		return {};
	}

	virtual cVector3iArray GetValidSourcePositions(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const override
	{
		return { cBlockRedstoneRepeaterHandler::GetRearCoordinateOffset(a_Meta) + a_Position };
	}

private:

	inline static bool IsOn(BLOCKTYPE a_Block)
	{
		return (a_Block == E_BLOCK_REDSTONE_REPEATER_ON);
	}

	/** Returns a pair with first element indicating if the block at the given position is an activated repeater.
	If it is activated, the second element is the repeater metadata. */
	static std::pair<bool, NIBBLETYPE> IsOnRepeater(cWorld & a_World, const Vector3i a_Position)
	{
		BLOCKTYPE Type;
		NIBBLETYPE Meta;

		if (!a_World.GetBlockTypeMeta(a_Position, Type, Meta))
		{
			return std::make_pair(false, 0);
		}

		return std::make_pair(IsOn(Type), Meta);
	}

	/** Determine if a repeater is locked.
	A locked repeater is one with another powered repeater facing them, to their immediate left or right sides.
	"Left" is relative to the direction the repeater output faces, naturally. */
	inline static bool IsLocked(cWorld & a_World, const Vector3i a_Position, const NIBBLETYPE a_Meta)
	{
		// The left hand side offset. Will be negated to get the rhs offset
		const auto LhsOffset = cBlockRedstoneRepeaterHandler::GetLeftCoordinateOffset(a_Meta);

		// Test the block to the left of us
		const auto Lhs = IsOnRepeater(a_World, LhsOffset + a_Position);
		if (Lhs.first && DoesLhsLockMe(Lhs.second, a_Meta))
		{
			return true;
		}

		// Test the right side, flipping the argument order to DoesLhsLockMe
		const auto Rhs = IsOnRepeater(a_World, -LhsOffset + a_Position);
		return Rhs.first && DoesLhsLockMe(a_Meta, Rhs.second);
	}

	/** Determine, from the metadata of a repeater on our left side, if they lock us.
	To test a repeater on our right, simply invert the order of arguments provided.
	"Left" is relative to the direction the repeater output faces, naturally. */
	static bool DoesLhsLockMe(NIBBLETYPE a_MetaLhs, NIBBLETYPE a_MyMeta)
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
};
