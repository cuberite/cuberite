#pragma once

#include "PacketID.h"
#include "cPacket.h"
#include "cItem.h"

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

	bool Parse(cSocket & a_Socket);

	virtual cPacket* Clone() const { return new cPacket_ItemData(*this); }

	void AppendItem(char* a_Message, unsigned int &a_Iterator, short a_ItemID, char a_Quantity, short a_Damage);

	void AppendItem(char* a_Message, unsigned int &a_Iterator, cItem *a_Item);

	int GetSize(short a_ItemID);


	// Below = item
	short m_ItemID; // if this is -1 the next stuff dont exist
	char m_ItemCount;
	short m_ItemUses;

	short m_EnchantNums;

	static unsigned int c_Size; // Minimal size ( +1+1  = max)
};