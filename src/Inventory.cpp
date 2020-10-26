
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Inventory.h"
#include "Entities/Player.h"
#include "ClientHandle.h"
#include "UI/Window.h"
#include "Item.h"
#include "Root.h"
#include "World.h"

#include "json/json.h"

#include "Items/ItemHandler.h"





cInventory::cInventory(cPlayer & a_Owner) :
	m_ArmorSlots    (1, 4),  // 1 x 4 slots
	m_InventorySlots(9, 3),  // 9 x 3 slots
	m_HotbarSlots   (9, 1),  // 9 x 1 slots
	m_ShieldSlots   (1, 1),  // 1 x 1 slots
	m_Owner(a_Owner)
{
	// Ask each ItemGrid to report changes to us:
	m_ArmorSlots.AddListener(*this);
	m_InventorySlots.AddListener(*this);
	m_HotbarSlots.AddListener(*this);
	m_ShieldSlots.AddListener(*this);

	SetEquippedSlotNum(0);
}





void cInventory::Clear(void)
{
	m_ArmorSlots.Clear();
	m_InventorySlots.Clear();
	m_HotbarSlots.Clear();
	m_ShieldSlots.Clear();
}





int cInventory::HowManyCanFit(const cItem & a_ItemStack, bool a_ConsiderEmptySlots)
{
	return HowManyCanFit(a_ItemStack, 0, invNumSlots - 1, a_ConsiderEmptySlots);
}





int cInventory::HowManyCanFit(const cItem & a_ItemStack, int a_BeginSlotNum, int a_EndSlotNum, bool a_ConsiderEmptySlots)
{

	UNUSED(a_ConsiderEmptySlots);
	if ((a_BeginSlotNum < 0) || (a_BeginSlotNum >= invNumSlots))
	{
		LOGWARNING("%s: Bad BeginSlotNum, got %d, there are %d slots; correcting to 0.", __FUNCTION__, a_BeginSlotNum, invNumSlots - 1);
		a_BeginSlotNum = 0;
	}
	if ((a_EndSlotNum < 0) || (a_EndSlotNum >= invNumSlots))
	{
		LOGWARNING("%s: Bad EndSlotNum, got %d, there are %d slots; correcting to %d.", __FUNCTION__, a_BeginSlotNum, invNumSlots, invNumSlots - 1);
		a_EndSlotNum = invNumSlots - 1;
	}
	if (a_BeginSlotNum > a_EndSlotNum)
	{
		std::swap(a_BeginSlotNum, a_EndSlotNum);
	}

	char NumLeft = a_ItemStack.m_ItemCount;
	int MaxStack = ItemHandler(a_ItemStack.m_ItemType)->GetMaxStackSize();
	for (int i = a_BeginSlotNum; i <= a_EndSlotNum; i++)
	{
		const cItem & Slot = GetSlot(i);
		if (Slot.IsEmpty())
		{
			NumLeft -= MaxStack;
		}
		else if (Slot.IsEqual(a_ItemStack))
		{
			NumLeft -= MaxStack - Slot.m_ItemCount;
		}
		if (NumLeft <= 0)
		{
			// All items fit
			return a_ItemStack.m_ItemCount;
		}
	}  // for i - m_Slots[]
	return a_ItemStack.m_ItemCount - NumLeft;
}





int cInventory::AddItem(const cItem & a_Item, bool a_AllowNewStacks)
{
	m_Owner.AddKnownItem(a_Item);

	cItem ToAdd(a_Item);
	int res = 0;

	// When the item is a armor, try to set it directly to the armor slot.
	if (ItemCategory::IsArmor(a_Item.m_ItemType))
	{
		for (int i = 0; i < m_ArmorSlots.GetNumSlots(); i++)
		{
			if (m_ArmorSlots.GetSlot(i).IsEmpty() && cSlotAreaArmor::CanPlaceArmorInSlot(i, a_Item))
			{
				m_ArmorSlots.SetSlot(i, a_Item);
				return a_Item.m_ItemCount;
			}
		}
	}

	// Add to existing stacks in the hotbar.
	res += m_HotbarSlots.AddItem(ToAdd, false);
	ToAdd.m_ItemCount = static_cast<char>(a_Item.m_ItemCount - res);
	if (ToAdd.m_ItemCount == 0)
	{
		return res;
	}

	// Add to existing stacks in main inventory.
	res += m_InventorySlots.AddItem(ToAdd, false);
	ToAdd.m_ItemCount = static_cast<char>(a_Item.m_ItemCount - res);
	if (ToAdd.m_ItemCount == 0)
	{
		return res;
	}

	// All existing stacks are now filled.
	if (!a_AllowNewStacks)
	{
		return res;
	}

	// Try adding new stacks to the hotbar.
	res += m_HotbarSlots.AddItem(ToAdd, true);
	ToAdd.m_ItemCount = static_cast<char>(a_Item.m_ItemCount - res);
	if (ToAdd.m_ItemCount == 0)
	{
		return res;
	}

	// Try adding new stacks to the main inventory.
	res += m_InventorySlots.AddItem(ToAdd, true);
	return res;
}





int cInventory::AddItems(cItems & a_ItemStackList, bool a_AllowNewStacks)
{
	int TotalAdded = 0;
	for (cItems::iterator itr = a_ItemStackList.begin(); itr != a_ItemStackList.end();)
	{
		int NumAdded = AddItem(*itr, a_AllowNewStacks);
		if (itr->m_ItemCount == NumAdded)
		{
			itr = a_ItemStackList.erase(itr);
		}
		else
		{
			itr->m_ItemCount -= NumAdded;
			++itr;
		}
		TotalAdded += NumAdded;
	}
	return TotalAdded;
}





int cInventory::RemoveItem(const cItem & a_ItemStack)
{
	int RemovedItems = m_ShieldSlots.RemoveItem(a_ItemStack);

	if (RemovedItems < a_ItemStack.m_ItemCount)
	{
		RemovedItems += m_HotbarSlots.RemoveItem(a_ItemStack);
	}

	if (RemovedItems < a_ItemStack.m_ItemCount)
	{
		cItem Temp(a_ItemStack);
		Temp.m_ItemCount -= RemovedItems;
		RemovedItems += m_InventorySlots.RemoveItem(Temp);
	}

	return RemovedItems;
}





cItem * cInventory::FindItem(const cItem & a_RecipeItem)
{
	cItem * Item = m_ShieldSlots.FindItem(a_RecipeItem);
	if (Item != nullptr)
	{
		return Item;
	}
	Item = m_HotbarSlots.FindItem(a_RecipeItem);
	if (Item != nullptr)
	{
		return Item;
	}

	return 	m_InventorySlots.FindItem(a_RecipeItem);
}





bool cInventory::RemoveOneEquippedItem(void)
{
	if (m_HotbarSlots.GetSlot(m_EquippedSlotNum).IsEmpty())
	{
		return false;
	}

	m_HotbarSlots.ChangeSlotCount(m_EquippedSlotNum, -1);
	return true;
}





int cInventory::ReplaceOneEquippedItem(const cItem & a_Item, bool a_TryOtherSlots)
{
	// Ignore whether there was an item in the slot to remove.
	RemoveOneEquippedItem();

	auto EquippedItem = GetEquippedItem();
	if (EquippedItem.IsEmpty())
	{
		SetEquippedItem(a_Item);
		return a_Item.m_ItemCount;
	}

	// Handle case when equipped item is the same as the replacement item.
	cItem ItemsToAdd = a_Item;
	if (EquippedItem.IsEqual(ItemsToAdd))
	{
		cItemHandler Handler(ItemsToAdd.m_ItemType);
		auto AmountToAdd = std::min(static_cast<char>(Handler.GetMaxStackSize() - EquippedItem.m_ItemCount), ItemsToAdd.m_ItemCount);

		EquippedItem.m_ItemCount += AmountToAdd;
		SetEquippedItem(EquippedItem);
		ItemsToAdd.m_ItemCount -= AmountToAdd;
	}

	auto ItemsAdded = a_Item.m_ItemCount - ItemsToAdd.m_ItemCount;

	if (ItemsToAdd.m_ItemCount == 0)
	{
		return ItemsAdded;
	}

	if (!a_TryOtherSlots)
	{
		return ItemsAdded;
	}

	// Try the rest of the inventory.
	return AddItem(ItemsToAdd) + ItemsAdded;
}





int cInventory::HowManyItems(const cItem & a_Item)
{
	return
		m_ArmorSlots.HowManyItems(a_Item) +
		m_InventorySlots.HowManyItems(a_Item) +
		m_HotbarSlots.HowManyItems(a_Item) +
		m_ShieldSlots.HowManyItems(a_Item);
}





bool cInventory::HasItems(const cItem & a_ItemStack)
{
	int CurrentlyHave = HowManyItems(a_ItemStack);
	return (CurrentlyHave >= a_ItemStack.m_ItemCount);
}





void cInventory::SetSlot(int a_SlotNum, const cItem & a_Item)
{
	if ((a_SlotNum < 0) || (a_SlotNum >= invNumSlots))
	{
		LOGWARNING("%s: requesting an invalid slot index: %d out of %d. Ignoring.", __FUNCTION__, a_SlotNum, invNumSlots - 1);
		return;
	}

	int GridSlotNum = 0;
	cItemGrid * Grid = GetGridForSlotNum(a_SlotNum, GridSlotNum);
	if (Grid == nullptr)
	{
		LOGWARNING("%s(%d): requesting an invalid itemgrid. Ignoring.", __FUNCTION__, a_SlotNum);
		return;
	}
	Grid->SetSlot(GridSlotNum, a_Item);
}





void cInventory::SetArmorSlot(int a_ArmorSlotNum, const cItem & a_Item)
{
	m_ArmorSlots.SetSlot(a_ArmorSlotNum, a_Item);
}





void cInventory::SetInventorySlot(int a_InventorySlotNum, const cItem & a_Item)
{
	m_InventorySlots.SetSlot(a_InventorySlotNum, a_Item);
}





void cInventory::SetHotbarSlot(int a_HotBarSlotNum, const cItem & a_Item)
{
	m_HotbarSlots.SetSlot(a_HotBarSlotNum, a_Item);
}





void cInventory::SetShieldSlot(const cItem & a_Item)
{
	m_ShieldSlots.SetSlot(0, a_Item);
}





void cInventory::SetEquippedItem(const cItem & a_Item)
{
	SetHotbarSlot(GetEquippedSlotNum(), a_Item);
}





void cInventory::SendEquippedSlot()
{
	int EquippedSlotNum = cInventory::invArmorCount + cInventory::invInventoryCount + GetEquippedSlotNum();
	SendSlot(EquippedSlotNum);
}





const cItem & cInventory::GetSlot(int a_SlotNum) const
{
	if ((a_SlotNum < 0) || (a_SlotNum >= invNumSlots))
	{
		LOGWARNING("%s: requesting an invalid slot index: %d out of %d. Returning the first inventory slot instead.", __FUNCTION__, a_SlotNum, invNumSlots - 1);
		return m_InventorySlots.GetSlot(0);
	}
	int GridSlotNum = 0;
	const cItemGrid * Grid = GetGridForSlotNum(a_SlotNum, GridSlotNum);
	if (Grid == nullptr)
	{
		// Something went wrong, but we don't know what. We must return a value, so return the first inventory slot
		LOGWARNING("%s(%d): requesting an invalid ItemGrid, returning the first inventory slot instead.", __FUNCTION__, a_SlotNum);
		return m_InventorySlots.GetSlot(0);
	}
	return Grid->GetSlot(GridSlotNum);
}





const cItem & cInventory::GetArmorSlot(int a_ArmorSlotNum) const
{
	if ((a_ArmorSlotNum < 0) || (a_ArmorSlotNum >= invArmorCount))
	{
		LOGWARNING("%s: requesting an invalid slot index: %d out of %d. Returning the first one instead", __FUNCTION__, a_ArmorSlotNum, invArmorCount - 1);
		return m_ArmorSlots.GetSlot(0);
	}
	return m_ArmorSlots.GetSlot(a_ArmorSlotNum);
}





const cItem & cInventory::GetInventorySlot(int a_InventorySlotNum) const
{
	if ((a_InventorySlotNum < 0) || (a_InventorySlotNum >= invInventoryCount))
	{
		LOGWARNING("%s: requesting an invalid slot index: %d out of %d. Returning the first one instead", __FUNCTION__, a_InventorySlotNum, invInventoryCount - 1);
		return m_InventorySlots.GetSlot(0);
	}
	return m_InventorySlots.GetSlot(a_InventorySlotNum);
}





const cItem & cInventory::GetHotbarSlot(int a_SlotNum) const
{
	if ((a_SlotNum < 0) || (a_SlotNum >= invHotbarCount))
	{
		LOGWARNING("%s: requesting an invalid slot index: %d out of %d. Returning the first one instead", __FUNCTION__, a_SlotNum, invHotbarCount - 1);
		return m_HotbarSlots.GetSlot(0);
	}
	return m_HotbarSlots.GetSlot(a_SlotNum);
}





const cItem & cInventory::GetShieldSlot() const
{
	return m_ShieldSlots.GetSlot(0);
}





const cItem & cInventory::GetEquippedItem(void) const
{
	return GetHotbarSlot(m_EquippedSlotNum);
}





void cInventory::SetEquippedSlotNum(int a_SlotNum)
{
	if ((a_SlotNum < 0) || (a_SlotNum >= invHotbarCount))
	{
		LOGWARNING("%s: requesting invalid slot index: %d out of %d. Setting 0 instead.", __FUNCTION__, a_SlotNum, invHotbarCount - 1);
		m_EquippedSlotNum = 0;
	}
	else
	{
		m_EquippedSlotNum = a_SlotNum;
	}
}





bool cInventory::DamageEquippedItem(short a_Amount)
{
	return DamageItem(invHotbarOffset + m_EquippedSlotNum, a_Amount);
}





int cInventory::ChangeSlotCount(int a_SlotNum, int a_AddToCount)
{
	int GridSlotNum = 0;
	cItemGrid * Grid = GetGridForSlotNum(a_SlotNum, GridSlotNum);
	if (Grid == nullptr)
	{
		LOGWARNING("%s: invalid slot number, expected 0 .. %d, got %d; ignoring", __FUNCTION__, invNumSlots, a_SlotNum);
		return -1;
	}
	return Grid->ChangeSlotCount(GridSlotNum, a_AddToCount);
}





bool cInventory::DamageItem(int a_SlotNum, short a_Amount)
{
	if ((a_SlotNum < 0) || (a_SlotNum >= invNumSlots))
	{
		LOGWARNING("%s: requesting an invalid slot index: %d out of %d", __FUNCTION__, a_SlotNum, invNumSlots - 1);
		return false;
	}
	if (a_Amount <= 0)
	{
		return false;
	}

	int GridSlotNum = 0;
	cItemGrid * Grid = GetGridForSlotNum(a_SlotNum, GridSlotNum);
	if (Grid == nullptr)
	{
		LOGWARNING("%s(%d, %d): requesting an invalid grid, ignoring.", __FUNCTION__, a_SlotNum, a_Amount);
		return false;
	}
	if (!Grid->DamageItem(GridSlotNum, a_Amount))
	{
		// The item has been damaged, but did not break yet
		SendSlot(a_SlotNum);
		return false;
	}

	// The item has broken, remove it:
	Grid->EmptySlot(GridSlotNum);
	return true;
}





void cInventory::CopyToItems(cItems & a_Items)
{
	m_ArmorSlots.CopyToItems(a_Items);
	m_InventorySlots.CopyToItems(a_Items);
	m_HotbarSlots.CopyToItems(a_Items);
}





void cInventory::SendSlot(int a_SlotNum)
{
	cItem Item(GetSlot(a_SlotNum));
	if (Item.IsEmpty())
	{
		// Sanitize items that are not completely empty (ie. count == 0, but type != empty)
		Item.Empty();
	}
	m_Owner.GetClientHandle()->SendInventorySlot(0, static_cast<short>(a_SlotNum + 5), Item);  // Slots in the client are numbered "+ 5" because of crafting grid and result
}





/*
int cInventory::MoveItem(short a_ItemType, short a_ItemDamage, int a_Count, int a_BeginSlot, int a_EndSlot)
{
	int res = 0;
	for (int i = a_BeginSlot; i <= a_EndSlot; i++)
	{
		if (
			m_Slots[i].IsEmpty() ||
			((m_Slots[i].m_ItemType == a_ItemType) && (m_Slots[i].m_ItemDamage == a_ItemDamage))
		)
		{
			int MaxCount = ItemHandler(a_ItemType)->GetMaxStackSize();
			ASSERT(m_Slots[i].m_ItemCount <= MaxCount);
			int NumToMove = std::min(a_Count, MaxCount - m_Slots[i].m_ItemCount);
			m_Slots[i].m_ItemCount += NumToMove;
			m_Slots[i].m_ItemDamage = a_ItemDamage;
			m_Slots[i].m_ItemType = a_ItemType;
			SendSlot(i);
			res += NumToMove;
			a_Count -= NumToMove;
			if (a_Count <= 0)
			{
				// No more items to distribute
				return res;
			}
		}
	}  // for i - m_Slots[]
	// No more space to distribute to
	return res;
}
*/





int cInventory::ArmorSlotNumToEntityEquipmentID(short a_ArmorSlotNum)
{
	switch (a_ArmorSlotNum)
	{
		case 0: return 4;  // Helmet
		case 1: return 3;  // Chestplate
		case 2: return 2;  // Leggings
		case 3: return 1;  // Boots
	}
	LOGWARN("%s: invalid armor slot number: %d", __FUNCTION__, a_ArmorSlotNum);
	return 0;
}





#if 0
bool cInventory::AddToBar(cItem & a_Item, const int a_Offset, const int a_Size, bool * a_bChangedSlots, int a_Mode /* = 0 */)
{
	// Fill already present stacks
	if (a_Mode < 2)
	{
		int MaxStackSize = cItemHandler::GetItemHandler(a_Item.m_ItemType)->GetMaxStackSize();
		for (int i = 0; i < a_Size; i++)
		{
			if (
				(m_Slots[i + a_Offset].m_ItemType == a_Item.m_ItemType) &&
				(m_Slots[i + a_Offset].m_ItemCount < MaxStackSize) &&
				(m_Slots[i + a_Offset].m_ItemDamage == a_Item.m_ItemDamage)
			)
			{
				int NumFree = MaxStackSize - m_Slots[i + a_Offset].m_ItemCount;
				if (NumFree >= a_Item.m_ItemCount)
				{

					// printf("1. Adding %i items ( free: %i)\n", a_Item.m_ItemCount, NumFree);
					m_Slots[i + a_Offset].m_ItemCount += a_Item.m_ItemCount;
					a_Item.m_ItemCount = 0;
					a_bChangedSlots[i + a_Offset] = true;
					break;
				}
				else
				{
					// printf("2. Adding %i items\n", NumFree);
					m_Slots[i + a_Offset].m_ItemCount += (char)NumFree;
					a_Item.m_ItemCount -= (char)NumFree;
					a_bChangedSlots[i + a_Offset] = true;
				}
			}
		}
	}

	if (a_Mode > 0)
	{
		// If we got more left, find first empty slot
		for (int i = 0; (i < a_Size) && (a_Item.m_ItemCount > 0); i++)
		{
			if (m_Slots[i + a_Offset].m_ItemType == -1)
			{
				m_Slots[i + a_Offset] = a_Item;
				a_Item.m_ItemCount = 0;
				a_bChangedSlots[i + a_Offset] = true;
			}
		}
	}

	return true;
}
#endif





void cInventory::UpdateItems(void)
{
	const cItem & Slot = GetEquippedItem();
	if (!Slot.IsEmpty())
	{
		ItemHandler(Slot.m_ItemType)->OnUpdate(m_Owner.GetWorld(), &m_Owner, Slot);
	}
}





void cInventory::SaveToJson(Json::Value & a_Value)
{
	// The JSON originally included the 4 crafting slots and the result, so we have to put empty items there, too:
	cItem EmptyItem;
	Json::Value EmptyItemJson;
	EmptyItem.GetJson(EmptyItemJson);
	for (int i = 0; i < 5; i++)
	{
		a_Value.append(EmptyItemJson);
	}

	// The 4 armor slots follow:
	for (int i = 0; i < invArmorCount; i++)
	{
		Json::Value JSON_Item;
		m_ArmorSlots.GetSlot(i).GetJson(JSON_Item);
		a_Value.append(JSON_Item);
	}

	// Next comes the main inventory:
	for (int i = 0; i < invInventoryCount; i++)
	{
		Json::Value JSON_Item;
		m_InventorySlots.GetSlot(i).GetJson(JSON_Item);
		a_Value.append(JSON_Item);
	}

	// The hotbar:
	for (int i = 0; i < invHotbarCount; i++)
	{
		Json::Value JSON_Item;
		m_HotbarSlots.GetSlot(i).GetJson(JSON_Item);
		a_Value.append(JSON_Item);
	}

	// Shield slot is the last
	Json::Value JSON_Item;
	m_ShieldSlots.GetSlot(0).GetJson(JSON_Item);
	a_Value.append(JSON_Item);
}





bool cInventory::LoadFromJson(Json::Value & a_Value)
{
	int SlotIdx = 0;

	for (Json::Value::iterator itr = a_Value.begin(); itr != a_Value.end(); ++itr, SlotIdx++)
	{
		cItem Item;
		Item.FromJson(*itr);

		// The JSON originally included the 4 crafting slots and the result slot, so we need to skip the first 5 items:
		if (SlotIdx < 5)
		{
			continue;
		}

		// If we loaded all the slots, stop now, even if the JSON has more:
		if (SlotIdx - 5 >= invNumSlots)
		{
			break;
		}

		int GridSlotNum = 0;
		cItemGrid * Grid = GetGridForSlotNum(SlotIdx - 5, GridSlotNum);
		ASSERT(Grid != nullptr);
		Grid->SetSlot(GridSlotNum, Item);
	}  // for itr - a_Value[]
	return true;
}





const cItemGrid * cInventory::GetGridForSlotNum(int a_SlotNum, int & a_GridSlotNum) const
{
	ASSERT(a_SlotNum >= 0);

	if (a_SlotNum < invArmorCount)
	{
		a_GridSlotNum = a_SlotNum;
		return &m_ArmorSlots;
	}
	a_SlotNum -= invArmorCount;
	if (a_SlotNum < invInventoryCount)
	{
		a_GridSlotNum = a_SlotNum;
		return &m_InventorySlots;
	}
	a_SlotNum -= invInventoryCount;
	if (a_SlotNum < invHotbarCount)
	{
		a_GridSlotNum = a_SlotNum;
		return &m_HotbarSlots;
	}
	a_GridSlotNum = a_SlotNum - invHotbarCount;
	return &m_ShieldSlots;
}





cItemGrid * cInventory::GetGridForSlotNum(int a_SlotNum, int & a_GridSlotNum)
{
	ASSERT(a_SlotNum >= 0);

	if (a_SlotNum < invArmorCount)
	{
		a_GridSlotNum = a_SlotNum;
		return &m_ArmorSlots;
	}
	a_SlotNum -= invArmorCount;
	if (a_SlotNum < invInventoryCount)
	{
		a_GridSlotNum = a_SlotNum;
		return &m_InventorySlots;
	}
	a_SlotNum -= invInventoryCount;
	if (a_SlotNum < invHotbarCount)
	{
		a_GridSlotNum = a_SlotNum;
		return &m_HotbarSlots;
	}
	a_GridSlotNum = a_SlotNum - invHotbarCount;
	return &m_ShieldSlots;
}





void cInventory::OnSlotChanged(cItemGrid * a_ItemGrid, int a_SlotNum)
{
	// Send the neccessary updates to whoever needs them

	if (!m_Owner.IsTicking())
	{
		// Owner is not (yet) valid, skip for now
		return;
	}

	// Armor update needs broadcast to other players:
	cWorld * World = m_Owner.GetWorld();
	if ((a_ItemGrid == &m_ArmorSlots) && (World != nullptr))
	{
		World->BroadcastEntityEquipment(
			m_Owner, static_cast<short>(ArmorSlotNumToEntityEquipmentID(static_cast<short>(a_SlotNum))),
			m_ArmorSlots.GetSlot(a_SlotNum), m_Owner.GetClientHandle()
		);
	}

	// Broadcast the Equipped Item, if the Slot is changed.
	if ((a_ItemGrid == &m_HotbarSlots) && (m_EquippedSlotNum == a_SlotNum))
	{
		m_Owner.GetWorld()->BroadcastEntityEquipment(m_Owner, 0, GetEquippedItem(), m_Owner.GetClientHandle());
	}

	// Convert the grid-local a_SlotNum to our global SlotNum:
	int Base = 0;
	if (a_ItemGrid == &m_ArmorSlots)
	{
		Base = invArmorOffset;
	}
	else if (a_ItemGrid == &m_InventorySlots)
	{
		Base = invInventoryOffset;
	}
	else if (a_ItemGrid == &m_HotbarSlots)
	{
		Base = invHotbarOffset;
	}
	else if (a_ItemGrid == &m_ShieldSlots)
	{
		Base = invShieldOffset;
	}
	else
	{
		ASSERT(!"Unknown ItemGrid calling OnSlotChanged()");
		return;
	}

	SendSlot(Base + a_SlotNum);
}
