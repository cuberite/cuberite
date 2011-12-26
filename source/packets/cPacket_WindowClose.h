#pragma once

#include "cPacket.h"
#include "PacketID.h"

class cPacket_WindowClose : public cPacket
{
public:
	cPacket_WindowClose()
		: m_Close( 0 )
	{ m_PacketID = E_WINDOW_CLOSE; }
	virtual cPacket* Clone() const { return new cPacket_WindowClose(*this); }

	bool Parse(cSocket & a_Socket);
	bool Send(cSocket & a_Socket);

	char m_Close; // m_Close == cWindow WindowType number

	static const unsigned int c_Size = 1 + 1;
};