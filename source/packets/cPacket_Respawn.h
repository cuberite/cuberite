#pragma once

#include "cPacket.h"
#include "PacketID.h"

class cPacket_Respawn : public cPacket
{
public:
	cPacket_Respawn()
		: m_World( 0 )
		, m_Difficulty( 0 )
		, m_CreativeMode( 0 )
		, m_WorldHeight( 0 )
		, m_MapSeed( 0 )
	{ m_PacketID = E_RESPAWN; }
	virtual cPacket* Clone() const { return new cPacket_Respawn( *this ); }

	bool Send(cSocket & a_Socket);
	bool Parse(cSocket & a_Socket);

	char m_World;
	char m_Difficulty;
	char m_CreativeMode;
	short m_WorldHeight;
	long long m_MapSeed;

	static const unsigned int c_Size = 1 + 1 + 1 + 1 + 2 + 8;
};
