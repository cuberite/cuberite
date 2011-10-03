#pragma once

#include "cPacket.h"
#include "PacketID.h"

class cPacket_PickupSpawn : public cPacket
{
public:
	cPacket_PickupSpawn()
		: m_UniqueID( 0 )
		, m_Item( 0 )
		, m_Count( 0 )
		, m_Health( 0 )
		, m_PosX( 0 )
		, m_PosY( 0 )
		, m_PosZ( 0 )
		, m_Rotation( 0 )
		, m_Pitch( 0 )
		, m_Roll( 0 )
	{ m_PacketID = E_PICKUP_SPAWN;  }
	virtual cPacket* Clone() const { return new cPacket_PickupSpawn(*this); }

	bool Parse( cSocket & a_Socket );
	bool Send( cSocket & a_Socket );

	int m_UniqueID;
	short m_Item;
	char m_Count;
	short m_Health;
	int m_PosX;
	int m_PosY;
	int m_PosZ;
	char m_Rotation;
	char m_Pitch;
	char m_Roll;

	static const unsigned int c_Size = 1 + 4 + 2 + 1 + 2 + 4 + 4 + 4 + 1 + 1 + 1;
};
