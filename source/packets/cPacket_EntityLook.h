
#pragma once

#include "cPacket.h"





class cEntity;
class cPacket_EntityLook : public cPacket
{
public:
	cPacket_EntityLook() 
		: m_UniqueID( 0 )
		, m_Rotation( 0 )
		, m_Pitch( 0 )
	{ m_PacketID = E_ENT_LOOK;  }
	cPacket_EntityLook(cEntity* a_Entity);
	virtual cPacket* Clone() const { return new cPacket_EntityLook(*this); }

	virtual void Serialize(AString & a_Data) const override;

	int m_UniqueID;
	char m_Rotation;
	char m_Pitch;

	static const unsigned int c_Size = 1 + 4 + 1 + 1;
};




