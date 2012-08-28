
#pragma once

#include "cPacket.h"





class cPacket_Chat :
	public cPacket
{
public:
	cPacket_Chat() { m_PacketID = E_CHAT;  }
	virtual cPacket* Clone() const { return new cPacket_Chat(*this); }

	virtual int Parse(cByteBuffer & a_Buffer) override;

	AString m_Message;
};




