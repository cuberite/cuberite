#pragma once

#include "cPacket.h"


class cPlayer;
class cPacket_PlayerLook : public cPacket
{
public:
	cPacket_PlayerLook()
		: m_Rotation( 0 )
		, m_Pitch( 0 )
		, m_bFlying( false )
	{ m_PacketID = E_PLAYERLOOK;  }
	cPacket_PlayerLook( cPlayer* a_Player );
	virtual cPacket* Clone() const { return new cPacket_PlayerLook(*this); }

	bool Parse( cSocket & a_Socket );
	bool Send( cSocket & a_Socket );

	float m_Rotation;
	float m_Pitch;
	bool m_bFlying; // Yeah.. wtf
	static const unsigned int c_Size = 10;
};