#pragma once

#include "cPacket.h"
#include "PacketID.h"

class cPacket_ArmAnim : public cPacket
{
public:
	cPacket_ArmAnim()
		: m_EntityID( 0 )
		, m_Animation( 0 )
	{ m_PacketID = E_ANIMATION; }
	virtual cPacket* Clone() const { return new cPacket_ArmAnim(*this); }

	bool Parse(cSocket & a_Socket);
	bool Send(cSocket & a_Socket);

	int m_EntityID;
	char m_Animation;
	static const unsigned int c_Size = 1 + 4 + 1;
};