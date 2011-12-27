#pragma once

#include "cPacket.h"
#include "PacketID.h"

class cPacket_KeepAlive : public cPacket
{
public:
	cPacket_KeepAlive() { m_PacketID = E_KEEP_ALIVE; }
	cPacket_KeepAlive(int a_PingID) { m_KeepAliveID = a_PingID; }
	virtual cPacket* Clone() const { return new cPacket_KeepAlive(*this); }

	virtual bool Parse(cSocket & a_Socket);
	virtual bool Send(cSocket & a_Socket);

	int m_KeepAliveID;

	static const unsigned int c_Size = 1 + 4;
};