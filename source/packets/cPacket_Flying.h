#pragma once

#include "cPacket.h"


class cPacket_Flying : public cPacket
{
public:
	// The BS packet
	cPacket_Flying()
		: m_bFlying( false )
	{ m_PacketID = E_FLYING;  }
	virtual cPacket* Clone() const { return new cPacket_Flying(*this); }

	bool Parse(cSocket & a_Socket);

	bool m_bFlying;
	static const unsigned int c_Size = 2;
};
