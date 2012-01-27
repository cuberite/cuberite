#pragma once

#include "cPacket.h"


class cPacket_ItemSwitch : public cPacket
{
public:
	cPacket_ItemSwitch() 
		: m_SlotNum( 0 )
	{ m_PacketID = E_ITEM_SWITCH; }
	virtual cPacket* Clone() const { return new cPacket_ItemSwitch(*this); }

	bool Parse(cSocket & a_Socket);
	bool Send(cSocket & a_Socket);

	short m_SlotNum;
	static const unsigned int c_Size = 1 + 2;
};