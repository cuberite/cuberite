
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_WholeInventory.h"
#include "../cItem.h"
#include "../cInventory.h"
#include "../cWindow.h"
#include "cPacket_ItemData.h"





cPacket_WholeInventory::cPacket_WholeInventory( const cPacket_WholeInventory & a_Clone )
{
	m_PacketID = E_INVENTORY_WHOLE;
	m_WindowID = a_Clone.m_WindowID;
	m_Count = a_Clone.m_Count;
	m_Items = new cItem[m_Count];
	// TODO: copy items one by one, they may have some values that needn't be shallow-copiable
	memcpy(m_Items, a_Clone.m_Items, sizeof(cItem) * m_Count);
}





cPacket_WholeInventory::cPacket_WholeInventory(cInventory * a_Inventory)
{
	m_PacketID = E_INVENTORY_WHOLE;
	m_WindowID = 0;
	m_Count = a_Inventory->c_NumSlots;
	m_Items = new cItem[m_Count];
	// TODO: copy items one by one, they may have some values that needn't be shallow-copiable
	memcpy( m_Items, a_Inventory->GetSlots(), sizeof(cItem)*m_Count );
}





cPacket_WholeInventory::cPacket_WholeInventory(cWindow * a_Window)
{
	m_PacketID = E_INVENTORY_WHOLE;
	m_WindowID = (char)a_Window->GetWindowID();
	m_Count = (short)a_Window->GetNumSlots();
	m_Items = new cItem[m_Count];
	// TODO: copy items one by one, they may have some values that needn't be shallow-copiable
	memcpy( m_Items, a_Window->GetSlots(), sizeof(cItem) * m_Count);
}





cPacket_WholeInventory::~cPacket_WholeInventory()
{
	delete [] m_Items;
}





void cPacket_WholeInventory::Serialize(AString & a_Data) const
{
	AppendByte (a_Data, m_PacketID);
	AppendByte (a_Data, m_WindowID);
	AppendShort(a_Data, m_Count);

	for (int j = 0; j < m_Count; j++)
	{
		cPacket_ItemData::AppendItem(a_Data, m_Items[j]);
	}
}




 