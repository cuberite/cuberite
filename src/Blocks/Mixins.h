// Mixins.h

// Provides various mixins for easier cBlockHandler descendant implementations

/* The general use case is to derive a handler from these mixins, providing a suitable base to them:
class cBlockAir: public cBlockWithNoDrops<cBlockHandler>;
class cBlockLadder: public cMetaRotator<cClearMetaOnDrop, ...>
*/

#pragma once

#include "../Item.h"
#include "../Entities/Player.h"





// MSVC generates warnings for the templated AssertIfNotMatched parameter conditions, so disable it:
#ifdef _MSC_VER
	#pragma warning(disable: 4127)  // Conditional expression is constant
#endif





template <class Base = cBlockHandler>
class cBlockWithNoDrops:
	public Base
{
public:

	cBlockWithNoDrops(BLOCKTYPE a_BlockType):
		Base(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		// Don't drop anything:
		return {};
	}
};





/** Mixin to clear the block's meta value when converting to a pickup. */
template <class Base>
class cClearMetaOnDrop:
	public Base
{
public:

	cClearMetaOnDrop(BLOCKTYPE a_BlockType):
		Base(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		// Reset the meta to zero:
		return cItem(this->m_BlockType);
	}
};





/** Mixin for rotations and reflections following the standard pattern of "apply mask, then use a switch".
Inherit from this class providing your base class as Base, the BitMask for the direction bits in bitmask and the masked value for the directions in North, East, South, West.
There is also an aptional parameter AssertIfNotMatched, set this if it is invalid for a block to exist in any other state. */
template <class Base, NIBBLETYPE BitMask, NIBBLETYPE North, NIBBLETYPE East, NIBBLETYPE South, NIBBLETYPE West, bool AssertIfNotMatched = false>
class cMetaRotator:
	public Base
{
public:

	cMetaRotator(BLOCKTYPE a_BlockType):
		Base(a_BlockType)
	{}





	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) override
	{
		NIBBLETYPE OtherMeta = a_Meta & (~BitMask);
		switch (a_Meta & BitMask)
		{
			case South: return East  | OtherMeta;
			case East:  return North | OtherMeta;
			case North: return West  | OtherMeta;
			case West:  return South | OtherMeta;
		}
		if (AssertIfNotMatched)
		{
			ASSERT(!"Invalid Meta value");
		}
		return a_Meta;
	}





	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta) override
	{
		NIBBLETYPE OtherMeta = a_Meta & (~BitMask);
		switch (a_Meta & BitMask)
		{
			case South: return West  | OtherMeta;
			case West:  return North | OtherMeta;
			case North: return East  | OtherMeta;
			case East:  return South | OtherMeta;
		}
		if (AssertIfNotMatched)
		{
			ASSERT(!"Invalid Meta value");
		}
		return a_Meta;
	}





	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta) override
	{
		NIBBLETYPE OtherMeta = a_Meta & (~BitMask);
		switch (a_Meta & BitMask)
		{
			case South: return North | OtherMeta;
			case North: return South | OtherMeta;
		}
		// Not Facing North or South; No change.
		return a_Meta;
	}





	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta) override
	{
		NIBBLETYPE OtherMeta = a_Meta & (~BitMask);
		switch (a_Meta & BitMask)
		{
			case West: return East | OtherMeta;
			case East: return West | OtherMeta;
		}
		// Not Facing East or West; No change.
		return a_Meta;
	}
};





/** Mixin for rotations and reflections following the standard pattern of "apply mask, then use a switch".
Inherit from this class providing your base class as Base, the BitMask for the direction bits in bitmask and the masked value for the directions in
North, NorthNorthEast, NorthEast, EastNorthEast,
East, EastSouthEast, SouthEast, SouthSouthEast,
South, SouthSouthWest, SouthWest, WestSouthWest,
West, WestNorthWest, NorthWest, NorthNorthWest.
There is also an aptional parameter AssertIfNotMatched, set this if it is invalid for a block to exist in any other state. */
template <class Base, NIBBLETYPE BitMask, NIBBLETYPE North, NIBBLETYPE NorthNorthEast, NIBBLETYPE NorthEast, NIBBLETYPE EastNorthEast,
	NIBBLETYPE East, NIBBLETYPE EastSouthEast, NIBBLETYPE SouthEast, NIBBLETYPE SouthSouthEast,
	NIBBLETYPE South, NIBBLETYPE SouthSouthWest, NIBBLETYPE SouthWest, NIBBLETYPE WestSouthWest,
	NIBBLETYPE West, NIBBLETYPE WestNorthWest, NIBBLETYPE NorthWest, NIBBLETYPE NorthNorthWest, bool AssertIfNotMatched = false>
class cMetaRotatorFine:
	public Base
{
public:

	cMetaRotatorFine(BLOCKTYPE a_BlockType):
		Base(a_BlockType)
	{}





	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) override
	{
		NIBBLETYPE OtherMeta = a_Meta & (~BitMask);
		switch (a_Meta & BitMask)
		{
			case North: return NorthNorthEast | OtherMeta;
			case NorthNorthEast: return NorthEast | OtherMeta;
			case NorthEast: return EastNorthEast | OtherMeta;
			case EastNorthEast: return East | OtherMeta;
			case East: return EastSouthEast | OtherMeta;
			case EastSouthEast: return SouthEast | OtherMeta;
			case SouthEast: return SouthSouthEast | OtherMeta;
			case SouthSouthEast: return South | OtherMeta;
			case South: return SouthSouthWest | OtherMeta;
			case SouthSouthWest: return SouthWest | OtherMeta;
			case SouthWest: return WestSouthWest | OtherMeta;
			case WestSouthWest: return West | OtherMeta;
			case West: return WestNorthWest | OtherMeta;
			case WestNorthWest: return NorthWest | OtherMeta;
			case NorthWest: return NorthNorthWest | OtherMeta;
			case NorthNorthWest: return North | OtherMeta;
		}
		if (AssertIfNotMatched)
		{
			ASSERT(!"Invalid Meta value");
		}
		return a_Meta;
	}





	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta) override
	{
		NIBBLETYPE OtherMeta = a_Meta & (~BitMask);
		switch (a_Meta & BitMask)
		{
			case North: return NorthNorthWest | OtherMeta;
			case NorthNorthWest: return NorthWest | OtherMeta;
			case NorthWest: return WestNorthWest | OtherMeta;
			case WestNorthWest: return West | OtherMeta;
			case West: return WestSouthWest | OtherMeta;
			case WestSouthWest: return SouthWest | OtherMeta;
			case SouthWest: return SouthSouthWest | OtherMeta;
			case SouthSouthWest: return South | OtherMeta;
			case South: return SouthSouthEast | OtherMeta;
			case SouthSouthEast: return SouthEast | OtherMeta;
			case SouthEast: return EastSouthEast | OtherMeta;
			case EastSouthEast: return East | OtherMeta;
			case East: return EastNorthEast | OtherMeta;
			case EastNorthEast: return NorthEast | OtherMeta;
			case NorthEast: return NorthNorthEast | OtherMeta;
			case NorthNorthEast: return North | OtherMeta;
		}
		if (AssertIfNotMatched)
		{
			ASSERT(!"Invalid Meta value");
		}
		return a_Meta;
	}





	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta) override
	{
		NIBBLETYPE OtherMeta = a_Meta & (~BitMask);
		switch (a_Meta & BitMask)
		{
			case North: return South | OtherMeta;
			case NorthNorthEast: return SouthSouthEast | OtherMeta;
			case NorthEast: return SouthEast | OtherMeta;
			case EastNorthEast: return EastSouthEast | OtherMeta;
			case EastSouthEast: return EastNorthEast | OtherMeta;
			case SouthEast: return NorthEast | OtherMeta;
			case SouthSouthEast: return NorthNorthEast | OtherMeta;
			case South: return North | OtherMeta;
			case SouthSouthWest: return NorthNorthWest;
			case SouthWest: return NorthWest;
			case WestSouthWest: return WestNorthWest;
			case WestNorthWest: return WestSouthWest;
			case NorthWest: return SouthWest;
			case NorthNorthWest: return SouthSouthWest;
		}
		// Facing East or West; No Change
		return a_Meta;
	}





	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta) override
	{
		NIBBLETYPE OtherMeta = a_Meta & (~BitMask);
		switch (a_Meta & BitMask)
		{
			case NorthNorthEast: return NorthNorthWest;
			case NorthEast: return NorthWest;
			case EastNorthEast: return WestNorthWest;
			case East: return West;
			case EastSouthEast: return WestSouthWest;
			case SouthEast: return SouthWest;
			case SouthSouthEast: return SouthSouthWest;
			case SouthSouthWest: return SouthSouthEast;
			case SouthWest: return SouthEast;
			case WestSouthWest: return EastSouthEast;
			case West: return East;
			case WestNorthWest: return EastNorthEast;
			case NorthWest: return NorthEast;
			case NorthNorthWest: return NorthNorthEast;
		}
		// Facing North or South; No change.
		return a_Meta;
	}
};





/** Mixin for blocks whose meta on placement depends on the yaw of the player placing the block. BitMask
selects the direction bits from the block's meta values. */
template <
	class Base,
	NIBBLETYPE BitMask = 0x07,
	NIBBLETYPE North = 0x02,
	NIBBLETYPE East = 0x05,
	NIBBLETYPE South = 0x03,
	NIBBLETYPE West = 0x04,
	bool AssertIfNotMatched = false
>
class cYawRotator:
	public cMetaRotator<Base, BitMask, North, East, South, West, AssertIfNotMatched>
{
	using Super = cMetaRotator<Base, BitMask, North, East, South, West, AssertIfNotMatched>;
public:

	cYawRotator(BLOCKTYPE a_BlockType):
		Super(a_BlockType)
	{
	}





	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace,
		const Vector3i a_CursorPos,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		NIBBLETYPE BaseMeta;
		if (!Super::GetPlacementBlockTypeMeta(a_ChunkInterface, a_Player, a_BlockPos, a_BlockFace, a_CursorPos, a_BlockType, BaseMeta))
		{
			return false;
		}

		a_BlockMeta = (BaseMeta & ~BitMask) | YawToMetaData(a_Player.GetYaw());
		return true;
	}





	/** Converts the rotation value as returned by cPlayer::GetYaw() to the appropriate metadata
	value for a block placed by a player facing that way */
	static NIBBLETYPE YawToMetaData(double a_Rotation)
	{
		if ((a_Rotation >= -135) && (a_Rotation < -45))
		{
			return East;
		}
		else if ((a_Rotation >= -45) && (a_Rotation < 45))
		{
			return South;
		}
		else if ((a_Rotation >= 45) && (a_Rotation < 135))
		{
			return West;
		}
		else  // degrees jumping from 180 to -180
		{
			return North;
		}
	}
};





/** Mixin for blocks whose meta on placement depends on the yaw of the player placing the block. BitMask
selects the direction bits from the block's meta values. */
template <class Base,
	NIBBLETYPE BitMask = 0x0F,
	NIBBLETYPE South = 0x00,
	NIBBLETYPE SouthSouthWest = 0x01,
	NIBBLETYPE SouthWest = 0x02,
	NIBBLETYPE WestSouthWest = 0x03,
	NIBBLETYPE West = 0x04,
	NIBBLETYPE WestNorthWest = 0x05,
	NIBBLETYPE NorthWest = 0x06,
	NIBBLETYPE NorthNorthWest = 0x07,
	NIBBLETYPE North = 0x08,
	NIBBLETYPE NorthNorthEast = 0x09,
	NIBBLETYPE NorthEast = 0x0a,
	NIBBLETYPE EastNorthEast = 0x0b,
	NIBBLETYPE East = 0x0c,
	NIBBLETYPE EastSouthEast = 0x0d,
	NIBBLETYPE SouthEast = 0x0e,
	NIBBLETYPE SouthSouthEast = 0x0f,
	bool AssertIfNotMatched = false>
class cYawRotatorFine:
	public cMetaRotatorFine<Base, BitMask,
		North, NorthNorthEast, NorthEast, EastNorthEast,
		East, EastSouthEast, SouthEast, SouthSouthEast,
		South, SouthSouthWest, SouthWest, WestSouthWest,
		West, WestNorthWest, NorthWest, NorthNorthWest, AssertIfNotMatched>
{
	using Super = cMetaRotatorFine<Base, BitMask,
		North, NorthNorthEast, NorthEast, EastNorthEast,
		East, EastSouthEast, SouthEast, SouthSouthEast,
		South, SouthSouthWest, SouthWest, WestSouthWest,
		West, WestNorthWest, NorthWest, NorthNorthWest, AssertIfNotMatched>;
  public:

	cYawRotatorFine(BLOCKTYPE a_BlockType):
		Super(a_BlockType)
	{
	}





	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace,
		const Vector3i a_CursorPos,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		NIBBLETYPE BaseMeta;
		if (!Super::GetPlacementBlockTypeMeta(a_ChunkInterface, a_Player, a_BlockPos, a_BlockFace, a_CursorPos, a_BlockType, BaseMeta))
		{
			return false;
		}

		a_BlockMeta = (BaseMeta & ~BitMask) | YawToMetaData(a_Player.GetYaw());
		return true;
	}





	/** Converts the rotation value as returned by cPlayer::GetYaw() to the appropriate metadata
	value for a block placed by a player facing that way */
	static NIBBLETYPE YawToMetaData(double a_Rotation)
	{
		if ((a_Rotation >= - 11.25f) && (a_Rotation < 11.25f))
		{
			return South
		}
		else if ((a_Rotation >= 11.25f) && (a_Rotation < 33.75f))
		{
			return SouthSouthWest
		}
		else if ((a_Rotation >= 23.75f) && (a_Rotation < 56.25f))
		{
			return SouthWest
		}
		else if ((a_Rotation >= 56.25f) && (a_Rotation < 78.75f))
		{
			return WestSouthWest
		}
		else if ((a_Rotation >= 78.75f) && (a_Rotation < 101.25f))
		{
			return West
		}
		else if ((a_Rotation >= 101.25f) && (a_Rotation < 123.75f))
		{
			return WestNorthWest
		}
		else if ((a_Rotation >= 123.75f) && (a_Rotation < 146.25f))
		{
			return NorthWest
		}
		else if ((a_Rotation >= 146.25f) && (a_Rotation < 168.75f))
		{
			return NorthNorthWest
		}
		else if ((a_Rotation >= -168.75f) && (a_Rotation < -146.25f))
		{
			return NorthNorthEast
		}
		else if ((a_Rotation >= -146.25f) && (a_Rotation < -123.75f))
		{
			return NorthEast
		}
		else if ((a_Rotation >= -123.75f) && (a_Rotation < -101.25f))
		{
			return EastNorthEast
		}
		else if ((a_Rotation >= -101.25) && (a_Rotation < -78.75f))
		{
			return East
		}
		else if ((a_Rotation >= -78.75) && (a_Rotation < -56.25f))
		{
			return EastSouthEast
		}
		else if ((a_Rotation >= -56.25f) && (a_Rotation < -33.75f))
		{
			return SouthEast
		}
		else if ((a_Rotation >= -33.75f) && (a_Rotation < -11.25f))
		{
			return SouthSouthEast
		}
		else // degrees jumping from 180 to -180
		{
			return North
		}
	}
};





/** Mixin for blocks whose meta on placement depends on the pitch and yaw of the player placing the block. BitMask
selects the direction bits from the block's meta values. */
template <
	class Base,
	NIBBLETYPE BitMask = 0x07,
	NIBBLETYPE North = 0x02,
	NIBBLETYPE East = 0x05,
	NIBBLETYPE South = 0x03,
	NIBBLETYPE West = 0x04,
	NIBBLETYPE Up = 0x00,
	NIBBLETYPE Down = 0x01
>
class cPitchYawRotator:
	public cYawRotator<Base, BitMask, North, East, South, West>
{
	using Super = cYawRotator<Base, BitMask, North, East, South, West>;

public:

	cPitchYawRotator(BLOCKTYPE a_BlockType):
		Super(a_BlockType)
	{
	}





	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface,
		cPlayer & a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		NIBBLETYPE BaseMeta;
		if (!Super::GetPlacementBlockTypeMeta(a_ChunkInterface, a_Player, a_PlacedBlockPos, a_ClickedBlockFace, a_CursorPos, a_BlockType, BaseMeta))
		{
			return false;
		}

		a_BlockMeta = (BaseMeta & ~BitMask) |  PitchYawToMetaData(a_Player.GetYaw(), a_Player.GetPitch());
		return true;
	}





	virtual NIBBLETYPE MetaMirrorXZ(NIBBLETYPE a_Meta) override
	{
		NIBBLETYPE OtherMeta = a_Meta & (~BitMask);
		switch (a_Meta & BitMask)
		{
			case Down: return Up | OtherMeta;  // Down -> Up
			case Up: return Down | OtherMeta;  // Up -> Down
		}
		// Not Facing Up or Down; No change.
		return a_Meta;
	}





	/** Converts the rotation and pitch values as returned by cPlayer::GetYaw() and cPlayer::GetPitch()
	respectively to the appropriate metadata value for a block placed by a player facing that way */
	static NIBBLETYPE PitchYawToMetaData(double a_Rotation, double a_Pitch)
	{
		if (a_Pitch >= 50)
		{
			return Up;
		}
		else if (a_Pitch <= -50)
		{
			return Down;
		}

		return Super::YawToMetaData(a_Rotation);
	}
};
