#pragma once

#include "cPacket.h"
#include "PacketID.h"

class cEntity;
class cPacket_TeleportEntity : public cPacket
{
public:
	cPacket_TeleportEntity() 
		: m_UniqueID( 0 )
		, m_PosX( 0 )
		, m_PosY( 0 )
		, m_PosZ( 0 )
		, m_Rotation( 0 )
		, m_Pitch( 0 )
	{ m_PacketID = E_ENT_TELEPORT;  }
	virtual cPacket* Clone() const { return new cPacket_TeleportEntity(*this); }
	cPacket_TeleportEntity(cEntity* a_Client);

	bool Send( cSocket & a_Socket );

	int m_UniqueID;
	int m_PosX;	// Pixel position, divide by 32 for block position
	int m_PosY;
	int m_PosZ;
	char m_Rotation;
	char m_Pitch;

	static const unsigned int c_Size = 1 + 4 + 4 + 4 + 4 + 1 + 1;
};
