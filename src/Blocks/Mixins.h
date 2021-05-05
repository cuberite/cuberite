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





/** Mixin to clear the block's meta value when converting to a pickup. */
template <class Base>
class cClearMetaOnDrop :
	public Base
{
public:

	constexpr cClearMetaOnDrop(BLOCKTYPE a_BlockType):
		Base(a_BlockType)
	{
	}

protected:

	~cClearMetaOnDrop() = default;

private:

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		// Reset the meta to zero:
		return cItem(this->m_BlockType);
	}
};





/** Mixin for rotations and reflections following the standard pattern of "apply mask, then use a switch".
Inherit from this class providing your base class as Base, the BitMask for the direction bits in bitmask and the masked value for the directions in North, East, South, West.
There is also an aptional parameter AssertIfNotMatched, set this if it is invalid for a block to exist in any other state. */
template <class Base, NIBBLETYPE BitMask, NIBBLETYPE North, NIBBLETYPE East, NIBBLETYPE South, NIBBLETYPE West, bool AssertIfNotMatched = false>
class cMetaRotator :
	public Base
{
public:

	constexpr cMetaRotator(BLOCKTYPE a_BlockType):
		Base(a_BlockType)
	{
	}

protected:

	~cMetaRotator() = default;

	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) const override
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





	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta) const override
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





	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta) const override
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





	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta) const override
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
class cYawRotator :
	public cMetaRotator<Base, BitMask, North, East, South, West, AssertIfNotMatched>
{
	using Super = cMetaRotator<Base, BitMask, North, East, South, West, AssertIfNotMatched>;

public:

	using Super::Super;


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

protected:

	~cYawRotator() = default;
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

	using Super::Super;


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

protected:

	~cPitchYawRotator() = default;


	virtual NIBBLETYPE MetaMirrorXZ(NIBBLETYPE a_Meta) const override
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
};
