
#pragma once

#include "cPacket.h"





class cPacket_BlockAction :
	public cPacket
{
public:
	cPacket_BlockAction()
		: m_BlockX( 0 )
		, m_BlockY( 0 )
		, m_BlockZ( 0 )
		, m_Byte1( 0 )
		, m_Byte2( 0 )
	{
		m_PacketID = E_BLOCK_ACTION;
	}
	
	cPacket_BlockAction( const cPacket_BlockAction & a_Copy );
	virtual cPacket * Clone() const { return new cPacket_BlockAction(*this); }

	virtual void Serialize(AString & a_Data) const override;

	int   m_BlockX;
	short m_BlockY;
	int   m_BlockZ;
	char  m_Byte1; 	// Varies
	char  m_Byte2; 	// Varies
};




