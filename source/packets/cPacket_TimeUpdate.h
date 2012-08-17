
#pragma once

#include "cPacket.h"





class cPacket_TimeUpdate : public cPacket
{
public:
	cPacket_TimeUpdate()
		: m_Time( 0 )
	{ m_PacketID = E_UPDATE_TIME;  }
	cPacket_TimeUpdate( long long a_Time ) { m_PacketID = E_UPDATE_TIME;  m_Time = a_Time; }
	virtual cPacket* Clone() const { return new cPacket_TimeUpdate(*this); }

	virtual int Parse(cByteBuffer & a_Buffer) override;
	virtual void Serialize(AString & a_Data) const override;

	long long m_Time;
	
	static const unsigned int c_Size = 1 + 8;
};




