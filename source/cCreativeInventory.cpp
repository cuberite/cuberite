
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cCreativeInventory.h"
#include "cPlayer.h"
#include "cClientHandle.h"
#include "cWindow.h"
#include "cItem.h"
#include "cRecipeChecker.h"
#include "cRoot.h"

#include <json/json.h>

#include "packets/cPacket_CreativeInventoryAction.h"
#include "packets/cPacket_WholeInventory.h"
#include "packets/cPacket_InventorySlot.h"

cCreativeInventory::~cCreativeInventory()
{
}

cCreativeInventory::cCreativeInventory(cPlayer* a_Owner)
	: cInventory(a_Owner)
{
	
}

void cCreativeInventory::Clicked( cPacket* a_ClickPacket )
{
	cPacket_CreativeInventoryAction* Packet = reinterpret_cast<cPacket_CreativeInventoryAction *>(a_ClickPacket);
	short Slot = Packet->m_Slot;

	cItem* SlotItem = &(this->m_Slots[Slot]);

	SlotItem->m_ItemID = (ENUM_ITEM_ID) Packet->m_ItemID;
	SlotItem->m_ItemHealth = Packet->m_Damage;
	SlotItem->m_ItemCount = Packet->m_Quantity;
}

