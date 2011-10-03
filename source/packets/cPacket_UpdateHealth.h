#pragma once

#include "cPacket.h"
#include "PacketID.h"

class cPacket_UpdateHealth : public cPacket
{
public:
	cPacket_UpdateHealth() 
		: m_Health( 0 )
		, m_Food( 0 )
		, m_Saturation( 0.f )
	{ m_PacketID = E_UPDATE_HEALTH; }
	cPacket_UpdateHealth( short a_Health ) { m_Health = a_Health; m_PacketID = E_UPDATE_HEALTH; }
	virtual cPacket* Clone() const { return new cPacket_UpdateHealth( *this ); }

	bool Send(cSocket & a_Socket);

	short m_Health;
	short m_Food;
	float m_Saturation;

	static const unsigned int c_Size = 1 + 2 + 2 + 4;
};
