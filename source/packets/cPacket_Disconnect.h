#pragma once

#include "cPacket.h"


class cPacket_Disconnect : public cPacket
{
public:
	cPacket_Disconnect() { m_PacketID = E_DISCONNECT;  }
	cPacket_Disconnect(const std::string & a_Reason) { m_PacketID = E_DISCONNECT; m_Reason = a_Reason;  }
	virtual cPacket* Clone() const { return new cPacket_Disconnect(*this); }

	bool Parse( cSocket & a_Socket );
	bool Send( cSocket & a_Socket );

	std::string m_Reason;
	static const unsigned int c_Size = 3; // Minimum size
};