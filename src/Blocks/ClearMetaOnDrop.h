
#pragma once

// mixin for use to clear meta values when the block is converted to a pickup

// Usage: inherit from this class, passing the parent class as the parameter Base
// For example to use in class Foo which should inherit Bar use
// class Foo : public cClearMetaOnDrop<Bar>;

template <class Base>
class cClearMetaOnDrop : public Base
{
public:

	cClearMetaOnDrop(BLOCKTYPE a_BlockType) :
		Base(a_BlockType)
	{}

	virtual ~cClearMetaOnDrop() override {}
	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(this->m_BlockType));
	}
};
