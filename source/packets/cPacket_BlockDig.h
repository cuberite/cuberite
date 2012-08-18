
#pragma once

#include "cPacket.h"





class cPacket_BlockDig :
	public cPacket
{
public:
	cPacket_BlockDig()
		: m_Status( 0 )
		, m_PosX( 0 )
		, m_PosY( 0 )
		, m_PosZ( 0 )
		, m_Direction( 0 )
	{
		m_PacketID = E_BLOCK_DIG;
	}
	
	virtual cPacket* Clone() const { return new cPacket_BlockDig(*this); }

	virtual int Parse(cByteBuffer & a_Buffer) override;
	virtual void Serialize(AString & a_Data) const override;

	char m_Status;
	int  m_PosX;
	char m_PosY;
	int  m_PosZ;
	char m_Direction;
};




