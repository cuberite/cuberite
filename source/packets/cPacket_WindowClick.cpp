#include "cPacket_WindowClick.h"


bool cPacket_WindowClick::Parse(cSocket & a_Socket)
{
//	LOG("-----------INV66-----------");
	m_Socket = a_Socket;

	if( !ReadByte(m_WindowID) ) return false;
	if( !ReadShort(m_SlotNum) ) return false;
	if( !ReadByte(m_RightMouse) ) return false;
	if( !ReadShort(m_NumClicks) ) return false;
	if( !ReadBool(m_Bool) )	return false;

// 	LOG("WindowID  : %i", m_Type );
// 	LOG("FromSlot: %i", m_SlotNum );
// 	LOG("Right/Le: %i", m_RightMouse );
// 	LOG("NumClick: %i", m_NumClicks );

	if( !ReadShort(m_ItemID) ) return false;
//	LOG("ItemID: %i", m_ItemID );
	if( m_ItemID > -1 )
	{
		if( !ReadByte(m_ItemCount) ) return false;
		if( !ReadShort(m_ItemUses) ) return false;
// 		LOG("Count : %i", m_ItemCount );
// 		LOG("Uses  : %i", m_ItemUses );
	}
	else
	{
	    m_ItemCount = 0;
	    m_ItemUses = 0;
	}

	return true;
}