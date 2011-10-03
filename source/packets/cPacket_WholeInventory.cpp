#include "cPacket_WholeInventory.h"
#include "../cItem.h"
#include "../cInventory.h"
#include "../cWindow.h"

cPacket_WholeInventory::cPacket_WholeInventory( const cPacket_WholeInventory & a_Clone )
{
	m_PacketID = E_INVENTORY_WHOLE;
	m_WindowID = a_Clone.m_WindowID;
	m_Count = a_Clone.m_Count;
	m_Items = new cItem[m_Count];
	memcpy( m_Items, a_Clone.m_Items, sizeof(cItem)*m_Count );
}

cPacket_WholeInventory::cPacket_WholeInventory( cInventory* a_Inventory )
{
	m_PacketID = E_INVENTORY_WHOLE;
	m_WindowID = 0;
	m_Count = a_Inventory->c_NumSlots;
	m_Items = new cItem[m_Count];
	memcpy( m_Items, a_Inventory->GetSlots(), sizeof(cItem)*m_Count );
}

cPacket_WholeInventory::cPacket_WholeInventory( cWindow* a_Window )
{
	m_PacketID = E_INVENTORY_WHOLE;
	m_WindowID = (char)a_Window->GetWindowID();
	m_Count = (short)a_Window->GetNumSlots();
	m_Items = new cItem[m_Count];
	memcpy( m_Items, a_Window->GetSlots(), sizeof(cItem)*m_Count );
}

cPacket_WholeInventory::~cPacket_WholeInventory()
{
	delete [] m_Items;
}

bool cPacket_WholeInventory::Send(cSocket & a_Socket)
{
	unsigned int TotalSize = c_Size;

	for(int i = 0; i < m_Count; i++)
	{
		if( m_Items[i].m_ItemID > -1 )
			TotalSize+=3;
		TotalSize+=2;
	}

	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte		( (char)m_PacketID,			Message, i );
	AppendByte		( m_WindowID,				Message, i );
	AppendShort		( m_Count,				Message, i );
	for(int j = 0; j < m_Count; j++)
	{
		AppendShort		( (short)m_Items[j].m_ItemID,	Message, i );
		if( m_Items[j].m_ItemID > -1 )
		{
			AppendByte	( m_Items[j].m_ItemCount,	Message, i );
			AppendShort	( m_Items[j].m_ItemHealth,	Message, i );
		}
	}

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}