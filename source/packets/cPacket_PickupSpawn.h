
#pragma once

#include "cPacket.h"





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

	virtual int Parse(const char * a_Data, int a_Size) override;
	virtual void Serialize(AString & a_Data) const override;

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




