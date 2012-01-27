#pragma once

#include "cPacket.h"


class cPacket_Explosion : public cPacket
{
public:
	cPacket_Explosion()
		: m_PosX( 0 )
		, m_PosY( 0 )
		, m_PosZ( 0 )
		, m_Radius( 0 )
		, m_RecordsCount( 0 )
		, m_Records( 0 )
	{ m_PacketID = E_EXPLOSION; m_Records = 0; }
	cPacket_Explosion( const cPacket_Explosion & a_Copy );
	~cPacket_Explosion();
	virtual cPacket* Clone() const { return new cPacket_Explosion(*this); }

	bool Send(cSocket & a_Socket);

	double	m_PosX; // The entity ID of the thunderbolt
	double	m_PosY; 	// Always true. Might have a meaning in the future...
	double	m_PosZ; 	// Thunderbolt X as Absolute Integer
	float		m_Radius; 	// Thunderbolt Y as Absolute Integer
	int			m_RecordsCount; 	// Thunderbolt Z as Absolute Integer

	static const unsigned int c_Size = 1 + 8 + 8 + 8 + 4 + 4;

	char*		m_Records;
};
