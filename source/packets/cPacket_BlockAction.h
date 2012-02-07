
#pragma once

#include "cPacket.h"


class cPacket_BlockAction : public cPacket
{
public:
	cPacket_BlockAction()
		: m_PosX( 0 )
		, m_PosY( 0 )
		, m_PosZ( 0 )
		, m_Byte1( 0 )
		, m_Byte2( 0 )
	{ m_PacketID = E_BLOCK_ACTION; }
	cPacket_BlockAction( const cPacket_BlockAction & a_Copy );
	virtual cPacket* Clone() const { return new cPacket_BlockAction(*this); }

	virtual void Serialize(AString & a_Data) const override;

	int   m_PosX; 	// Block X Coordinate 
	short m_PosY; 	// Block Y Coordinate 
	int   m_PosZ; 	// Block Z Coordinate 
	char  m_Byte1; 	// Varies
	char  m_Byte2; 	// Varies

	static const unsigned int c_Size = 1 + 4 + 2 + 4 + 1 + 1;
};




