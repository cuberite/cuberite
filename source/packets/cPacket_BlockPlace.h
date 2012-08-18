
#pragma once

#include "cPacket.h"
#include "../cItem.h"





class cPacket_BlockPlace :
	public cPacket
{
public:
	cPacket_BlockPlace()
		: m_PosX( 0 )
		, m_PosY( 0 )
		, m_PosZ( 0 )
		, m_Direction( 0 )
	{
		m_PacketID = E_BLOCK_PLACE;
	}
	
	virtual cPacket* Clone() const { return new cPacket_BlockPlace(*this); }

	virtual int Parse(cByteBuffer & a_Buffer) override;

	int  m_PosX;
	unsigned char m_PosY;
	int  m_PosZ;
	char m_Direction;

	cItem m_HeldItem;
} ;




