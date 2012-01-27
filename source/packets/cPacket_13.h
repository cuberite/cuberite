#pragma once

#include "cPacket.h"


class cPacket_13 : public cPacket
{
public:
	enum ENUM_ACTION
	{
		ACTION_CROUCH = 1,
		ACTION_UNCROUCH = 2,
		ACTION_LEAVEBED = 3,
		ACTION_STARTSPRINTING = 4,
		ACTION_STOPSPRINTING = 5,
	};

	cPacket_13()
		: m_EntityID( 0 )
		, m_ActionID( 0 )
	{ m_PacketID = E_PACKET_13; }
	virtual cPacket* Clone() const { return new cPacket_13( *this ); }

	bool Parse(cSocket & a_Socket);

	int m_EntityID;
	char m_ActionID;

	static const unsigned int c_Size = 1;
};