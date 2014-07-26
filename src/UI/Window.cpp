#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Window.h"
#include "WindowOwner.h"
#include "SlotArea.h"
#include "../Item.h"
#include "../ClientHandle.h"
#include "../Entities/Player.h"
#include "../Entities/Pickup.h"
#include "../Inventory.h"
#include "../Items/ItemHandler.h"
#include "../BlockEntities/ChestEntity.h"
#include "../BlockEntities/DropSpenserEntity.h"
#include "../BlockEntities/EnderChestEntity.h"
#include "../BlockEntities/HopperEntity.h"
#include "../Root.h"
#include "../Bindings/PluginManager.h"




char cWindow::m_WindowIDCounter = 1;





cWindow::cWindow(WindowType a_WindowType, const AString & a_WindowTitle) :
	m_WindowID((++m_WindowIDCounter) % 127),
	m_WindowType(a_WindowType),
	m_WindowTitle(a_WindowTitle),
	m_IsDestroyed(false),
	m_ShouldDistributeToHotbarFirst(true),
	m_Owner(NULL)
{
	if (a_WindowType == wtInventory)
	{
		m_WindowID = 0;
	}
}





cWindow::~cWindow()
{
	for (cSlotAreas::iterator itr = m_SlotAreas.begin(), end = m_SlotAreas.end(); itr != end; ++itr)
	{
		delete *itr;
	}
	m_SlotAreas.clear();
}





int cWindow::GetNumSlots(void) const
{
	int res = 0;
	for (cSlotAreas::const_iterator itr = m_SlotAreas.begin(), end = m_SlotAreas.end(); itr != end; ++itr)
	{
		res += (*itr)->GetNumSlots();
	}  // for itr - m_SlotAreas[]
	return res;
}





const cItem * cWindow::GetSlot(cPlayer & a_Player, int a_SlotNum) const
{
	// Return the item at the specified slot for the specified player
	int LocalSlotNum = 0;
	const cSlotArea * Area = GetSlotArea(a_SlotNum, LocalSlotNum);
	if (Area == NULL)
	{
		LOGWARNING("%s: requesting item from an invalid SlotArea (SlotNum %d), returning NULL.", __FUNCTION__, a_SlotNum);
		return NULL;
	}
	return Area->GetSlot(LocalSlotNum, a_Player);
}





void cWindow::SetSlot(cPlayer & a_Player, int a_SlotNum, const cItem & a_Item)
{
	// Set the item to the specified slot for the specified player
	int LocalSlotNum = 0;
	cSlotArea * Area = GetSlotArea(a_SlotNum, LocalSlotNum);
	if (Area == NULL)
	{
		LOGWARNING("%s: requesting write to an invalid SlotArea (SlotNum %d), ignoring.", __FUNCTION__, a_SlotNum);
		return;
	}
	Area->SetSlot(LocalSlotNum, a_Player, a_Item);
}





bool cWindow::IsSlotInPlayerMainInventory(int a_SlotNum) const
{
	// Returns true if the specified slot is in the Player Main Inventory slotarea
	// The player main inventory is always 27 slots, 9 slots from the end of the inventory
	return ((a_SlotNum >= GetNumSlots() - 36) && (a_SlotNum < GetNumSlots() - 9));
}





bool cWindow::IsSlotInPlayerHotbar(int a_SlotNum) const
{
	// Returns true if the specified slot is in the Player Hotbar slotarea
	// The hotbar is always the last 9 slots
	return ((a_SlotNum >= GetNumSlots() - 9) && (a_SlotNum < GetNumSlots()));
}





bool cWindow::IsSlotInPlayerInventory(int a_SlotNum) const
{
	// Returns true if the specified slot is in the Player Main Inventory or Hotbar slotareas. Note that returns false for Armor.
	// The player combined inventory is always the last 36 slots
	return ((a_SlotNum >= GetNumSlots() - 36) && (a_SlotNum < GetNumSlots()));
}





void cWindow::GetSlots(cPlayer & a_Player, cItems & a_Slots) const
{
	a_Slots.clear();
	a_Slots.reserve(GetNumSlots());
	for (cSlotAreas::const_iterator itr = m_SlotAreas.begin(), end = m_SlotAreas.end(); itr != end; ++itr)
	{
		int NumSlots = (*itr)->GetNumSlots();
		for (int i = 0; i < NumSlots; i++)
		{
			const cItem * Item = (*itr)->GetSlot(i, a_Player);
			if (Item == NULL)
			{
				a_Slots.push_back(cItem());
			}
			else
			{
				a_Slots.push_back(*Item);
			}
		}
	}  // for itr - m_SlotAreas[]
}





void cWindow::Clicked(
	cPlayer & a_Player,
	int a_WindowID, short a_SlotNum, eClickAction a_ClickAction,
	const cItem & a_ClickedItem
)
{
	cPluginManager * PlgMgr = cRoot::Get()->GetPluginManager();
	if (a_WindowID != m_WindowID)
	{
		LOGWARNING("%s: Wrong window ID (exp %d, got %d) received from \"%s\"; ignoring click.", __FUNCTION__, m_WindowID, a_WindowID, a_Player.GetName().c_str());
		return;
	}

	switch (a_ClickAction)
	{
		case caLeftClickOutside:
		case caRightClickOutside:
		{
			if (PlgMgr->CallHookPlayerTossingItem(a_Player))
			{
				// A plugin doesn't agree with the tossing. The plugin itself is responsible for handling the consequences (possible inventory mismatch)
				return;
			}
			if (a_Player.IsGameModeCreative())
			{
				a_Player.TossPickup(a_ClickedItem);
			}

			if (a_ClickAction == caLeftClickOutside)
			{
				// Toss all dragged items:
				a_Player.TossHeldItem(a_Player.GetDraggingItem().m_ItemCount);
			}
			else
			{
				// Toss one of the dragged items:
				a_Player.TossHeldItem();
			}
			return;
		}
		case caLeftClickOutsideHoldNothing:
		case caRightClickOutsideHoldNothing:
		{
			// Nothing needed
			return;
		}
		case caLeftPaintBegin:     OnPaintBegin   (a_Player);            return;
		case caRightPaintBegin:    OnPaintBegin   (a_Player);            return;
		case caLeftPaintProgress:  OnPaintProgress(a_Player, a_SlotNum); return;
		case caRightPaintProgress: OnPaintProgress(a_Player, a_SlotNum); return;
		case caLeftPaintEnd:       OnLeftPaintEnd (a_Player);            return;
		case caRightPaintEnd:      OnRightPaintEnd(a_Player);            return;
		default:
		{
			break;
		}
	}
	
	if (a_SlotNum < 0)
	{
		// TODO: Other click actions with irrelevant slot number (FS #371)
		return;
	}

	int LocalSlotNum = a_SlotNum;
	int idx = 0;
	for (cSlotAreas::iterator itr = m_SlotAreas.begin(), end = m_SlotAreas.end(); itr != end; ++itr)
	{
		if (LocalSlotNum < (*itr)->GetNumSlots())
		{
			(*itr)->Clicked(a_Player, LocalSlotNum, a_ClickAction, a_ClickedItem);
			return;
		}
		LocalSlotNum -= (*itr)->GetNumSlots();
		idx++;
	}
	
	LOGWARNING("Slot number higher than available window slots: %d, max %d received from \"%s\"; ignoring.",
		a_SlotNum, GetNumSlots(), a_Player.GetName().c_str()
	);
}





void cWindow::OpenedByPlayer(cPlayer & a_Player)
{
	{
		cCSLock Lock(m_CS);
		// If player is already in OpenedBy remove player first
		m_OpenedBy.remove(&a_Player);
		// Then add player
		m_OpenedBy.push_back(&a_Player);
		
		for (cSlotAreas::iterator itr = m_SlotAreas.begin(), end = m_SlotAreas.end(); itr != end; ++itr)
		{
			(*itr)->OnPlayerAdded(a_Player);
		}  // for itr - m_SlotAreas[]
	}

	a_Player.GetClientHandle()->SendWindowOpen(*this);
}





bool cWindow::ClosedByPlayer(cPlayer & a_Player, bool a_CanRefuse)
{
	// Checks whether the player is still holding an item
	if (a_Player.IsDraggingItem())
	{
		LOGD("Player holds item! Dropping it...");
		a_Player.TossHeldItem(a_Player.GetDraggingItem().m_ItemCount);
	}

	cClientHandle * ClientHandle = a_Player.GetClientHandle();
	if (ClientHandle != NULL)
	{
		ClientHandle->SendWindowClose(*this);
	}

	{
		cCSLock Lock(m_CS);

		for (cSlotAreas::iterator itr = m_SlotAreas.begin(), end = m_SlotAreas.end(); itr != end; ++itr)
		{
			(*itr)->OnPlayerRemoved(a_Player);
		}  // for itr - m_SlotAreas[]

		m_OpenedBy.remove(&a_Player);
		
		if ((m_WindowType != wtInventory) && m_OpenedBy.empty())
		{
			Destroy();
		}
	}
	if (m_IsDestroyed)
	{
		delete this;
	}
	
	return true;
}





void cWindow::OwnerDestroyed()
{
	m_Owner = NULL;
	// Close window for each player. Note that the last one needs special handling
	while (m_OpenedBy.size() > 1)
	{
		(*m_OpenedBy.begin())->CloseWindow();
	}
	(*m_OpenedBy.begin())->CloseWindow();
}





bool cWindow::ForEachPlayer(cItemCallback<cPlayer> & a_Callback)
{
	cCSLock Lock(m_CS);
	for (cPlayerList::iterator itr = m_OpenedBy.begin(), end = m_OpenedBy.end(); itr != end; ++itr)
	{
		if (a_Callback.Item(*itr))
		{
			return false;
		}
	}  // for itr - m_OpenedBy[]
	return true;
}





bool cWindow::ForEachClient(cItemCallback<cClientHandle> & a_Callback)
{
	cCSLock Lock(m_CS);
	for (cPlayerList::iterator itr = m_OpenedBy.begin(), end = m_OpenedBy.end(); itr != end; ++itr)
	{
		if (a_Callback.Item((*itr)->GetClientHandle()))
		{
			return false;
		}
	}  // for itr - m_OpenedBy[]
	return true;
}





void cWindow::DistributeStack(cItem & a_ItemStack, cPlayer & a_Player, cSlotArea * a_ExcludeArea, bool a_ShouldApply)
{
	// Ask each slot area to take as much of the stack as it can.
	// First ask only slots that already have the same kind of item
	// Then ask any remaining slots
	for (int Pass = 0; Pass < 2; ++Pass)
	{
		if (m_ShouldDistributeToHotbarFirst)
		{
			// First distribute into the hotbar:
			if (a_ExcludeArea != m_SlotAreas.back())
			{
				m_SlotAreas.back()->DistributeStack(a_ItemStack, a_Player, a_ShouldApply, (Pass == 0));
				if (a_ItemStack.IsEmpty())
				{
					// Distributed it all
					return;
				}
			}
		}
		
		// The distribute to all other areas:
		cSlotAreas::iterator end = m_ShouldDistributeToHotbarFirst ? (m_SlotAreas.end() - 1) : m_SlotAreas.end();
		for (cSlotAreas::iterator itr = m_SlotAreas.begin(); itr != end; ++itr)
		{
			if (*itr == a_ExcludeArea)
			{
				continue;
			}
			(*itr)->DistributeStack(a_ItemStack, a_Player, a_ShouldApply, (Pass == 0));
			if (a_ItemStack.IsEmpty())
			{
				// Distributed it all
				return;
			}
		}  // for itr - m_SlotAreas[]
	}  // for Pass - repeat twice
}





bool cWindow::CollectItemsToHand(cItem & a_Dragging, cSlotArea & a_Area, cPlayer & a_Player, bool a_CollectFullStacks)
{
	// First ask the slot areas from a_Area till the end of list:
	bool ShouldCollect = false;
	for (cSlotAreas::iterator itr = m_SlotAreas.begin(), end = m_SlotAreas.end(); itr != end; ++itr)
	{
		if (&a_Area == *itr)
		{
			ShouldCollect = true;
		}
		if (!ShouldCollect)
		{
			continue;
		}
		if ((*itr)->CollectItemsToHand(a_Dragging, a_Player, a_CollectFullStacks))
		{
			// a_Dragging is full
			return true;
		}
	}
	
	// a_Dragging still not full, ask slot areas before a_Area in the list:
	for (cSlotAreas::iterator itr = m_SlotAreas.begin(), end = m_SlotAreas.end(); itr != end; ++itr)
	{
		if (*itr == &a_Area)
		{
			// All areas processed
			return false;
		}
		if ((*itr)->CollectItemsToHand(a_Dragging, a_Player, a_CollectFullStacks))
		{
			// a_Dragging is full
			return true;
		}
	}
	// Shouldn't reach here
	// a_Area is expected to be part of m_SlotAreas[], so the "return false" in the loop above should have returned already
	ASSERT(!"This branch should not be reached");
	return false;
}





void cWindow::SendSlot(cPlayer & a_Player, cSlotArea * a_SlotArea, int a_RelativeSlotNum)
{
	int SlotBase = 0;
	bool Found = false;
	for (cSlotAreas::iterator itr = m_SlotAreas.begin(), end = m_SlotAreas.end(); itr != end; ++itr)
	{
		if (*itr == a_SlotArea)
		{
			Found = true;
			break;
		}
		SlotBase += (*itr)->GetNumSlots();
	}  // for itr - m_SlotAreas[]
	if (!Found)
	{
		LOGERROR("cWindow::SendSlot(): unknown a_SlotArea");
		ASSERT(!"cWindow::SendSlot(): unknown a_SlotArea");
		return;
	}
	
	a_Player.GetClientHandle()->SendInventorySlot(
		m_WindowID, a_RelativeSlotNum + SlotBase, *(a_SlotArea->GetSlot(a_RelativeSlotNum, a_Player))
	);
}





void cWindow::Destroy(void)
{
	if (m_Owner != NULL)
	{
		m_Owner->CloseWindow();
		m_Owner = NULL;
	}
	m_IsDestroyed = true;
}





cSlotArea * cWindow::GetSlotArea(int a_GlobalSlotNum, int & a_LocalSlotNum)
{
	if ((a_GlobalSlotNum < 0) || (a_GlobalSlotNum >= GetNumSlots()))
	{
		LOGWARNING("%s: requesting an invalid SlotNum: %d out of %d slots", __FUNCTION__, a_GlobalSlotNum, GetNumSlots() - 1);
		ASSERT(!"Invalid SlotNum");
		return NULL;
	}
	
	// Iterate through all the SlotAreas, find the correct one
	int LocalSlotNum = a_GlobalSlotNum;
	for (cSlotAreas::iterator itr = m_SlotAreas.begin(), end = m_SlotAreas.end(); itr != end; ++itr)
	{
		if (LocalSlotNum < (*itr)->GetNumSlots())
		{
			a_LocalSlotNum = LocalSlotNum;
			return *itr;
		}
		LocalSlotNum -= (*itr)->GetNumSlots();
	}  // for itr - m_SlotAreas[]
	
	// We shouldn't be here - the check at the beginnning should prevent this. Log and assert
	LOGWARNING("%s: GetNumSlots() is out of sync: %d; LocalSlotNum = %d", __FUNCTION__, GetNumSlots(), LocalSlotNum);
	ASSERT(!"Invalid GetNumSlots");
	return NULL;
}





const cSlotArea * cWindow::GetSlotArea(int a_GlobalSlotNum, int & a_LocalSlotNum) const
{
	if ((a_GlobalSlotNum < 0) || (a_GlobalSlotNum >= GetNumSlots()))
	{
		LOGWARNING("%s: requesting an invalid SlotNum: %d out of %d slots", __FUNCTION__, a_GlobalSlotNum, GetNumSlots() - 1);
		ASSERT(!"Invalid SlotNum");
		return NULL;
	}
	
	// Iterate through all the SlotAreas, find the correct one
	int LocalSlotNum = a_GlobalSlotNum;
	for (cSlotAreas::const_iterator itr = m_SlotAreas.begin(), end = m_SlotAreas.end(); itr != end; ++itr)
	{
		if (LocalSlotNum < (*itr)->GetNumSlots())
		{
			a_LocalSlotNum = LocalSlotNum;
			return *itr;
		}
		LocalSlotNum -= (*itr)->GetNumSlots();
	}  // for itr - m_SlotAreas[]
	
	// We shouldn't be here - the check at the beginnning should prevent this. Log and assert
	LOGWARNING("%s: GetNumSlots() is out of sync: %d; LocalSlotNum = %d", __FUNCTION__, GetNumSlots(), LocalSlotNum);
	ASSERT(!"Invalid GetNumSlots");
	return NULL;
}





void cWindow::OnPaintBegin(cPlayer & a_Player)
{
	// Prepares the internal structures for inventory painting from the specified player
	a_Player.ClearInventoryPaintSlots();
}





void cWindow::OnPaintProgress(cPlayer & a_Player, int a_SlotNum)
{
	// Add the slot to the internal structures for inventory painting by the specified player
	a_Player.AddInventoryPaintSlot(a_SlotNum);
}





void cWindow::OnLeftPaintEnd(cPlayer & a_Player)
{
	// Process the entire action stored in the internal structures for inventory painting
	// distribute as many items as possible
	
	const cSlotNums & SlotNums = a_Player.GetInventoryPaintSlots();
	cItem ToDistribute(a_Player.GetDraggingItem());
	int ToEachSlot = (int)ToDistribute.m_ItemCount / (int)SlotNums.size();
	
	int NumDistributed = DistributeItemToSlots(a_Player, ToDistribute, ToEachSlot, SlotNums);
	
	// Remove the items distributed from the dragging item:
	a_Player.GetDraggingItem().m_ItemCount -= NumDistributed;
	if (a_Player.GetDraggingItem().m_ItemCount == 0)
	{
		a_Player.GetDraggingItem().Empty();
	}
	
	SendWholeWindow(*a_Player.GetClientHandle());
}





void cWindow::OnRightPaintEnd(cPlayer & a_Player)
{
	// Process the entire action stored in the internal structures for inventory painting
	// distribute one item into each slot

	const cSlotNums & SlotNums = a_Player.GetInventoryPaintSlots();
	cItem ToDistribute(a_Player.GetDraggingItem());
	
	int NumDistributed = DistributeItemToSlots(a_Player, ToDistribute, 1, SlotNums);
	
	// Remove the items distributed from the dragging item:
	a_Player.GetDraggingItem().m_ItemCount -= NumDistributed;
	if (a_Player.GetDraggingItem().m_ItemCount == 0)
	{
		a_Player.GetDraggingItem().Empty();
	}
	
	SendWholeWindow(*a_Player.GetClientHandle());
}





int cWindow::DistributeItemToSlots(cPlayer & a_Player, const cItem & a_Item, int a_NumToEachSlot, const cSlotNums & a_SlotNums)
{
	if ((size_t)(a_Item.m_ItemCount) < a_SlotNums.size())
	{
		LOGWARNING("%s: Distributing less items (%d) than slots (" SIZE_T_FMT  ")", __FUNCTION__, (int)a_Item.m_ItemCount, a_SlotNums.size());
		// This doesn't seem to happen with the 1.5.1 client, so we don't worry about it for now
		return 0;
	}
	
	// Distribute to individual slots, keep track of how many items were actually distributed (full stacks etc.)
	int NumDistributed = 0;
	for (cSlotNums::const_iterator itr = a_SlotNums.begin(), end = a_SlotNums.end(); itr != end; ++itr)
	{
		int LocalSlotNum = 0;
		cSlotArea * Area = GetSlotArea(*itr, LocalSlotNum);
		if (Area == NULL)
		{
			LOGWARNING("%s: Bad SlotArea for slot %d", __FUNCTION__, *itr);
			continue;
		}
		
		// Modify the item at the slot
		cItem AtSlot(*Area->GetSlot(LocalSlotNum, a_Player));
		int MaxStack = AtSlot.GetMaxStackSize();
		if (AtSlot.IsEmpty())
		{
			// Empty, just move all of it there:
			cItem ToStore(a_Item);
			ToStore.m_ItemCount = std::min(a_NumToEachSlot, (int)MaxStack);
			Area->SetSlot(LocalSlotNum, a_Player, ToStore);
			NumDistributed += ToStore.m_ItemCount;
		}
		else if (AtSlot.IsEqual(a_Item))
		{
			// Occupied, add and cap at MaxStack:
			int CanStore = std::min(a_NumToEachSlot, (int)MaxStack - AtSlot.m_ItemCount);
			AtSlot.m_ItemCount += CanStore;
			Area->SetSlot(LocalSlotNum, a_Player, AtSlot);
			NumDistributed += CanStore;
		}
	}  // for itr - SlotNums[]
	return NumDistributed;
}





void cWindow::BroadcastSlot(cSlotArea * a_Area, int a_LocalSlotNum)
{
	// Translate local slot num into global slot num:
	int SlotNum = 0;
	bool HasFound = false;
	for (cSlotAreas::const_iterator itr = m_SlotAreas.begin(), end = m_SlotAreas.end(); itr != end; ++itr)
	{
		if (a_Area == *itr)
		{
			SlotNum += a_LocalSlotNum;
			HasFound = true;
			break;
		}
		SlotNum += (*itr)->GetNumSlots();
	}  // for itr - m_SlotAreas[]
	if (!HasFound)
	{
		LOGWARNING("%s: Invalid slot area parameter", __FUNCTION__);
		ASSERT(!"Invalid slot area");
		return;
	}
	
	// Broadcast the update packet:
	cCSLock Lock(m_CS);
	for (cPlayerList::iterator itr = m_OpenedBy.begin(); itr != m_OpenedBy.end(); ++itr)
	{
		(*itr)->GetClientHandle()->SendInventorySlot(m_WindowID, SlotNum, *a_Area->GetSlot(a_LocalSlotNum, **itr));
	}  // for itr - m_OpenedBy[]
}





void cWindow::SendWholeWindow(cClientHandle & a_Client)
{
	a_Client.SendWholeInventory(*this);
}





void cWindow::BroadcastWholeWindow(void)
{
	cCSLock Lock(m_CS);
	for (cPlayerList::iterator itr = m_OpenedBy.begin(); itr != m_OpenedBy.end(); ++itr)
	{
		SendWholeWindow(*(*itr)->GetClientHandle());
	}  // for itr - m_OpenedBy[]
}





void cWindow::BroadcastProgress(int a_Progressbar, int a_Value)
{
	cCSLock Lock(m_CS);
	for (cPlayerList::iterator itr = m_OpenedBy.begin(); itr != m_OpenedBy.end(); ++itr)
	{
		(*itr)->GetClientHandle()->SendWindowProperty(*this, a_Progressbar, a_Value);
	}  // for itr - m_OpenedBy[]
}





void cWindow::SetProperty(int a_Property, int a_Value)
{
	cCSLock Lock(m_CS);
	for (cPlayerList::iterator itr = m_OpenedBy.begin(), end = m_OpenedBy.end(); itr != end; ++itr)
	{
		(*itr)->GetClientHandle()->SendWindowProperty(*this, a_Property, a_Value);
	}  // for itr - m_OpenedBy[]
}





void cWindow::SetProperty(int a_Property, int a_Value, cPlayer & a_Player)
{
	a_Player.GetClientHandle()->SendWindowProperty(*this, a_Property, a_Value);
}





////////////////////////////////////////////////////////////////////////////////
// cInventoryWindow:

cInventoryWindow::cInventoryWindow(cPlayer & a_Player) :
	cWindow(wtInventory, "Inventory"),
	m_Player(a_Player)
{
	m_SlotAreas.push_back(new cSlotAreaCrafting(2, *this));  // The creative inventory doesn't display it, but it's still counted into slot numbers
	m_SlotAreas.push_back(new cSlotAreaArmor(*this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
}





////////////////////////////////////////////////////////////////////////////////
// cCraftingWindow:

cCraftingWindow::cCraftingWindow(int a_BlockX, int a_BlockY, int a_BlockZ) :
	cWindow(wtWorkbench, "Crafting Table")
{
	m_SlotAreas.push_back(new cSlotAreaCrafting(3, *this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
}





////////////////////////////////////////////////////////////////////////////////
// cAnvilWindow:

cAnvilWindow::cAnvilWindow(int a_BlockX, int a_BlockY, int a_BlockZ) :
	cWindow(wtAnvil, "Repair"),
	m_RepairedItemName(""),
	m_BlockX(a_BlockX),
	m_BlockY(a_BlockY),
	m_BlockZ(a_BlockZ)
{
	m_AnvilSlotArea = new cSlotAreaAnvil(*this);
	m_SlotAreas.push_back(m_AnvilSlotArea);
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
}





void cAnvilWindow::SetRepairedItemName(const AString & a_Name, cPlayer * a_Player)
{
	m_RepairedItemName = a_Name;

	if (a_Player != NULL)
	{
		m_AnvilSlotArea->UpdateResult(*a_Player);
	}
}





void cAnvilWindow::GetBlockPos(int & a_PosX, int & a_PosY, int & a_PosZ)
{
	a_PosX = m_BlockX;
	a_PosY = m_BlockY;
	a_PosZ = m_BlockZ;
}





////////////////////////////////////////////////////////////////////////////////
// cEnchantingWindow:

cEnchantingWindow::cEnchantingWindow(int a_BlockX, int a_BlockY, int a_BlockZ) :
	cWindow(wtEnchantment, "Enchant"),
	m_SlotArea(),
	m_BlockX(a_BlockX),
	m_BlockY(a_BlockY),
	m_BlockZ(a_BlockZ)
{
	m_SlotAreas.push_back(new cSlotAreaEnchanting(*this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
}





void cEnchantingWindow::SetProperty(int a_Property, int a_Value)
{
	m_PropertyValue[a_Property] = a_Value;

	super::SetProperty(a_Property, a_Value);
}





void cEnchantingWindow::SetProperty(int a_Property, int a_Value, cPlayer & a_Player)
{
	m_PropertyValue[a_Property] = a_Value;

	super::SetProperty(a_Property, a_Value, a_Player);
}





int cEnchantingWindow::GetPropertyValue(int a_Property)
{
	return m_PropertyValue[a_Property];
}





void cEnchantingWindow::GetBlockPos(int & a_PosX, int & a_PosY, int & a_PosZ)
{
	a_PosX = m_BlockX;
	a_PosY = m_BlockY;
	a_PosZ = m_BlockZ;
}





////////////////////////////////////////////////////////////////////////////////
// cChestWindow:

cChestWindow::cChestWindow(cChestEntity * a_Chest) :
	cWindow(wtChest, (a_Chest->GetBlockType() == E_BLOCK_CHEST) ? "Chest" : "Trapped Chest"),
	m_World(a_Chest->GetWorld()),
	m_BlockX(a_Chest->GetPosX()),
	m_BlockY(a_Chest->GetPosY()),
	m_BlockZ(a_Chest->GetPosZ()),
	m_PrimaryChest(a_Chest),
	m_SecondaryChest(NULL)
{
	m_SlotAreas.push_back(new cSlotAreaChest(a_Chest, *this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
	
	// Play the opening sound:
	m_World->BroadcastSoundEffect("random.chestopen", (double)m_BlockX, (double)m_BlockY, (double)m_BlockZ, 1, 1);

	// Send out the chest-open packet:
	m_World->BroadcastBlockAction(m_BlockX, m_BlockY, m_BlockZ, 1, 1, a_Chest->GetBlockType());
}





cChestWindow::cChestWindow(cChestEntity * a_PrimaryChest, cChestEntity * a_SecondaryChest) :
	cWindow(wtChest, (a_PrimaryChest->GetBlockType() == E_BLOCK_CHEST) ? "Double Chest" : "Double Trapped Chest"),
	m_World(a_PrimaryChest->GetWorld()),
	m_BlockX(a_PrimaryChest->GetPosX()),
	m_BlockY(a_PrimaryChest->GetPosY()),
	m_BlockZ(a_PrimaryChest->GetPosZ()),
	m_PrimaryChest(a_PrimaryChest),
	m_SecondaryChest(a_SecondaryChest)
{
	m_SlotAreas.push_back(new cSlotAreaDoubleChest(a_PrimaryChest, a_SecondaryChest, *this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
	
	m_ShouldDistributeToHotbarFirst = false;
	
	// Play the opening sound:
	m_World->BroadcastSoundEffect("random.chestopen", (double)m_BlockX, (double)m_BlockY, (double)m_BlockZ, 1, 1);

	// Send out the chest-open packet:
	m_World->BroadcastBlockAction(m_BlockX, m_BlockY, m_BlockZ, 1, 1, a_PrimaryChest->GetBlockType());
}





void cChestWindow::OpenedByPlayer(cPlayer & a_Player)
{
	int ChunkX, ChunkZ;

	m_PrimaryChest->SetNumberOfPlayers(m_PrimaryChest->GetNumberOfPlayers() + 1);
	cChunkDef::BlockToChunk(m_PrimaryChest->GetPosX(), m_PrimaryChest->GetPosZ(), ChunkX, ChunkZ);
	m_PrimaryChest->GetWorld()->MarkRedstoneDirty(ChunkX, ChunkZ);

	if (m_SecondaryChest != NULL)
	{
		m_SecondaryChest->SetNumberOfPlayers(m_SecondaryChest->GetNumberOfPlayers() + 1);
		cChunkDef::BlockToChunk(m_SecondaryChest->GetPosX(), m_SecondaryChest->GetPosZ(), ChunkX, ChunkZ);
		m_SecondaryChest->GetWorld()->MarkRedstoneDirty(ChunkX, ChunkZ);
	}

	cWindow::OpenedByPlayer(a_Player);
}





bool cChestWindow::ClosedByPlayer(cPlayer & a_Player, bool a_CanRefuse)
{
	int ChunkX, ChunkZ;

	m_PrimaryChest->SetNumberOfPlayers(m_PrimaryChest->GetNumberOfPlayers() - 1);
	cChunkDef::BlockToChunk(m_PrimaryChest->GetPosX(), m_PrimaryChest->GetPosZ(), ChunkX, ChunkZ);
	m_PrimaryChest->GetWorld()->MarkRedstoneDirty(ChunkX, ChunkZ);

	if (m_SecondaryChest != NULL)
	{
		m_SecondaryChest->SetNumberOfPlayers(m_SecondaryChest->GetNumberOfPlayers() - 1);
		cChunkDef::BlockToChunk(m_SecondaryChest->GetPosX(), m_SecondaryChest->GetPosZ(), ChunkX, ChunkZ);
		m_SecondaryChest->GetWorld()->MarkRedstoneDirty(ChunkX, ChunkZ);
	}

	cWindow::ClosedByPlayer(a_Player, a_CanRefuse);
	return true;
}





cChestWindow::~cChestWindow()
{
	// Send out the chest-close packet:
	m_World->BroadcastBlockAction(m_BlockX, m_BlockY, m_BlockZ, 1, 0, m_PrimaryChest->GetBlockType());

	m_World->BroadcastSoundEffect("random.chestclosed", (double)m_BlockX, (double)m_BlockY, (double)m_BlockZ, 1, 1);
}





////////////////////////////////////////////////////////////////////////////////
// cDropSpenserWindow:

cDropSpenserWindow::cDropSpenserWindow(int a_BlockX, int a_BlockY, int a_BlockZ, cDropSpenserEntity * a_DropSpenser) :
	cWindow(wtDropSpenser, (a_DropSpenser->GetBlockType() == E_BLOCK_DISPENSER) ? "Dispenser" : "Dropper")
{
	m_ShouldDistributeToHotbarFirst = false;
	m_SlotAreas.push_back(new cSlotAreaItemGrid(a_DropSpenser->GetContents(), *this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
}





////////////////////////////////////////////////////////////////////////////////
// cEnderChestWindow:

cEnderChestWindow::cEnderChestWindow(cEnderChestEntity * a_EnderChest) :
	cWindow(wtChest, "Ender Chest"),
	m_World(a_EnderChest->GetWorld()),
	m_BlockX(a_EnderChest->GetPosX()),
	m_BlockY(a_EnderChest->GetPosY()),
	m_BlockZ(a_EnderChest->GetPosZ())
{
	m_SlotAreas.push_back(new cSlotAreaEnderChest(a_EnderChest, *this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
	
	// Play the opening sound:
	m_World->BroadcastSoundEffect("random.chestopen", (double)m_BlockX, (double)m_BlockY, (double)m_BlockZ, 1, 1);

	// Send out the chest-open packet:
	m_World->BroadcastBlockAction(m_BlockX, m_BlockY, m_BlockZ, 1, 1, E_BLOCK_ENDER_CHEST);
}





cEnderChestWindow::~cEnderChestWindow()
{
	// Send out the chest-close packet:
	m_World->BroadcastBlockAction(m_BlockX, m_BlockY, m_BlockZ, 1, 0, E_BLOCK_ENDER_CHEST);

	// Play the closing sound
	m_World->BroadcastSoundEffect("random.chestclosed", (double)m_BlockX, (double)m_BlockY, (double)m_BlockZ, 1, 1);
}





////////////////////////////////////////////////////////////////////////////////
// cHopperWindow:

cHopperWindow::cHopperWindow(int a_BlockX, int a_BlockY, int a_BlockZ, cHopperEntity * a_Hopper) :
	super(wtHopper, "Hopper")
{
	m_ShouldDistributeToHotbarFirst = false;
	m_SlotAreas.push_back(new cSlotAreaItemGrid(a_Hopper->GetContents(), *this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
}





////////////////////////////////////////////////////////////////////////////////
// cFurnaceWindow:

cFurnaceWindow::cFurnaceWindow(int a_BlockX, int a_BlockY, int a_BlockZ, cFurnaceEntity * a_Furnace) :
	cWindow(wtFurnace, "Furnace")
{
	m_ShouldDistributeToHotbarFirst = false;
	m_SlotAreas.push_back(new cSlotAreaFurnace(a_Furnace, *this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
}




