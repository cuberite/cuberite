#pragma once

#include "cPacket.h"


class cPacket_InventoryProgressBar : public cPacket
{
public:
	cPacket_InventoryProgressBar()
		: m_WindowID( 0 )
		, m_ProgressBar( 0 )
		, m_Value( 0 )
	{ m_PacketID = E_INVENTORY_PROGRESS; }
	virtual cPacket* Clone() const { return new cPacket_InventoryProgressBar(*this); }

	bool Send(cSocket & a_Socket);

	char m_WindowID;
	short m_ProgressBar;
	short m_Value;

	static const unsigned int c_Size = 1 + 1 + 2 + 2;
};