// Mixins.h

// Provides various mixins for easier cBlockHandler descendant implementations

/* The general use case is to derive a handler from these mixins, providing a suitable base to them:
class cBlockAir: public cBlockWithNoDrops<cBlockHandler>;
class cBlockLadder: public cMetaRotator<cClearMetaOnDrop, ...>
*/

#pragma once

#include "../Item.h"





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
