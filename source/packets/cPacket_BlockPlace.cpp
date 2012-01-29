
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_BlockPlace.h"
#include "cPacket_ItemData.h"





bool cPacket_BlockPlace::Parse(cSocket & a_Socket)
{
	m_Socket = a_Socket;
	if( !ReadInteger( m_PosX ) ) return false;
	if( !ReadByte	( m_PosY ) ) return false;
	if( !ReadInteger( m_PosZ ) ) return false;
	if( !ReadByte	( m_Direction ) ) return false;

	/*
	if( !ReadShort	( m_ItemType ) ) return false;
	if( m_ItemType > -1 )
	{
		if( !ReadByte	( m_Count ) ) return false;
		if( !ReadShort	( m_Uses ) ) return false;
	}*/

	cPacket_ItemData Item;

	Item.Parse(m_Socket);

	m_ItemType = Item.m_ItemID;
	m_Count = Item.m_ItemCount;
	m_Uses = Item.m_ItemUses;

	return true;
}