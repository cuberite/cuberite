#pragma once

#include "cPacket.h"


class cPacket_BlockChange : public cPacket
{
public:
	cPacket_BlockChange()
		: m_PosX( 0 )
		, m_PosY( 0 )
		, m_PosZ( 0 )
		, m_BlockType( 0 )
		, m_BlockMeta( 0 )
	{ m_PacketID = E_BLOCK_CHANGE;  }
	virtual cPacket* Clone() const { return new cPacket_BlockChange(*this); }

	virtual void Serialize(AString & a_Data) const override;

	int  m_PosX;
	unsigned char m_PosY;
	int  m_PosZ;
	char m_BlockType;
	char m_BlockMeta;
	
	static const unsigned int c_Size = 1 + 4 + 1 + 4 + 1 + 1;
};