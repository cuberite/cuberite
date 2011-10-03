#pragma once

#include "cPacket.h"
#include "PacketID.h"

class cPacket_Handshake : public cPacket
{
public:
	cPacket_Handshake() { m_PacketID = E_HANDSHAKE; }
	virtual cPacket* Clone() const { return new cPacket_Handshake(*this); }

	virtual bool Parse(cSocket & a_Socket);
	virtual bool Send(cSocket & a_Socket);

	std::string m_Username;
	static const unsigned int c_Size = 3; // Minimal size
};