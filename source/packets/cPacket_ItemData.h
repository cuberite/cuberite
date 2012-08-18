
#pragma once


#include "cPacket.h"
#include "../cItem.h"





class cPacket_ItemData : public cPacket
{
	cItem & m_Dst;
	
public:
	cPacket_ItemData(cItem & a_Dst) :
		m_Dst(a_Dst)
	{
	}

	virtual cPacket * Clone() const { return new cPacket_ItemData(*this); }

	virtual int Parse(cByteBuffer & a_Buffer) override;

	static void AppendItem(AString & a_Data, short a_ItemType, char a_Quantity, short a_Damage);
	static void AppendItem(AString & a_Data, const cItem & a_Item);

	int GetSize(short a_ItemID);
} ;




