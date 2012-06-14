#pragma once


#include "cPacket.h"
#include "../cItem.h"

class cPacket_ItemData : public cPacket
{
public:
	cPacket_ItemData()
		: m_ItemID( 0 )
		, m_ItemCount( 0 )
		, m_ItemUses( 0 )
		, m_EnchantNums(-1)
	{
	}

	virtual cPacket* Clone() const { return new cPacket_ItemData(*this); }

	virtual int Parse(const char * a_Data, int a_Size) override;

	static void AppendItem(AString & a_Data, short a_ItemID, char a_Quantity, short a_Damage);
	static void AppendItem(AString & a_Data, const cItem * a_Item);

	int GetSize(short a_ItemID);

	// Below = item
	short m_ItemID; // if this is -1 the next stuff dont exist
	char m_ItemCount;
	short m_ItemUses;

	short m_EnchantNums;

	static unsigned int c_Size; // Minimal size ( +1+1  = max)
};