
#pragma once

#include "cPacket.h"





class cPacket_UseEntity : public cPacket
{
public:
	cPacket_UseEntity()
		: m_UniqueID( 0 )
		, m_TargetID( 0 )
		, m_bLeftClick( false )
	{ m_PacketID = E_USE_ENTITY;  }
	virtual cPacket* Clone() const { return new cPacket_UseEntity(*this); }

	virtual int Parse(const char * a_Data, int a_Size) override;

	int  m_UniqueID;
	int  m_TargetID;
	bool m_bLeftClick;

	static const unsigned int c_Size = 1 + 4 + 4 + 1;
};




