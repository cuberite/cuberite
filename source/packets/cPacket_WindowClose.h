
#pragma once

#include "cPacket.h"





class cPacket_WindowClose : public cPacket
{
public:
	cPacket_WindowClose()
		: m_Close( 0 )
	{ m_PacketID = E_WINDOW_CLOSE; }
	virtual cPacket* Clone() const { return new cPacket_WindowClose(*this); }

	virtual int Parse(const char * a_Data, int a_Size) override;
	virtual void Serialize(AString & a_Data) const override;

	char m_Close; // m_Close == cWindow WindowType number

	static const unsigned int c_Size = 1 + 1;
};




