
#pragma once

#include "../Item.h"





// mixin for use to clear meta values when the block is converted to a pickup

// Usage: inherit from this class, passing the parent class as the parameter Base
// For example to use in class Foo which should inherit Bar use
// class Foo : public cClearMetaOnDrop<Bar>;





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
