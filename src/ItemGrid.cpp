
// ItemGrid.cpp

// Implements the cItemGrid class representing a storage for items in a XY grid (chests, dispensers, inventory etc.)

#include "Globals.h"
#include "ItemGrid.h"
#include "Items/ItemHandler.h"
#include "Noise/Noise.h"





cItemGrid::cItemGrid(int a_Width, int a_Height):
	m_Width(a_Width),
	m_Height(a_Height),
	m_Slots(a_Width * a_Height),
	m_IsInTriggerListeners(false)
{
}





bool cItemGrid::IsValidSlotNum(int a_SlotNum) const
{
	return ((a_SlotNum >= 0) && (a_SlotNum < m_Slots.size()));
}





bool cItemGrid::IsValidSlotCoords(int a_X, int a_Y) const
{
	return (
		(a_X >= 0) && (a_X < m_Width) &&
		(a_Y >= 0) && (a_Y < m_Height)
	);
}





int cItemGrid::GetSlotNum(int a_X, int a_Y) const
{
	if (!IsValidSlotCoords(a_X, a_Y))
	{
		LOGWARNING("%s: coords out of range: (%d, %d) in grid of size (%d, %d)",
			__FUNCTION__, a_X, a_Y, m_Width, m_Height
		);
		return -1;
	}
	return a_X + m_Width * a_Y;
}





void cItemGrid::GetSlotCoords(int a_SlotNum, int & a_X, int & a_Y) const
{
	if (!IsValidSlotNum(a_SlotNum))
	{
		LOGWARNING("%s: SlotNum out of range: %d in grid of range %d",
			__FUNCTION__, a_SlotNum, m_Slots.size()
		);
		a_X = -1;
		a_Y = -1;
		return;
	}
	a_X = a_SlotNum % m_Width;
	a_Y = a_SlotNum / m_Width;
}





void cItemGrid::CopyFrom(const cItemGrid & a_Src)
{
	m_Width = a_Src.m_Width;
	m_Height = a_Src.m_Height;
	m_Slots = a_Src.m_Slots;

	// The listeners are not copied
}





const cItem & cItemGrid::GetSlot(int a_X, int a_Y) const
{
	return GetSlot(GetSlotNum(a_X, a_Y));
}





const cItem & cItemGrid::GetSlot(int a_SlotNum) const
{
	if (!IsValidSlotNum(a_SlotNum))
	{
		LOGWARNING("%s: Invalid slot number, %d out of %d slots",
			__FUNCTION__, a_SlotNum, m_Slots.size()
		);
		a_SlotNum = 0;
	}
	return m_Slots.GetAt(a_SlotNum);
}





void cItemGrid::SetSlot(int a_X, int a_Y, const cItem & a_Item)
{
	SetSlot(GetSlotNum(a_X, a_Y), a_Item);
}





void cItemGrid::SetSlot(int a_X, int a_Y, short a_ItemType, char a_ItemCount, short a_ItemDamage)
{
	SetSlot(GetSlotNum(a_X, a_Y), cItem(a_ItemType, a_ItemCount, a_ItemDamage));
}





void cItemGrid::SetSlot(int a_SlotNum, const cItem & a_Item)
{
	if (!IsValidSlotNum(a_SlotNum))
	{
		LOGWARNING("%s: Invalid slot number %d out of %d slots",
			__FUNCTION__, a_SlotNum, m_Slots.size()
		);
		return;
	}

	if (!a_Item.IsEmpty() || m_Slots.IsStorageAllocated())
	{
		m_Slots[a_SlotNum] = a_Item;
	}
	TriggerListeners(a_SlotNum);
}





void cItemGrid::SetSlot(int a_SlotNum, short a_ItemType, char a_ItemCount, short a_ItemDamage)
{
	SetSlot(a_SlotNum, cItem(a_ItemType, a_ItemCount, a_ItemDamage));
}





void cItemGrid::EmptySlot(int a_X, int a_Y)
{
	EmptySlot(GetSlotNum(a_X, a_Y));
}





void cItemGrid::EmptySlot(int a_SlotNum)
{
	if (!IsValidSlotNum(a_SlotNum))
	{
		LOGWARNING("%s: Invalid slot number %d out of %d slots",
			__FUNCTION__, a_SlotNum, m_Slots.size()
		);
		return;
	}

	// Check if already empty:
	if (m_Slots.GetAt(a_SlotNum).IsEmpty())
	{
		return;
	}

	// Empty and notify
	m_Slots[a_SlotNum].Empty();
	TriggerListeners(a_SlotNum);
}





bool cItemGrid::IsSlotEmpty(int a_SlotNum) const
{
	if (!IsValidSlotNum(a_SlotNum))
	{
		LOGWARNING("%s: Invalid slot number %d out of %d slots",
			__FUNCTION__, a_SlotNum, m_Slots.size()
		);
		return true;
	}
	return m_Slots.GetAt(a_SlotNum).IsEmpty();
}





bool cItemGrid::IsSlotEmpty(int a_X, int a_Y) const
{
	return IsSlotEmpty(GetSlotNum(a_X, a_Y));
}





void cItemGrid::Clear(void)
{
	if (!m_Slots.IsStorageAllocated())
	{
		return;  // Already clear
	}

	for (int i = 0; i < m_Slots.size(); i++)
	{
		m_Slots[i].Empty();
		TriggerListeners(i);
	}
}





int cItemGrid::HowManyCanFit(const cItem & a_ItemStack, bool a_AllowNewStacks)
{
	int NumLeft = a_ItemStack.m_ItemCount;
	int MaxStack = ItemHandler(a_ItemStack.m_ItemType)->GetMaxStackSize();

	if (!m_Slots.IsStorageAllocated())
	{
		// Grid is empty, all slots are available
		return a_AllowNewStacks ? std::min(NumLeft, m_Slots.size() * MaxStack) : 0;
	}

	for (const auto & Slot : m_Slots)
	{
		if (Slot.IsEmpty())
		{
			if (a_AllowNewStacks)
			{
				NumLeft -= MaxStack;
			}
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
	}  // for Slot - m_Slots[]
	return a_ItemStack.m_ItemCount - NumLeft;
}





int cItemGrid::AddItemToSlot(const cItem & a_ItemStack, int a_Slot, int a_Num, int a_MaxStack)
{
	if (!IsValidSlotNum(a_Slot))
	{
		LOGWARNING("%s: Invalid slot number %d out of %d slots",
			__FUNCTION__, a_Slot, m_Slots.size()
		);
		return 0;
	}

	int PrevCount = 0;
	if (m_Slots[a_Slot].IsEmpty())
	{
		m_Slots[a_Slot] = a_ItemStack;
		PrevCount = 0;
	}
	else
	{
		PrevCount = m_Slots[a_Slot].m_ItemCount;
	}
	m_Slots[a_Slot].m_ItemCount = static_cast<char>(std::min(a_MaxStack, PrevCount + a_Num));
	int toReturn = m_Slots[a_Slot].m_ItemCount - PrevCount;
	TriggerListeners(a_Slot);
	return toReturn;
}





int cItemGrid::AddItem(cItem & a_ItemStack, bool a_AllowNewStacks, int a_PrioritySlot)
{
	int NumLeft = a_ItemStack.m_ItemCount;
	int MaxStack = a_ItemStack.GetMaxStackSize();

	if ((a_PrioritySlot != -1) && !IsValidSlotNum(a_PrioritySlot))
	{
		LOGWARNING("%s: Invalid slot number %d out of %d slots",
			__FUNCTION__, a_PrioritySlot, m_Slots.size()
		);
		a_PrioritySlot = -1;
	}

	if (!a_AllowNewStacks && !m_Slots.IsStorageAllocated())
	{
		return 0;  // No existing stacks to add to
	}

	// Try prioritySlot first:
	if (
		(a_PrioritySlot != -1) &&
		(
			m_Slots[a_PrioritySlot].IsEmpty() ||
			m_Slots[a_PrioritySlot].IsEqual(a_ItemStack)
		)
	)
	{
		NumLeft -= AddItemToSlot(a_ItemStack, a_PrioritySlot, NumLeft, MaxStack);
	}

	// Scan existing stacks:
	for (int i = 0; i < m_Slots.size(); i++)
	{
		if (m_Slots[i].IsEqual(a_ItemStack))
		{
			NumLeft -= AddItemToSlot(a_ItemStack, i, NumLeft, MaxStack);
		}
		if (NumLeft <= 0)
		{
			// All items fit
			return a_ItemStack.m_ItemCount;
		}
	}  // for i - m_Slots[]

	if (!a_AllowNewStacks)
	{
		return (a_ItemStack.m_ItemCount - NumLeft);
	}

	for (int i = 0; i < m_Slots.size(); i++)
	{
		if (m_Slots[i].IsEmpty())
		{
			NumLeft -= AddItemToSlot(a_ItemStack, i, NumLeft, MaxStack);
		}
		if (NumLeft <= 0)
		{
			// All items fit
			return a_ItemStack.m_ItemCount;
		}
	}  // for i - m_Slots[]
	return (a_ItemStack.m_ItemCount - NumLeft);
}





int cItemGrid::AddItems(cItems & a_ItemStackList, bool a_AllowNewStacks, int a_PrioritySlot)
{
	int TotalAdded = 0;
	for (cItems::iterator itr = a_ItemStackList.begin(); itr != a_ItemStackList.end();)
	{
		int NumAdded = AddItem(*itr, a_AllowNewStacks, a_PrioritySlot);
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





int cItemGrid::RemoveItem(const cItem & a_ItemStack)
{
	int NumLeft = a_ItemStack.m_ItemCount;

	if (!m_Slots.IsStorageAllocated())
	{
		return 0;  // No items to remove
	}

	for (int i = 0; i < m_Slots.size(); i++)
	{
		if (NumLeft <= 0)
		{
			break;
		}

		if (m_Slots[i].IsEqual(a_ItemStack))
		{
			int NumToRemove = std::min(NumLeft, static_cast<int>(m_Slots[i].m_ItemCount));
			NumLeft -= NumToRemove;
			m_Slots[i].m_ItemCount -= NumToRemove;

			if (m_Slots[i].m_ItemCount <= 0)
			{
				m_Slots[i].Empty();
			}

			TriggerListeners(i);
		}
	}

	return (a_ItemStack.m_ItemCount - NumLeft);
}





cItem * cItemGrid::FindItem(const cItem & a_RecipeItem)
{
	if (!m_Slots.IsStorageAllocated())
	{
		return nullptr;
	}

	for (int i = 0; i < m_Slots.size(); i++)
	{
		// Items are equal if none is greater the other
		auto compare = cItem::sItemCompare{};
		if (!compare(a_RecipeItem, m_Slots[i]) &&
			!compare(m_Slots[i], a_RecipeItem))
		{
			return &m_Slots[i];
		}
	}

	return nullptr;
}





int cItemGrid::ChangeSlotCount(int a_SlotNum, int a_AddToCount)
{
	if (!IsValidSlotNum(a_SlotNum))
	{
		LOGWARNING("%s: Invalid slot number %d out of %d slots, ignoring the call, returning -1",
			__FUNCTION__, a_SlotNum, m_Slots.size()
		);
		return -1;
	}

	if (m_Slots.GetAt(a_SlotNum).IsEmpty())
	{
		// The item is empty, it's not gonna change
		return 0;
	}

	if (m_Slots[a_SlotNum].m_ItemCount <= -a_AddToCount)
	{
		// Trying to remove more items than there already are, make the item empty
		m_Slots[a_SlotNum].Empty();
		TriggerListeners(a_SlotNum);
		return 0;
	}

	m_Slots[a_SlotNum].m_ItemCount += a_AddToCount;

	cItemHandler * Handler = cItemHandler::GetItemHandler(m_Slots[a_SlotNum].m_ItemType);
	if (m_Slots[a_SlotNum].m_ItemCount > Handler->GetMaxStackSize())
	{
		m_Slots[a_SlotNum].m_ItemCount = Handler->GetMaxStackSize();
	}

	TriggerListeners(a_SlotNum);
	return m_Slots[a_SlotNum].m_ItemCount;
}





int cItemGrid::ChangeSlotCount(int a_X, int a_Y, int a_AddToCount)
{
	return ChangeSlotCount(GetSlotNum(a_X, a_Y), a_AddToCount);
}





cItem cItemGrid::RemoveOneItem(int a_SlotNum)
{
	if (!IsValidSlotNum(a_SlotNum))
	{
		LOGWARNING("%s: Invalid slot number %d out of %d slots, ignoring the call, returning empty item",
			__FUNCTION__, a_SlotNum, m_Slots.size()
		);
		return cItem();
	}

	// If the slot is empty, return an empty item
	if (m_Slots.GetAt(a_SlotNum).IsEmpty())
	{
		return cItem();
	}

	// Make a copy of the item in slot, set count to 1 and remove one from the slot
	cItem res = m_Slots[a_SlotNum];
	res.m_ItemCount = 1;
	m_Slots[a_SlotNum].m_ItemCount -= 1;

	// Emptying the slot correctly if appropriate
	if (m_Slots[a_SlotNum].m_ItemCount == 0)
	{
		m_Slots[a_SlotNum].Empty();
	}

	// Notify everyone of the change
	TriggerListeners(a_SlotNum);

	// Return the stored one item
	return res;
}





cItem cItemGrid::RemoveOneItem(int a_X, int a_Y)
{
	return RemoveOneItem(GetSlotNum(a_X, a_Y));
}





int cItemGrid::HowManyItems(const cItem & a_Item)
{
	if (!m_Slots.IsStorageAllocated())
	{
		return 0;
	}

	int res = 0;
	for (auto & Slot : m_Slots)
	{
		if (Slot.IsEqual(a_Item))
		{
			res += Slot.m_ItemCount;
		}
	}
	return res;
}





bool cItemGrid::HasItems(const cItem & a_ItemStack)
{
	int CurrentlyHave = HowManyItems(a_ItemStack);
	return (CurrentlyHave >= a_ItemStack.m_ItemCount);
}





int cItemGrid::GetFirstEmptySlot(void) const
{
	return GetNextEmptySlot(-1);
}





int cItemGrid::GetFirstUsedSlot(void) const
{
	return GetNextUsedSlot(-1);
}





int cItemGrid::GetLastEmptySlot(void) const
{
	for (int i = m_Slots.size() - 1; i >= 0; i--)
	{
		if (m_Slots.GetAt(i).IsEmpty())
		{
			return i;
		}
	}
	return -1;
}





int cItemGrid::GetLastUsedSlot(void) const
{
	if (!m_Slots.IsStorageAllocated())
	{
		return -1;  // No slots are used
	}

	for (int i = m_Slots.size() - 1; i >= 0; i--)
	{
		if (!m_Slots.GetAt(i).IsEmpty())
		{
			return i;
		}
	}
	return -1;
}





int cItemGrid::GetNextEmptySlot(int a_StartFrom) const
{
	if ((a_StartFrom != -1) && !IsValidSlotNum(a_StartFrom))
	{
		LOGWARNING("%s: Invalid slot number %d out of %d slots",
			__FUNCTION__, a_StartFrom, m_Slots.size()
		);
		a_StartFrom = -1;
	}

	for (int i = a_StartFrom + 1; i < m_Slots.size(); i++)
	{
		if (m_Slots.GetAt(i).IsEmpty())
		{
			return i;
		}
	}
	return -1;
}





int cItemGrid::GetNextUsedSlot(int a_StartFrom) const
{
	if ((a_StartFrom != -1) && !IsValidSlotNum(a_StartFrom))
	{
		LOGWARNING("%s: Invalid slot number %d out of %d slots",
			__FUNCTION__, a_StartFrom, m_Slots.size()
		);
		a_StartFrom = -1;
	}

	if (!m_Slots.IsStorageAllocated())
	{
		return -1;  // No slots are used
	}

	for (int i = a_StartFrom + 1; i < m_Slots.size(); i++)
	{
		if (!m_Slots.GetAt(i).IsEmpty())
		{
			return i;
		}
	}
	return -1;
}





void cItemGrid::CopyToItems(cItems & a_Items) const
{
	if (!m_Slots.IsStorageAllocated())
	{
		return;  // Nothing to copy
	}

	for (const auto & Slot : m_Slots)
	{
		if (!Slot.IsEmpty())
		{
			a_Items.push_back(Slot);
		}
	}  // for Slot - m_Slots[]
}





bool cItemGrid::DamageItem(int a_SlotNum, short a_Amount)
{
	if (!IsValidSlotNum(a_SlotNum))
	{
		LOGWARNING("%s: invalid slot number %d out of %d slots, ignoring.", __FUNCTION__, a_SlotNum, m_Slots.size());
		return false;
	}

	if (!m_Slots.IsStorageAllocated())
	{
		return false;  // Nothing to damage
	}

	return m_Slots[a_SlotNum].DamageItem(a_Amount);
}





bool cItemGrid::DamageItem(int a_X, int a_Y, short a_Amount)
{
	return DamageItem(GetSlotNum(a_X, a_Y), a_Amount);
}





void cItemGrid::GenerateRandomLootWithBooks(const cLootProbab * a_LootProbabs, size_t a_CountLootProbabs, int a_NumSlots, int a_Seed)
{
	// Calculate the total weight:
	int TotalProbab = 1;
	for (size_t i = 0; i < a_CountLootProbabs; i++)
	{
		TotalProbab += a_LootProbabs[i].m_Weight;
	}

	// Pick the loot items:
	cNoise Noise(a_Seed);
	for (int i = 0; i < a_NumSlots; i++)
	{
		int Rnd = (Noise.IntNoise1DInt(i) / 7);
		int LootRnd = Rnd % TotalProbab;
		Rnd >>= 8;
		cItem CurrentLoot = cItem(E_ITEM_ENCHANTED_BOOK, 1, 0);

		// Choose the enchantments
		cWeightedEnchantments Enchantments;
		cEnchantments::AddItemEnchantmentWeights(Enchantments, E_ITEM_BOOK, 24 + Noise.IntNoise2DInt(a_Seed, TotalProbab) % 7);
		int NumEnchantments = Noise.IntNoise3DInt(TotalProbab, Rnd, a_Seed) % 5;  // The number of enchantments this book wil get.

		for (int j = 0; j <= NumEnchantments; j++)
		{
			cEnchantments Enchantment = cEnchantments::SelectEnchantmentFromVector(Enchantments, Noise.IntNoise2DInt(NumEnchantments, i));
			CurrentLoot.m_Enchantments.Add(Enchantment);
			cEnchantments::RemoveEnchantmentWeightFromVector(Enchantments, Enchantment);
			cEnchantments::CheckEnchantmentConflictsFromVector(Enchantments, Enchantment);
		}

		for (size_t j = 0; j < a_CountLootProbabs; j++)
		{
			LootRnd -= a_LootProbabs[j].m_Weight;
			if (LootRnd < 0)
			{
				CurrentLoot = a_LootProbabs[j].m_Item;
				if ((a_LootProbabs[j].m_MaxAmount - a_LootProbabs[j].m_MinAmount) > 0)
				{
					CurrentLoot.m_ItemCount = static_cast<char>(a_LootProbabs[j].m_MinAmount + (Rnd % (a_LootProbabs[j].m_MaxAmount - a_LootProbabs[j].m_MinAmount)));
				}
				else
				{
					CurrentLoot.m_ItemCount = static_cast<char>(a_LootProbabs[j].m_MinAmount);
				}
				Rnd >>= 8;
				break;
			}
		}  // for j - a_LootProbabs[]
		SetSlot(Rnd % m_Slots.size(), CurrentLoot);
	}  // for i - NumSlots
}





void cItemGrid::AddListener(cListener & a_Listener)
{
	cCSLock Lock(m_CSListeners);
	ASSERT(!m_IsInTriggerListeners);  // Must not call this while in TriggerListeners()
	m_Listeners.push_back(&a_Listener);
}





void cItemGrid::RemoveListener(cListener & a_Listener)
{
	cCSLock Lock(m_CSListeners);
	ASSERT(!m_IsInTriggerListeners);  // Must not call this while in TriggerListeners()
	for (cListeners::iterator itr = m_Listeners.begin(), end = m_Listeners.end(); itr != end; ++itr)
	{
		if (*itr == &a_Listener)
		{
			m_Listeners.erase(itr);
			return;
		}
	}  // for itr - m_Listeners[]
}





void cItemGrid::TriggerListeners(int a_SlotNum)
{
	cListeners Listeners;
	{
		cCSLock Lock(m_CSListeners);
		m_IsInTriggerListeners = true;
		Listeners = m_Listeners;
	}
	for (cListeners::iterator itr = Listeners.begin(), end = Listeners.end(); itr != end; ++itr)
	{
		(*itr)->OnSlotChanged(this, a_SlotNum);
	}  // for itr - m_Listeners[]
	m_IsInTriggerListeners = false;
}
