
#pragma once

#include "cPacket.h"





class cPacket_Disconnect : public cPacket
{
public:
	cPacket_Disconnect() { m_PacketID = E_DISCONNECT;  }
	cPacket_Disconnect(const AString & a_Reason) { m_PacketID = E_DISCONNECT; m_Reason = a_Reason;  }
	virtual cPacket* Clone() const { return new cPacket_Disconnect(*this); }

	virtual int Parse(cByteBuffer & a_Buffer) override;

	AString m_Reason;
	static const unsigned int c_Size = 3; // Minimum size
};




