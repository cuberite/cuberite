
#pragma once

#include "cPacket.h"





class cPacket_ArmAnim :
	public cPacket
{
public:
	cPacket_ArmAnim()
		: m_EntityID( 0 )
		, m_Animation( 0 )
	{
		m_PacketID = E_ANIMATION;
	}
	virtual cPacket* Clone() const { return new cPacket_ArmAnim(*this); }

	virtual int Parse(cByteBuffer & a_Buffer) override;

	int m_EntityID;
	char m_Animation;
};




