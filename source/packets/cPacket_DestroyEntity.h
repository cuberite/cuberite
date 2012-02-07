#pragma once

#include "cPacket.h"





class cEntity;





class cPacket_DestroyEntity : public cPacket
{
public:
	cPacket_DestroyEntity()
		: m_UniqueID( 0 )
	{ m_PacketID = E_DESTROY_ENT;  }
	cPacket_DestroyEntity(cEntity* a_Entity);
	virtual cPacket* Clone() const { return new cPacket_DestroyEntity(*this); }

	virtual void Serialize(AString & a_Data) const override;

	int m_UniqueID;
	
	static const unsigned int c_Size = 1 + 4;
};




