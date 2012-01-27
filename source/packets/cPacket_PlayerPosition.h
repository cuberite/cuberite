#pragma once

#include "cPacket.h"


class cPlayer;
class cPacket_PlayerPosition : public cPacket
{
public:
	cPacket_PlayerPosition( cPlayer* a_Player );
	cPacket_PlayerPosition()
		: m_PosX( 0.0 )
		, m_PosY( 0.0 )
		, m_Stance( 0.0 )
		, m_PosZ( 0.0 )
		, m_bFlying( false )
	{ m_PacketID = E_PLAYERPOS;  }
	virtual cPacket* Clone() const { return new cPacket_PlayerPosition(*this); }

	virtual bool Parse(cSocket & a_Socket);
	virtual bool Send(cSocket & a_Socket);

	double m_PosX;
	double m_PosY;
	double m_Stance;
	double m_PosZ;
	bool m_bFlying; // Yeah.. wtf
	static const unsigned int c_Size = 34;
};