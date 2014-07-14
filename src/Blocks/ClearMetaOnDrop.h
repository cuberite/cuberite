
#pragma once

template<class Base>
class cClearMetaOnDrop : public Base
{
public:

	cClearMetaOnDrop(BLOCKTYPE a_BlockType) :
		Base(a_BlockType)
	{}
	
	virtual ~cClearMetaOnDrop() {}
	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(this->m_BlockType));
	}
};
