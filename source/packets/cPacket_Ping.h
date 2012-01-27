#pragma once

#include "cPacket.h"


class cPacket_Ping : public cPacket
{
public:
	cPacket_Ping()
	{ m_PacketID = E_PING; }
	virtual cPacket* Clone() const { return new cPacket_Ping(*this); }

	bool Parse(cSocket & a_Socket) { (void)a_Socket; return true; }

	static const unsigned int c_Size = 1;
};