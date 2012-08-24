
#pragma once

#include "cPacket.h"





class cPacket_PickupSpawn : public cPacket
{
public:
	cPacket_PickupSpawn()
		: m_UniqueID( 0 )
		, m_ItemType( 0 )
		, m_ItemCount( 0 )
		, m_ItemDamage( 0 )
		, m_PosX( 0 )
		, m_PosY( 0 )
		, m_PosZ( 0 )
		, m_Rotation( 0 )
		, m_Pitch( 0 )
		, m_Roll( 0 )
	{
		m_PacketID = E_PICKUP_SPAWN;
	}
	
	virtual cPacket* Clone() const { return new cPacket_PickupSpawn(*this); }

	virtual int  Parse(cByteBuffer & a_Buffer) override;
	virtual void Serialize(AString & a_Data) const override;

	int   m_UniqueID;
	short m_ItemType;
	char  m_ItemCount;
	short m_ItemDamage;
	int   m_PosX;
	int   m_PosY;
	int   m_PosZ;
	char  m_Rotation;
	char  m_Pitch;
	char  m_Roll;
};




