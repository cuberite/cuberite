#pragma once

#include "cPacket.h"
#include "PacketID.h"

class cPacket_WindowClick : public cPacket // [C -> S]
{
public:
	cPacket_WindowClick()
		: m_WindowID( 0 )
		, m_SlotNum( 0 )
		, m_RightMouse( 0 )
		, m_NumClicks( 0 )
		, m_Bool( false )
		, m_ItemID( 0 )
		, m_ItemCount( 0 )
		, m_ItemUses( 0 )
	{ m_PacketID = E_WINDOW_CLICK; }
	virtual cPacket* Clone() const { return new cPacket_WindowClick(*this); }

	bool Parse(cSocket & a_Socket);

	char m_WindowID;
	short m_SlotNum;	// Slot
	// 0		= craft result
	// 1-4		= crafting table
	// 5-8		= armor
	// 9-35		= inventory
	// 36-44	= Hot bar

	char m_RightMouse;	// 0		= left 1 = Right mb
	short m_NumClicks;	// Num clicks
	bool m_Bool; // unkown????????????

	// Below = item
	short m_ItemID; // if this is -1 the next stuff dont exist
	char m_ItemCount;
	short m_ItemUses;

	static const unsigned int c_Size = 1 + 1 + 2 + 1 + 2 + 2; // Minimal size ( +1+1  = max)
};