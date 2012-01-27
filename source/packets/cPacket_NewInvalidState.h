#pragma once

#include "cPacket.h"



class cPacket_NewInvalidState : public cPacket
{
public:
	cPacket_NewInvalidState()
		: m_Reason( 0 )
		, m_GameMode( 0 )
	{ m_PacketID = E_NEW_INVALID_STATE;}
	cPacket_NewInvalidState( const cPacket_NewInvalidState & a_Copy );
	virtual cPacket* Clone() const { return new cPacket_NewInvalidState(*this); }

	bool Parse(cSocket & a_Socket);
	bool Send(cSocket & a_Socket);

	char m_Reason; // 0 = Invalid Bed, 1 = Begin Raining, 2 End Raining, 3 = Change Gamemode
	char m_GameMode; // Used only when reason = 3. 0 is survival, 1 is creative.

	static const unsigned int c_Size = 1 + 1 + 1;
};
