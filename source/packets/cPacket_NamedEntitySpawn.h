#pragma once

#include "cPacket.h"


class cPacket_NamedEntitySpawn : public cPacket
{
public:
	cPacket_NamedEntitySpawn()
		: m_UniqueID( 0 )
		, m_PosX( 0 )
		, m_PosY( 0 )
		, m_PosZ( 0 )
		, m_Rotation( 0 )
		, m_Pitch( 0 )
		, m_CurrentItem( 0 )
	{ m_PacketID = E_NAMED_ENTITY_SPAWN;  }
	virtual cPacket* Clone() const { return new cPacket_NamedEntitySpawn(*this); }

	bool Send( cSocket & a_Socket );

	int m_UniqueID;
	std::string m_PlayerName;
	int m_PosX;	// Pixel position, devide by 32 for block position
	int m_PosY;
	int m_PosZ;
	char m_Rotation;
	char m_Pitch;
	short m_CurrentItem;

	static const unsigned int c_Size = 1 + 4 + 2 + 4 + 4 + 4 + 1 + 1 + 2; // Minimum size
};
