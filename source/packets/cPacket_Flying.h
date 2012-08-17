
#pragma once

#include "cPacket.h"





class cPacket_Flying : public cPacket
{
public:
	// The BS packet
	cPacket_Flying()
		: m_bFlying( false )
	{ m_PacketID = E_FLYING;  }
	virtual cPacket* Clone() const { return new cPacket_Flying(*this); }

	virtual int Parse(cByteBuffer & a_Buffer) override;

	bool m_bFlying;
	static const unsigned int c_Size = 2;
};




