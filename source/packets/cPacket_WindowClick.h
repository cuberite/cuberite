
#pragma once

#include "cPacket.h"
#include "../cItem.h"





class cPacket_WindowClick : public cPacket // [C -> S]
{
public:
	cPacket_WindowClick()
		: m_WindowID( 0 )
		, m_SlotNum( 0 )
		, m_IsRightClick(false)
		, m_TransactionID( 0 )
		, m_IsShiftPressed( false )
	{
		m_PacketID = E_WINDOW_CLICK;
	}
	
	virtual cPacket* Clone() const { return new cPacket_WindowClick(*this); }

	virtual int Parse(cByteBuffer & a_Buffer) override;

	char  m_WindowID;
	short m_SlotNum;
	bool  m_IsRightClick;
	short m_TransactionID;
	bool  m_IsShiftPressed;  // Shift pressed when clicked?

	cItem m_HeldItem;
};




