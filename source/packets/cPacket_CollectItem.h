#pragma once

#include "cPacket.h"


class cPacket_CollectItem : public cPacket
{
public:
	cPacket_CollectItem()
		: m_CollectedID( 0 )
		, m_CollectorID( 0 )
	{ m_PacketID = E_COLLECT_ITEM;  }
	virtual cPacket* Clone() const { return new cPacket_CollectItem(*this); }

	bool Send( cSocket & a_Socket );

	int m_CollectedID;
	int m_CollectorID;
	static const unsigned int c_Size = 1 + 4 + 4;
};