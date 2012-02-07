
#pragma once

#include "cPacket.h"





class cPacket_PreChunk: public cPacket
{
public:
	cPacket_PreChunk()
		: m_PosX( 0 )
		, m_PosZ( 0 )
		, m_bLoad( false )
	{ m_PacketID = E_PRE_CHUNK;  }
	virtual cPacket* Clone() const { return new cPacket_PreChunk(*this); }

	virtual void Serialize(AString & a_Data) const override;

	int  m_PosX;
	int  m_PosZ;
	bool m_bLoad;
	
	static const unsigned int c_Size = 10;
};




