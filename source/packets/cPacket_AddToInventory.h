#pragma once

#include "cPacket.h"

#include "../BlockID.h"

class cPacket_AddToInventory : public cPacket
{
public:
	cPacket_AddToInventory() 
		: m_ItemType( E_ITEM_EMPTY )
		, m_Count( 0 )
		, m_Life( 0 )
	{ m_PacketID = E_ADD_TO_INV;  }
	virtual cPacket* Clone() const { return new cPacket_AddToInventory(*this); }

	bool Parse( cSocket & a_Socket );
	bool Send( cSocket & a_Socket );

	ENUM_ITEM_ID m_ItemType;
	char m_Count;
	short m_Life;
	static const unsigned int c_Size = 1;
};