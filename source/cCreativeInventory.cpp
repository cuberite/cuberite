
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cCreativeInventory.h"
#include "cPlayer.h"
#include "cClientHandle.h"
#include "cWindow.h"
#include "cItem.h"
#include "cRoot.h"

#include <json/json.h>

#include "packets/cPacket_CreativeInventoryAction.h"
#include "packets/cPacket_WholeInventory.h"
#include "packets/cPacket_InventorySlot.h"





cCreativeInventory::cCreativeInventory(cPlayer * a_Owner)
	: cInventory(a_Owner)
{
	
}





cCreativeInventory::~cCreativeInventory()
{
}





void cCreativeInventory::Clicked(
	short a_SlotNum, bool a_IsRightClick, bool a_IsShiftPressed, 
	const cItem & a_HeldItem
)
{
	if (a_SlotNum == -1)
	{
		// object thrown out
		m_Owner->TossItem(false, a_HeldItem.m_ItemCount, a_HeldItem.m_ItemType, a_HeldItem.m_ItemDamage);
		return;
	}
	
	if ((a_SlotNum < c_HotOffset) || (a_SlotNum >= c_NumSlots))
	{
		LOG("%s: Invalid slot (%d) in cCreativeInventory::Clicked(). Ignoring...", m_Owner->GetName().c_str(), a_SlotNum);
		return;
	}

	m_Slots[a_SlotNum] = a_HeldItem;
}





