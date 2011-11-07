#pragma once

#include "cPacket.h"
#include "PacketID.h"

class cPacket_SoundEffect : public cPacket
{
public:
	cPacket_SoundEffect()
		: m_SoundID( 0 )
		, m_PosX( 0 )
		, m_PosY( 0 )
		, m_PosZ( 0 )
		, m_SoundData( 0 )
	{ m_PacketID = E_SOUND_EFFECT; }
	cPacket_SoundEffect( const cPacket_SoundEffect & a_Copy );
	virtual cPacket* Clone() const { return new cPacket_SoundEffect(*this); }

	bool Send(cSocket & a_Socket);

	int 	m_SoundID; 	// Sound ID
	int 	m_PosX; 		// Block X Coordinate 
	char	m_PosY; 		// Block Y Coordinate 
	int 	m_PosZ; 		// Block Z Coordinate 
	int 	m_SoundData;// Extra Sound Data

	static const unsigned int c_Size = 1 + 4 + 4 + 1 + 4 + 4;
};
