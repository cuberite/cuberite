#include "cPacket_WindowClick.h"
#include "cPacket_WholeInventory.h"
#include "cPacket_ItemData.h"

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

	cPacket_ItemData Item;

	Item.Parse(m_Socket);

	m_ItemID = Item.m_ItemID;
	m_ItemCount = Item.m_ItemCount;
	m_ItemUses = Item.m_ItemUses;

	m_EnchantNums = Item.m_EnchantNums;

	return true;
}