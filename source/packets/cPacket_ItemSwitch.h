
#pragma once

#include "cPacket.h"





class cPacket_ItemSwitch : public cPacket
{
public:
	cPacket_ItemSwitch() 
		: m_SlotNum( 0 )
	{ m_PacketID = E_ITEM_SWITCH; }
	virtual cPacket* Clone() const { return new cPacket_ItemSwitch(*this); }

	virtual int Parse(cByteBuffer & a_Buffer) override;

	short m_SlotNum;
};




