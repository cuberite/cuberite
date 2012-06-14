#pragma once

#include "cPacket.h"





class cPacket_EntityStatus : public cPacket
{
public:
	cPacket_EntityStatus()
		: m_UniqueID( 0 )
		, m_Status( 0 )
	{ m_PacketID = E_ENT_STATUS; }
	virtual cPacket* Clone() const { return new cPacket_EntityStatus( *this ); }

	virtual void Serialize(AString & a_Data) const override;

	static const char STATUS_TAKEDAMAGE = 2;
	static const char STATUS_DIE = 3;

	int m_UniqueID;
	char m_Status;

	static const unsigned int c_Size = 1 + 4 + 1;
};




