#pragma once

#include "cPacket.h"


class cPacket_ArmAnim : public cPacket
{
public:
	cPacket_ArmAnim()
		: m_EntityID( 0 )
		, m_Animation( 0 )
	{ m_PacketID = E_ANIMATION; }
	virtual cPacket* Clone() const { return new cPacket_ArmAnim(*this); }

	virtual int Parse(cByteBuffer & a_Buffer) override;
	virtual void Serialize(AString & a_Data) const override;

	int m_EntityID;
	char m_Animation;
	static const unsigned int c_Size = 1 + 4 + 1;
};