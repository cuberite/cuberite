
#pragma once

#include "cPacket.h"





class cPacket_Handshake : public cPacket
{
public:
	cPacket_Handshake() { m_PacketID = E_HANDSHAKE; }
	virtual cPacket* Clone() const { return new cPacket_Handshake(*this); }

	virtual int Parse(cByteBuffer & a_Buffer) override;

	std::string m_Username;
};




