#pragma once

#include "cPacket.h"
#include "PacketID.h"

class cPacket_Thunderbolt : public cPacket
{
public:
	cPacket_Thunderbolt()
		: m_UniqueID( 0 )
		, m_Unknown( 0 )
		, m_xLBPos( 0 )
		, m_yLBPos( 0 )
		, m_zLBPos( 0 )
	{ m_PacketID = E_THUNDERBOLT; m_Unknown = true; }
	cPacket_Thunderbolt( const cPacket_Thunderbolt & a_Copy );
	virtual cPacket* Clone() const { return new cPacket_Thunderbolt(*this); }

	bool Send(cSocket & a_Socket);

	int m_UniqueID; // The entity ID of the thunderbolt
	bool m_Unknown; // Always true. Might have a meaning in the future...
	int m_xLBPos; // Thunderbolt X as Absolute Integer
	int m_yLBPos; // Thunderbolt Y as Absolute Integer
	int m_zLBPos; // Thunderbolt Z as Absolute Integer

	static const unsigned int c_Size = 1 + 4 + 1 + 4 + 4 + 4;
};
