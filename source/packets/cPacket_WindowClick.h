
#pragma once

#include "cPacket.h"





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
		, m_EnchantNums(-1)
	{ m_PacketID = E_WINDOW_CLICK; }
	virtual cPacket* Clone() const { return new cPacket_WindowClick(*this); }

	virtual int Parse(const char * a_Data, int a_Size) override;

	char  m_WindowID;
	short m_SlotNum;	// Slot
	// 0		= craft result
	// 1-4		= crafting table
	// 5-8		= armor
	// 9-35		= inventory
	// 36-44	= Hot bar

	char  m_RightMouse;  // 0 = Left 1 = Right mb
	short m_NumClicks;   // Num clicks
	bool  m_Bool;        // Shift pressed when clicked?

	// Below = item
	short m_ItemID; // if this is -1 the next stuff dont exist
	char  m_ItemCount;
	short m_ItemUses;

	short m_EnchantNums;
};




