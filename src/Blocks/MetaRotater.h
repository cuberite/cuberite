
#pragma once

template<class Base, NIBBLETYPE BitFilter, NIBBLETYPE North, NIBBLETYPE East, NIBBLETYPE South, NIBBLETYPE West, bool AssertIfNotMatched = false>
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


template<class Base, NIBBLETYPE BitFilter, NIBBLETYPE North, NIBBLETYPE East, NIBBLETYPE South, NIBBLETYPE West, bool AssertIfNotMatched>
NIBBLETYPE cMetaRotater<Base, BitFilter, North, East, South, West, AssertIfNotMatched>::MetaRotateCW(NIBBLETYPE a_Meta)
{
	NIBBLETYPE OtherMeta = a_Meta & (~BitFilter);
	switch (a_Meta & BitFilter)
	{
		case South: return West  | OtherMeta; 
		case West:  return North | OtherMeta; 
		case North: return East  | OtherMeta;
		case East:  return South | OtherMeta;
	}
	if(AssertIfNotMatched)
	{
		ASSERT(!"Invalid Meta value");
		return a_Meta;
	}
}


template<class Base, NIBBLETYPE BitFilter, NIBBLETYPE North, NIBBLETYPE East, NIBBLETYPE South, NIBBLETYPE West, bool AssertIfNotMatched>
NIBBLETYPE cMetaRotater<Base, BitFilter, North, East, South, West, AssertIfNotMatched>::MetaRotateCCW(NIBBLETYPE a_Meta)
{
	NIBBLETYPE OtherMeta = a_Meta & (~BitFilter);
	switch (a_Meta & BitFilter)
	{
		case South: return East  | OtherMeta; 
		case East:  return North | OtherMeta;
		case North: return West  | OtherMeta;
		case West:  return South | OtherMeta;
	}
	if(AssertIfNotMatched)
	{
		ASSERT(!"Invalid Meta value");
		return a_Meta;
	}
}



template<class Base, NIBBLETYPE BitFilter, NIBBLETYPE North, NIBBLETYPE East, NIBBLETYPE South, NIBBLETYPE West, bool AssertIfNotMatched>
NIBBLETYPE cMetaRotater<Base, BitFilter, North, East, South, West, AssertIfNotMatched>::MetaMirrorXY(NIBBLETYPE a_Meta)
{
	NIBBLETYPE OtherMeta = a_Meta & (~BitFilter);
	switch (a_Meta & BitFilter)
	{
		case South: return North | OtherMeta;
		case North: return South | OtherMeta;
	}
	// Not Facing North or South; No change.
	return a_Meta;
}




template<class Base, NIBBLETYPE BitFilter, NIBBLETYPE North, NIBBLETYPE East, NIBBLETYPE South, NIBBLETYPE West, bool AssertIfNotMatched>
NIBBLETYPE cMetaRotater<Base, BitFilter, North, East, South, West, AssertIfNotMatched>::MetaMirrorYZ(NIBBLETYPE a_Meta)
{
	NIBBLETYPE OtherMeta = a_Meta & (~BitFilter);
	switch (a_Meta & BitFilter)
	{
		case West: return East | OtherMeta;
		case East: return West | OtherMeta;
	}
	// Not Facing East or West; No change.
	return a_Meta;
}
