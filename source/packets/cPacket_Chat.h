#pragma once

#include "cPacket.h"
#include "PacketID.h"

class cPacket_Chat : public cPacket
{
public:
	cPacket_Chat() { m_PacketID = E_CHAT;  }
	cPacket_Chat( const std::string & a_Message ) : m_Message( a_Message) { m_PacketID = E_CHAT; }
	virtual cPacket* Clone() const { return new cPacket_Chat(*this); }

	bool Parse(cSocket & a_Socket);
	bool Send(cSocket & a_Socket);

	std::string m_Message;
	static const unsigned int c_Size = 3; // Minimum size
};