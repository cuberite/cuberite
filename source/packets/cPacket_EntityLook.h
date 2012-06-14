
#pragma once

#include "cPacket.h"





class cEntity;





class cPacket_EntityLook :
	public cPacket
{
public:
	cPacket_EntityLook(void) 
		: m_UniqueID( 0 )
		, m_Rotation( 0 )
		, m_Pitch( 0 )
	{ m_PacketID = E_ENT_LOOK;  }
	cPacket_EntityLook(const cEntity & a_Entity);
	virtual cPacket* Clone(void) const { return new cPacket_EntityLook(*this); }

	virtual void Serialize(AString & a_Data) const override;

	int m_UniqueID;
	char m_Rotation;
	char m_Pitch;
};





class cPacket_EntityHeadLook :
	public cPacket
{
public:
	cPacket_EntityHeadLook(void) 
		: m_UniqueID( 0 )
		, m_HeadYaw( 0 )
	{ m_PacketID = E_ENT_LOOK;  }
	cPacket_EntityHeadLook(const cEntity & a_Entity);
	
	virtual cPacket * Clone(void) const { return new cPacket_EntityHeadLook(*this); }

	virtual void Serialize(AString & a_Data) const override;

	int m_UniqueID;
	char m_HeadYaw;
};




