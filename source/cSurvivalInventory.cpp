
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cSurvivalInventory.h"
#include "cPlayer.h"
#include "cClientHandle.h"
#include "cWindow.h"
#include "cItem.h"
#include "CraftingRecipes.h"
#include "cRoot.h"
#include "items/Item.h"





cSurvivalInventory::cSurvivalInventory(cPlayer* a_Owner)
	: cInventory(a_Owner)
{
}





cSurvivalInventory::~cSurvivalInventory()
{
}





void cSurvivalInventory::Clicked(
	short a_SlotNum, bool a_IsRightClick, bool a_IsShiftPressed, 
	const cItem & a_HeldItem
)
{
	cWindow * Window = GetWindow();
	if (
		a_IsShiftPressed &&      // Shift pressed
		(Window != NULL) &&      // Window is valid
		(Window->GetDraggingItem()->IsEmpty())  // Not dragging anything
	)
	{
		ShiftClicked(a_SlotNum);
		return;
	}
	
	bool bDontCook = false;
	if (Window != NULL)
	{
		// Override for craft result slot
		if (a_SlotNum == (short)c_CraftOffset)
		{
			LOGD("Clicked in craft result slot, item there: %d:%d (%d times) !!", m_Slots[c_CraftOffset].m_ItemID, m_Slots[c_CraftOffset].m_ItemHealth, m_Slots[c_CraftOffset].m_ItemCount);
			cItem * DraggingItem = Window->GetDraggingItem();
			if (DraggingItem->IsEmpty())
			{
				*DraggingItem = m_Slots[c_CraftOffset];
				m_Slots[c_CraftOffset].Empty();
			}
			else if (DraggingItem->IsEqual(m_Slots[c_CraftOffset]))
			{
				cItemHandler * Handler = ItemHandler(m_Slots[c_CraftOffset].m_ItemID);
				if (DraggingItem->m_ItemCount + m_Slots[c_CraftOffset].m_ItemCount <= Handler->GetMaxStackSize())
				{
					DraggingItem->m_ItemCount += m_Slots[c_CraftOffset].m_ItemCount;
					m_Slots[c_CraftOffset].Empty();
				}
				else
				{
					bDontCook = true;
				}
			}
			else
			{
				bDontCook = true;
			}
		}
		else
		{
			Window->Clicked(*m_Owner, 0, a_SlotNum, a_IsRightClick, a_IsShiftPressed, a_HeldItem);
		}
	}
	else
	{
		ASSERT(!"No inventory window! WTF?");
		LOG("No Inventory window! WTF");
	}

	if ((a_SlotNum >= (short)c_CraftOffset) && (a_SlotNum < (short)(c_CraftOffset + c_CraftSlots + 1)))
	{
		cCraftingGrid   Grid(m_Slots + c_CraftOffset + 1, 2, 2);
		cCraftingRecipe Recipe(Grid);
		
		cRoot::Get()->GetCraftingRecipes()->GetRecipe(m_Owner, Grid, Recipe);
		
		if ((a_SlotNum == 0) && !bDontCook)
		{
			// Consume the items from the crafting grid:
			Recipe.ConsumeIngredients(Grid);
			
			// Propagate grid back to m_Slots:
			Grid.CopyToItems(m_Slots + c_CraftOffset + 1);
			
			// Get the recipe for the new grid contents:
			cRoot::Get()->GetCraftingRecipes()->GetRecipe(m_Owner, Grid, Recipe);
		}
		m_Slots[c_CraftOffset] = Recipe.GetResult();
		LOGD("%s cooked: %d:%d (%d times) !!", m_Owner->GetName().c_str(), m_Slots[c_CraftOffset].m_ItemID, m_Slots[c_CraftOffset].m_ItemHealth, m_Slots[c_CraftOffset].m_ItemCount );
		SendWholeInventory(m_Owner->GetClientHandle());
	}
	SendSlot(0);
}





void cSurvivalInventory::ShiftClicked(short a_SlotNum)
{
	ASSERT((GetWindow() == NULL) || (GetWindow()->GetDraggingItem()->IsEmpty()));  // Cannot handle shift-click if dragging something
	
	if (a_SlotNum == SLOT_CRAFTING_RESULT)
	{
		ShiftClickedCraftingResult(a_SlotNum);
	}
	else if ((a_SlotNum >= SLOT_CRAFTING_MIN) && (a_SlotNum <= SLOT_CRAFTING_MAX))
	{
		ShiftClickedCraftingGrid(a_SlotNum);
	}
	else if ((a_SlotNum >= SLOT_ARMOR_MIN) && (a_SlotNum <= SLOT_ARMOR_MAX))
	{
		ShiftClickedArmor(a_SlotNum);
	}
	else if ((a_SlotNum >= SLOT_HOTBAR_MIN) && (a_SlotNum <= SLOT_HOTBAR_MAX))
	{
		ShiftClickedHotbar(a_SlotNum);
	}
	else
	{
		ShiftClickedInventory(a_SlotNum);
	}
	// Because the client tries to guess our actions and is not always right, send the whole inventory:
	SendWholeInventoryToAll();
}





void cSurvivalInventory::ShiftClickedCraftingResult(short a_Slot)
{
	// Craft until either the recipe changes (due to ingredients) or there's not enough storage for the result
	cItem * CraftingResult = GetSlot(SLOT_CRAFTING_RESULT);
	if ((CraftingResult == NULL) || CraftingResult->IsEmpty())
	{
		return;
	}
	cItem ResultCopy = *CraftingResult;
	int HowManyItemsWillFit = HowManyCanFit(CraftingResult->m_ItemID, CraftingResult->m_ItemHealth, SLOT_INVENTORY_MIN, SLOT_INVENTORY_MAX);
	HowManyItemsWillFit += HowManyCanFit(CraftingResult->m_ItemID, CraftingResult->m_ItemHealth, SLOT_HOTBAR_MIN, SLOT_HOTBAR_MAX);
	int HowManyPassesWillFit = HowManyItemsWillFit / CraftingResult->m_ItemCount;
	for (int i = 0; i < HowManyPassesWillFit; i++)
	{
		// First try moving into the hotbar:
		int NumMoved = MoveItem(CraftingResult->m_ItemID, CraftingResult->m_ItemHealth, CraftingResult->m_ItemCount, SLOT_HOTBAR_MIN, SLOT_HOTBAR_MAX);
		
		// If something didn't fit, move into main inventory:
		if (NumMoved < CraftingResult->m_ItemCount)
		{
			MoveItem(CraftingResult->m_ItemID, CraftingResult->m_ItemHealth, CraftingResult->m_ItemCount - NumMoved, SLOT_INVENTORY_MIN, SLOT_INVENTORY_MAX);
		}
		
		// "Use" the crafting recipe once:
		cCraftingGrid   Grid(m_Slots + SLOT_CRAFTING_MIN, 2, 2);
		cCraftingRecipe Recipe(Grid);
		cRoot::Get()->GetCraftingRecipes()->GetRecipe(m_Owner, Grid, Recipe);
		Recipe.ConsumeIngredients(Grid);
		Grid.CopyToItems(m_Slots + c_CraftOffset + 1);
		cRoot::Get()->GetCraftingRecipes()->GetRecipe(m_Owner, Grid, Recipe);
		m_Slots[SLOT_CRAFTING_RESULT] = Recipe.GetResult();
		
		// If the recipe changed, abort:
		if (!Recipe.GetResult().IsEqual(ResultCopy))
		{
			break;
		}
	}
}





void cSurvivalInventory::ShiftClickedCraftingGrid(short a_Slot)
{
	// Move the item from the crafting grid into the main inventory:
	cItem * Item = GetSlot(a_Slot);
	if ((Item == NULL) || Item->IsEmpty())
	{
		return;
	}
	// First try the main inventory:
	Item->m_ItemCount -= MoveItem(Item->m_ItemID, Item->m_ItemHealth, Item->m_ItemCount, SLOT_INVENTORY_MIN, SLOT_INVENTORY_MAX);
	
	// If anything left, try the hotbar:
	if (Item->m_ItemCount > 0)
	{
		Item->m_ItemCount -= MoveItem(Item->m_ItemID, Item->m_ItemHealth, Item->m_ItemCount, SLOT_HOTBAR_MIN, SLOT_HOTBAR_MAX);
	}
	SendSlot(a_Slot);
}





void cSurvivalInventory::ShiftClickedArmor(short a_Slot)
{
	// Move the item from the armor slot into the main inventory:
	cItem * Item = GetSlot(a_Slot);
	if ((Item == NULL) || Item->IsEmpty())
	{
		return;
	}
	Item->m_ItemCount -= MoveItem(Item->m_ItemID, Item->m_ItemHealth, Item->m_ItemCount, SLOT_INVENTORY_MIN, SLOT_INVENTORY_MAX);
	SendSlot(a_Slot);
}





void cSurvivalInventory::ShiftClickedHotbar(short a_Slot)
{
	// Move the item from the hotbar into the main inventory:
	cItem * Item = GetSlot(a_Slot);
	if ((Item == NULL) || Item->IsEmpty())
	{
		return;
	}
	Item->m_ItemCount -= MoveItem(Item->m_ItemID, Item->m_ItemHealth, Item->m_ItemCount, SLOT_INVENTORY_MIN, SLOT_INVENTORY_MAX);
	SendSlot(a_Slot);
}





void cSurvivalInventory::ShiftClickedInventory(short a_Slot)
{
	// Move the item from the main inventory into armor slot if it is armor, or the hotbar otherwise:
	cItem * Item = GetSlot(a_Slot);
	if ((Item == NULL) || Item->IsEmpty())
	{
		return;
	}
	if (ItemCategory::IsHelmet(Item->m_ItemID))
	{
		Item->m_ItemCount -= MoveItem(Item->m_ItemID, Item->m_ItemHealth, Item->m_ItemCount, SLOT_ARMOR_HELMET, SLOT_ARMOR_HELMET);
	}
	else if (ItemCategory::IsChestPlate(Item->m_ItemID))
	{
		Item->m_ItemCount -= MoveItem(Item->m_ItemID, Item->m_ItemHealth, Item->m_ItemCount, SLOT_ARMOR_CHESTPLATE, SLOT_ARMOR_CHESTPLATE);
	}
	else if (ItemCategory::IsLeggings(Item->m_ItemID))
	{
		Item->m_ItemCount -= MoveItem(Item->m_ItemID, Item->m_ItemHealth, Item->m_ItemCount, SLOT_ARMOR_LEGGINGS, SLOT_ARMOR_LEGGINGS);
	}
	else if (ItemCategory::IsBoots(Item->m_ItemID))
	{
		Item->m_ItemCount -= MoveItem(Item->m_ItemID, Item->m_ItemHealth, Item->m_ItemCount, SLOT_ARMOR_BOOTS, SLOT_ARMOR_BOOTS);
	}
	else
	{
		Item->m_ItemCount -= MoveItem(Item->m_ItemID, Item->m_ItemHealth, Item->m_ItemCount, SLOT_HOTBAR_MIN, SLOT_HOTBAR_MAX);
	}
	SendSlot(a_Slot);
}




