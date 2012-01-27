#pragma once

#include "cPacket.h"

#include "../BlockID.h"

class cPacket_InventorySlot : public cPacket // Set item [S -> C] ?
{
public:
	cPacket_InventorySlot()
		: m_WindowID( 0 )
		, m_SlotNum( 0 )
		, m_ItemID( E_ITEM_EMPTY )
		, m_ItemCount( 0 )
		, m_ItemUses( 0 )
	{ m_PacketID = E_INVENTORY_SLOT; }
	virtual cPacket* Clone() const { return new cPacket_InventorySlot(*this); }

	bool Send(cSocket & a_Socket);

	char m_WindowID;
	short m_SlotNum;	// Slot
	// 0		= craft result
	// 1-4		= crafting table
	// 5-8		= armor
	// 9-35		= inventory
	// 36-44	= Hot bar

	// Below = item
	short m_ItemID; // if this is -1 the next stuff dont exist
	char m_ItemCount;
	short m_ItemUses;

	static const unsigned int c_Size = 1 + 1 + 2; // Minimal size ( +1+1  = max)
};