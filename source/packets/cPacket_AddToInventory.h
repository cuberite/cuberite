#pragma once

#include "cPacket.h"
#include "PacketID.h"

class cPacket_AddToInventory : public cPacket
{
public:
	cPacket_AddToInventory() 
		: m_ItemType( 0 )
		, m_Count( 0 )
		, m_Life( 0 )
	{ m_PacketID = E_ADD_TO_INV;  }
	virtual cPacket* Clone() const { return new cPacket_AddToInventory(*this); }

	bool Parse( cSocket & a_Socket );
	bool Send( cSocket & a_Socket );

	short m_ItemType;
	char m_Count;
	short m_Life;
	static const unsigned int c_Size = 1 + 2 + 1 + 2;
};