
#pragma once

#include "cPacket.h"





class cPacket_UseEntity :
	public cPacket
{
public:
	cPacket_UseEntity()
		: m_SourceEntityID(0)
		, m_TargetEntityID(0)
		, m_IsLeftClick(false)
	{
		m_PacketID = E_USE_ENTITY;
	}
	
	virtual cPacket * Clone() const { return new cPacket_UseEntity(*this); }

	virtual int Parse(cByteBuffer & a_Buffer) override;

	int  m_SourceEntityID;
	int  m_TargetEntityID;
	bool m_IsLeftClick;
};




