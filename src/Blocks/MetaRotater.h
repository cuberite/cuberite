/**
 * Copyright 2014 MCServer Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
// MetaRotater.h

// Provides a mixin for rotations and reflections

#pragma once

// MSVC generates warnings for the templated AssertIfNotMatched parameter conditions, so disable it:
#ifdef _MSC_VER
	#pragma warning(disable: 4127) // Conditional expression is constant
#endif





/*
Provides a mixin for rotations and reflections following the standard pattern of apply mask then use case.

Usage:
Inherit from this class providing your base class as Base, the BitMask for the direction bits in bitmask and the masked value for the directions in North, East, South, West. There is also an aptional parameter AssertIfNotMatched. Set this if it is invalid for a block to exist in any other state.
*/

template<class Base, NIBBLETYPE BitMask, NIBBLETYPE North, NIBBLETYPE East, NIBBLETYPE South, NIBBLETYPE West, bool AssertIfNotMatched = false>
class cMetaRotater : public Base
{
public:

	cMetaRotater(BLOCKTYPE a_BlockType) :
		Base(a_BlockType)
	{}
	
	virtual ~cMetaRotater() {}

	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta)  override;
	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta)  override;
	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta)  override;
	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta)  override;
};





template<class Base, NIBBLETYPE BitMask, NIBBLETYPE North, NIBBLETYPE East, NIBBLETYPE South, NIBBLETYPE West, bool AssertIfNotMatched>
NIBBLETYPE cMetaRotater<Base, BitMask, North, East, South, West, AssertIfNotMatched>::MetaRotateCW(NIBBLETYPE a_Meta)
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





template<class Base, NIBBLETYPE BitMask, NIBBLETYPE North, NIBBLETYPE East, NIBBLETYPE South, NIBBLETYPE West, bool AssertIfNotMatched>
NIBBLETYPE cMetaRotater<Base, BitMask, North, East, South, West, AssertIfNotMatched>::MetaRotateCCW(NIBBLETYPE a_Meta)
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





template<class Base, NIBBLETYPE BitMask, NIBBLETYPE North, NIBBLETYPE East, NIBBLETYPE South, NIBBLETYPE West, bool AssertIfNotMatched>
NIBBLETYPE cMetaRotater<Base, BitMask, North, East, South, West, AssertIfNotMatched>::MetaMirrorXY(NIBBLETYPE a_Meta)
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





template<class Base, NIBBLETYPE BitMask, NIBBLETYPE North, NIBBLETYPE East, NIBBLETYPE South, NIBBLETYPE West, bool AssertIfNotMatched>
NIBBLETYPE cMetaRotater<Base, BitMask, North, East, South, West, AssertIfNotMatched>::MetaMirrorYZ(NIBBLETYPE a_Meta)
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




