#pragma once

#include "cPacket.h"


class cPacket_Chat : public cPacket
{
public:
	cPacket_Chat() { m_PacketID = E_CHAT;  }
	cPacket_Chat( const std::string & a_Message ) : m_Message( a_Message) { m_PacketID = E_CHAT; }
	virtual cPacket* Clone() const { return new cPacket_Chat(*this); }

	virtual int Parse(const char * a_Data, int a_Size) override;
	virtual void Serialize(AString & a_Data) const override;

	AString m_Message;
	static const unsigned int c_Size = 3; // Minimum size
};




