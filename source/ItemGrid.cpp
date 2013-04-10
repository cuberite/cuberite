
// ItemGrid.cpp

// Implements the cItemGrid class representing a storage for items in a XY grid (chests, dispensers, inventory etc.)

#include "Globals.h"
#include "ItemGrid.h"
#include "Items/ItemHandler.h"
#include "Noise.h"





cItemGrid::cItemGrid(int a_Width, int a_Height) :
	m_Width(a_Width),
	m_Height(a_Height),
	m_NumSlots(a_Width * a_Height),
	m_Slots(new cItem[a_Width * a_Height])
{
}





cItemGrid::~cItemGrid()
{
	delete[] m_Slots;
}





int cItemGrid::GetSlotNum(int a_X, int a_Y) const
{
	if (
		(a_X < 0) || (a_X >= m_Width) ||
		(a_Y < 0) || (a_Y >= m_Height)
	)
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
	if ((a_SlotNum < 0) || (a_SlotNum >= m_NumSlots))
	{
		LOGWARNING("%s: SlotNum out of range: %d in grid of range %d",
			__FUNCTION__, a_SlotNum, m_NumSlots
		);
		a_X = -1;
		a_Y = -1;
		return;
	}
	a_X = a_SlotNum % m_Width;
	a_Y = a_SlotNum / m_Width;
}





const cItem & cItemGrid::GetSlot(int a_X, int a_Y) const
{
	return GetSlot(GetSlotNum(a_X, a_Y));
}





cItem & cItemGrid::GetSlot(int a_X, int a_Y)
{
	return GetSlot(GetSlotNum(a_X, a_Y));
}





const cItem & cItemGrid::GetSlot(int a_SlotNum) const
{
	if ((a_SlotNum < 0) || (a_SlotNum >= m_NumSlots))
	{
		LOGWARNING("%s: Invalid slot number, %d out of %d slots",
			__FUNCTION__, a_SlotNum, m_NumSlots
		);
		return m_Slots[0];
	}
	return m_Slots[a_SlotNum];
}





cItem & cItemGrid::GetSlot(int a_SlotNum)
{
	if ((a_SlotNum < 0) || (a_SlotNum >= m_NumSlots))
	{
		LOGWARNING("%s: Invalid slot number, %d out of %d slots",
			__FUNCTION__, a_SlotNum, m_NumSlots
		);
		return m_Slots[0];
	}
	return m_Slots[a_SlotNum];
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
	if ((a_SlotNum < 0) || (a_SlotNum >= m_NumSlots))
	{
		LOGWARNING("%s: Invalid slot number %d out of %d slots",
			__FUNCTION__, a_SlotNum, m_NumSlots
		);
		return;
	}
	m_Slots[a_SlotNum] = a_Item;
}





void cItemGrid::SetSlot(int a_SlotNum, short a_ItemType, char a_ItemCount, short a_ItemDamage)
{
	SetSlot(a_SlotNum, cItem(a_ItemType, a_ItemCount, a_ItemDamage));
}





void cItemGrid::Clear(void)
{
	for (int i = 0; i < m_NumSlots; i++)
	{
		m_Slots[i].Empty();
	}
}





int cItemGrid::HowManyCanFit(const cItem & a_ItemStack)
{
	char NumLeft = a_ItemStack.m_ItemCount;
	int MaxStack = ItemHandler(a_ItemStack.m_ItemType)->GetMaxStackSize();
	for (int i = m_NumSlots - 1; i >= 0; i--)
	{
		if (m_Slots[i].IsEmpty())
		{
			NumLeft -= MaxStack;
		}
		else if (m_Slots[i].IsStackableWith(a_ItemStack))
		{
			NumLeft -= MaxStack - m_Slots[i].m_ItemCount;
		}
		if (NumLeft <= 0)
		{
			// All items fit
			return a_ItemStack.m_ItemCount;
		}
	}  // for i - m_Slots[]
	return a_ItemStack.m_ItemCount - NumLeft;
}





bool cItemGrid::AddItem(cItem & a_ItemStack)
{
	int NumLeft = a_ItemStack.m_ItemCount;
	int MaxStack = ItemHandler(a_ItemStack.m_ItemType)->GetMaxStackSize();
	for (int i = m_NumSlots - 1; i >= 0; i--)
	{
		if (m_Slots[i].IsEmpty())
		{
			m_Slots[i] = a_ItemStack;
			m_Slots[i].m_ItemCount = std::min(MaxStack, NumLeft);
			NumLeft -= m_Slots[i].m_ItemCount;
		}
		else if (m_Slots[i].IsStackableWith(a_ItemStack))
		{
			int PrevCount = m_Slots[i].m_ItemCount;
			m_Slots[i].m_ItemCount = std::min(MaxStack, PrevCount + NumLeft);
			NumLeft -= m_Slots[i].m_ItemCount - PrevCount;
		}
		if (NumLeft <= 0)
		{
			// All items fit
			return true;
		}
	}  // for i - m_Slots[]
	return (a_ItemStack.m_ItemCount > NumLeft);
}





bool cItemGrid::AddItems(cItems & a_ItemStackList)
{
	bool res;
	for (cItems::iterator itr = a_ItemStackList.begin(); itr != a_ItemStackList.end();)
	{
		res = AddItem(*itr) | res;
		if (itr->IsEmpty())
		{
			itr = a_ItemStackList.erase(itr);
		}
		else
		{
			++itr;
		}
	}
	return res;
}





int cItemGrid::GetFirstEmptySlot(void) const
{
	return GetNextEmptySlot(-1);
}





int cItemGrid::GetLastEmptySlot(void) const
{
	for (int i = m_NumSlots - 1; i >= 0; i--)
	{
		if (m_Slots[i].IsEmpty())
		{
			return i;
		}
	}
	return -1;
}





int cItemGrid::GetNextEmptySlot(int a_StartFrom) const
{
	for (int i = a_StartFrom + 1; i < m_NumSlots; i++)
	{
		if (m_Slots[i].IsEmpty())
		{
			return i;
		}
	}
	return -1;
}





void cItemGrid::CopyToItems(cItems & a_Items) const
{
	for (int i = 0; i < m_NumSlots; i++)
	{
		if (!m_Slots[i].IsEmpty())
		{
			a_Items.push_back(m_Slots[i]);
		}
	}  // for i - m_Slots[]
}





void cItemGrid::GenerateRandomLootWithBooks(const cLootProbab * a_LootProbabs, int a_CountLootProbabs, int a_NumSlots, int a_Seed)
{
	// Calculate the total weight:
	int TotalProbab = 1;
	for (int i = 0; i < a_CountLootProbabs; i++)
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
		cItem CurrentLoot = cItem(E_ITEM_BOOK, 1, 0);  // TODO: enchantment
		for (int j = 0; j < a_CountLootProbabs; j++)
		{
			LootRnd -= a_LootProbabs[i].m_Weight;
			if (LootRnd < 0)
			{
				CurrentLoot = a_LootProbabs[i].m_Item;
				CurrentLoot.m_ItemCount = a_LootProbabs[i].m_MinAmount + (Rnd % (a_LootProbabs[i].m_MaxAmount - a_LootProbabs[i].m_MinAmount));
				Rnd >>= 8;
				break;
			}
		}  // for j - a_LootProbabs[]
		SetSlot(Rnd % m_NumSlots, CurrentLoot);
	}  // for i - NumSlots
}




