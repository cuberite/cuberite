
#pragma once

#include "cPacket.h"





class cPacket_KeepAlive : public cPacket
{
public:
	cPacket_KeepAlive() { m_PacketID = E_KEEP_ALIVE; }
	cPacket_KeepAlive(int a_PingID) { m_KeepAliveID = a_PingID; }
	virtual cPacket* Clone() const { return new cPacket_KeepAlive(*this); }

	virtual int Parse(const char * a_Data, int a_Size) override;
	virtual void Serialize(AString & a_Data) const override;

	int m_KeepAliveID;

	static const unsigned int c_Size = 1 + 4;
};




