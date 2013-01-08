
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Window.h"
#include "../Item.h"
#include "../ClientHandle.h"
#include "../Player.h"
#include "../Pickup.h"
#include "../Inventory.h"
#include "WindowOwner.h"
#include "../Items/ItemHandler.h"
#include "SlotArea.h"
#include "../ChestEntity.h"





char cWindow::m_WindowIDCounter = 1;





cWindow::cWindow(cWindow::WindowType a_WindowType, const AString & a_WindowTitle)
	: m_WindowID(1 + (m_WindowIDCounter++ % 127))
	, m_WindowType(a_WindowType)
	, m_WindowTitle(a_WindowTitle)
	, m_Owner(NULL)
	, m_IsDestroyed(false)
	, m_ShouldDistributeToHotbarFirst(true)
{
	if (a_WindowType == Inventory)
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
	int a_WindowID, short a_SlotNum, bool a_IsRightClick, bool a_IsShiftPressed, 
	const cItem & a_ClickedItem
)
{
	if (a_WindowID != m_WindowID)
	{
		LOG("WRONG WINDOW ID! (exp %d, got %d) received from \"%s\"", m_WindowID, a_WindowID, a_Player.GetName().c_str());
		return;
	}

	if (a_SlotNum < 0)  // Outside window click
	{
		if (a_IsRightClick)
		{
			a_Player.TossItem(true);
		}
		else
		{
			a_Player.TossItem(true, a_Player.GetDraggingItem().m_ItemCount);
		}
		return;
	}

	int LocalSlotNum = a_SlotNum;
	int idx = 0;
	for (cSlotAreas::iterator itr = m_SlotAreas.begin(), end = m_SlotAreas.end(); itr != end; ++itr)
	{
		if (LocalSlotNum < (*itr)->GetNumSlots())
		{
			(*itr)->Clicked(a_Player, LocalSlotNum, a_IsRightClick, a_IsShiftPressed, a_ClickedItem);
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

	a_Player.GetClientHandle()->SendWindowOpen(m_WindowID, m_WindowType, m_WindowTitle, GetNumSlots() - c_NumInventorySlots);
}





void cWindow::ClosedByPlayer(cPlayer & a_Player)
{
	ASSERT(m_WindowType != Inventory);  // Inventory windows must not be closed (the client would repeat the close packet, looping forever)
	
	// Checks whether the player is still holding an item
	if (a_Player.IsDraggingItem())
	{
		LOGD("Player holds item! Dropping it...");
		a_Player.TossItem(true, a_Player.GetDraggingItem().m_ItemCount);
	}

	cClientHandle * ClientHandle = a_Player.GetClientHandle();
	if (ClientHandle != NULL)
	{
		ClientHandle->SendWindowClose(m_WindowID);
	}

	{
		cCSLock Lock(m_CS);

		for (cSlotAreas::iterator itr = m_SlotAreas.begin(), end = m_SlotAreas.end(); itr != end; ++itr)
		{
			(*itr)->OnPlayerRemoved(a_Player);
		}  // for itr - m_SlotAreas[]

		m_OpenedBy.remove(&a_Player);
		if (m_OpenedBy.empty())
		{
			Destroy();
		}
	}
	if (m_IsDestroyed)
	{
		delete this;
	}
}





void cWindow::OwnerDestroyed()
{
	m_Owner = NULL;
	// Close window for each player. Note that the last one needs special handling
	while (m_OpenedBy.size() > 1)
	{
		(*m_OpenedBy.begin() )->CloseWindow((char)GetWindowType());
	}
	(*m_OpenedBy.begin() )->CloseWindow((char)GetWindowType());
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





void cWindow::BroadcastInventoryProgress(short a_Progressbar, short a_Value)
{
	cCSLock Lock(m_CS);
	for (cPlayerList::iterator itr = m_OpenedBy.begin(); itr != m_OpenedBy.end(); ++itr)
	{
		(*itr)->GetClientHandle()->SendInventoryProgress(m_WindowID, a_Progressbar, a_Value);
	}  // for itr - m_OpenedBy[]
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cInventoryWindow:

cInventoryWindow::cInventoryWindow(cPlayer & a_Player) :
	cWindow(cWindow::Inventory, "MCS-Inventory"),
	m_Player(a_Player)
{
	m_SlotAreas.push_back(new cSlotAreaCrafting(2, *this));  // The creative inventory doesn't display it, but it's still counted into slot numbers
	m_SlotAreas.push_back(new cSlotAreaArmor(*this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cCraftingWindow:

cCraftingWindow::cCraftingWindow(int a_BlockX, int a_BlockY, int a_BlockZ) :
	cWindow(cWindow::Workbench, "MCS-Workbench")
{
	m_SlotAreas.push_back(new cSlotAreaCrafting(3, *this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cChestWindow:

cChestWindow::cChestWindow(cChestEntity * a_Chest) :
	cWindow(cWindow::Chest, "MCS-SingleChest"),
	m_World(a_Chest->GetWorld()),
	m_BlockX(a_Chest->GetPosX()),
	m_BlockY(a_Chest->GetPosY()),
	m_BlockZ(a_Chest->GetPosZ())
{
	m_SlotAreas.push_back(new cSlotAreaChest(a_Chest, *this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
	
	// Play the opening sound:
	m_World->BroadcastSoundEffect("random.chestopen", m_BlockX * 8, m_BlockY * 8, m_BlockZ * 8, 1, 1);

	// Send out the chest-open packet:
	m_World->BroadcastBlockAction(m_BlockX, m_BlockY, m_BlockZ, 1, 1, E_BLOCK_CHEST);
}





cChestWindow::cChestWindow(cChestEntity * a_PrimaryChest, cChestEntity * a_SecondaryChest) :
	cWindow(cWindow::Chest, "MCS-DoubleChest"),
	m_World(a_PrimaryChest->GetWorld()),
	m_BlockX(a_PrimaryChest->GetPosX()),
	m_BlockY(a_PrimaryChest->GetPosY()),
	m_BlockZ(a_PrimaryChest->GetPosZ())
{
	m_SlotAreas.push_back(new cSlotAreaDoubleChest(a_PrimaryChest, a_SecondaryChest, *this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
	
	m_ShouldDistributeToHotbarFirst = false;
	
	// Play the opening sound:
	m_World->BroadcastSoundEffect("random.chestopen", m_BlockX * 8, m_BlockY * 8, m_BlockZ * 8, 1, 1);

	// Send out the chest-open packet:
	m_World->BroadcastBlockAction(m_BlockX, m_BlockY, m_BlockZ, 1, 1, E_BLOCK_CHEST);
}





cChestWindow::~cChestWindow()
{
	// Send out the chest-close packet:
	m_World->BroadcastBlockAction(m_BlockX, m_BlockY, m_BlockZ, 1, 0, E_BLOCK_CHEST);

	m_World->BroadcastSoundEffect("random.chestclosed", m_BlockX * 8, m_BlockY * 8, m_BlockZ * 8, 1, 1);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cDispenserWindow:

cDispenserWindow::cDispenserWindow(int a_BlockX, int a_BlockY, int a_BlockZ, cDispenserEntity * a_Dispenser) :
	cWindow(cWindow::Dispenser, "MCS-Dispenser")
{
	m_SlotAreas.push_back(new cSlotAreaDispenser(a_Dispenser, *this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cFurnaceWindow:

cFurnaceWindow::cFurnaceWindow(int a_BlockX, int a_BlockY, int a_BlockZ, cFurnaceEntity * a_Furnace) :
	cWindow(cWindow::Furnace, "MCS-Furnace")
{
	m_SlotAreas.push_back(new cSlotAreaFurnace(a_Furnace, *this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
}




